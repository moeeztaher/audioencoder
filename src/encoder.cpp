#include <fstream>
#include "encoder.h"
#include "lame_wrapper.h"
#include "log.h"
#include "wav.h"
#include <chrono>

namespace
{
    void encode_file(const std::filesystem::path& path)
    {
        cin::log::info("Encoding {}", path.string());

        cin::WavFile wav_file{path};

        if (wav_file.num_channels() > 2) {
            throw cin::Encoder::UnsupportedFormat("More than two channels are not supported");
        }

        std::filesystem::path output_path{path};
        output_path.replace_extension(".mp3");
        std::ofstream mp3_file{output_path, std::ios::binary};

        const int num_channels{wav_file.num_channels()};
        cin::Lame lame{num_channels, wav_file.sample_rate()};

        constexpr size_t num_samples{1024};
        constexpr size_t mp3_buffer_size{static_cast<size_t>(num_samples * 1.25) + 7200};
        std::vector<uint8_t> mp3_buffer;
        mp3_buffer.reserve(mp3_buffer_size);
        std::vector<int16_t> sample_buffer;

        const size_t num_frames{num_samples / (num_channels == 1 ? 1 : 2)};
        size_t read_size{0};
        size_t write_size{0};
        auto t1 = high_resolution_clock::now();
        
        

        while (true) {
            read_size += wav_file.read_samples(sample_buffer, num_frames) * sizeof(uint16_t);

            mp3_buffer.resize(mp3_buffer_size);

            if (sample_buffer.empty()) {
                write_size += lame.flush(mp3_buffer);
                mp3_file.write((char *) mp3_buffer.data(), mp3_buffer.size());
                break;
            }

            write_size += lame.encode(sample_buffer, mp3_buffer);
            mp3_file.write((char *) mp3_buffer.data(), mp3_buffer.size());
        }

        auto t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as an integer. */
        auto ms_int = duration_cast<milliseconds>(t2 - t1);

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;

        constexpr float bytes_per_kib{1024.0F};

        cin::log::debug(" size reduced from {:.2f} KiB to {:.2f} KiB ({:.2f}x smaller) in {:.2f} seconds",
            read_size / bytes_per_kib,
            write_size / bytes_per_kib,
            1.0F * read_size / write_size,
            ms_double.count()
            );
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
