#include <sndfile.h>
#include "encoder.h"
#include "log.h"
#include "wav.h"

namespace
{
    void close_sf(SNDFILE *sf)
    {
        if (sf == nullptr) {
            return;
        }

        sf_close(sf);
    }
}

cin::WavFile::WavFile(const std::filesystem::path& path)
: m_sf{sf_open(path.c_str(), SFM_READ, &m_info), close_sf}
{
    if (!m_sf) {
        throw cin::WavFile::CouldNotRead{sf_strerror(nullptr)};
    }

    if ((m_info.format & SF_FORMAT_WAV) == 0) {
        cin::log::debug("Format bitmask is 0x{:x}", m_info.format);
        throw cin::Encoder::UnsupportedFormat{"not a WAV file"};
    }

    cin::log::debug(" channels={}, sample rate={}", m_info.channels, m_info.samplerate);
}

int cin::WavFile::num_channels() const
{
    return m_info.channels;
}

int cin::WavFile::num_samples() const
{
    return m_info.frames;
}

int cin::WavFile::sample_rate() const
{
    return m_info.samplerate;
}

size_t cin::WavFile::read_samples(std::vector<int16_t>& samples, int num_frames) const
{
    samples.resize(num_frames * m_info.channels * sizeof(int16_t));
    const auto read_size{static_cast<size_t>(sf_readf_short(m_sf.get(), samples.data(), num_frames)) * m_info.channels};
    samples.resize(read_size);
    return read_size;
}
