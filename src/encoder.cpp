#include <fstream>
#include "encoder.h"
#include "lame_wrapper.h"
#include "log.h"
#include "wav.h"
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <cstdint>
#include <filesystem>


namespace
{
    void encode_file(const std::filesystem::path& path)
    {
        cin::log::info("Encoding {}", path.string());

        cin::WavFile wav_file{ path };

        if (wav_file.num_channels() > 2) {
            throw cin::Encoder::UnsupportedFormat("More than two channels are not supported");
        }

        std::filesystem::path output_path{ path };
        output_path.replace_extension(".mp3");
        std::ofstream mp3_file{ output_path, std::ios::binary };

        const int num_channels{ wav_file.num_channels() };
        cin::Lame lame{ num_channels, wav_file.sample_rate() };

        constexpr size_t num_samples{ 1024 };
        constexpr size_t mp3_buffer_size{ static_cast<size_t>(num_samples * 1.25) + 7200 };
        std::vector<uint8_t> mp3_buffer;
        mp3_buffer.reserve(mp3_buffer_size);
        std::vector<int16_t> sample_buffer;

        const size_t num_frames{ num_samples / (num_channels == 1 ? 1 : 2) };
        size_t read_size{ 0 };
        size_t write_size{ 0 };

        std::mutex buffer_mutex; // Mutex to protect mp3_buffer

        const int num_threads = std::thread::hardware_concurrency(); // Get the number of CPU cores

        // Function to perform encoding on a chunk of samples
        auto encode_chunk = [&wav_file, &lame, &mp3_buffer, &sample_buffer, &mp3_file, &read_size, &write_size, &buffer_mutex, num_frames]() {
            while (true) {
                std::vector<int16_t> local_sample_buffer;
                local_sample_buffer.reserve(num_frames);

                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    if (sample_buffer.empty()) {
                        return; // No more work to do
                    }

                    read_size += wav_file.read_samples(local_sample_buffer, num_frames) * sizeof(uint16_t);
                }

                mp3_buffer.resize(mp3_buffer_size);
                write_size += lame.encode(local_sample_buffer, mp3_buffer);

                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    mp3_file.write((char*)mp3_buffer.data(), mp3_buffer.size());
                }
            }
            };

        std::vector<std::thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(encode_chunk);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        constexpr float bytes_per_kib{ 1024.0F };

        cin::log::debug(" size reduced from {:.2f} KiB to {:.2f} KiB ({:.2f}x smaller)",
            read_size / bytes_per_kib,
            write_size / bytes_per_kib,
            1.0F * read_size / write_size);
    }
}

cin::Encoder::Encoder(cin::Paths&& paths)
: m_paths{std::move(paths)}
{}

void cin::Encoder::encode() const
{
    for (const auto& path: m_paths) {
        try {
            encode_file(path);
        }
        catch (const cin::WavFile::CouldNotRead& err) {
            cin::log::error("Could not read {}: {}", path.c_str(), err.what());
        }
        catch (const cin::Encoder::UnsupportedFormat& err) {
            cin::log::error("{} has unsupported format: {}", path.c_str(), err.what());
        }
        catch (const cin::Lame::ConfigurationFailure& err) {
            cin::log::error("Could not configure LAME: {}", err.what());
        }
        catch (const cin::Lame::EncodeError& err) {
            cin::log::error("Failed to encode samples: {}", err.what());
        }
    }
}
