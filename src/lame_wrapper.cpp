#include "lame_wrapper.h"
#include "log.h"

namespace
{
    void close_lame(lame_t lame)
    {
        if (lame == nullptr) {
            return;
        }

        lame_close(lame);
    }

    void throw_on_error(ssize_t size)
    {
        if (size >= 0) {
            return;
        }
        else if (size == -1) {
            throw cin::Lame::EncodeError("MP3 buffer too small");
        }
        else if (size == -2) {
            throw cin::Lame::EncodeError("Allocation problem");
        }
        else if (size == -3) {
            throw cin::Lame::EncodeError("lame_init_params() not called");
        }
        else if (size == -2) {
            throw cin::Lame::EncodeError("Psycho acoustic problem");
        }

        throw cin::Lame::EncodeError("unknown");
    }
}

cin::Lame::Lame(int num_channels, int sample_rate)
: m_mono{num_channels == 1}
, m_lame{lame_init(), close_lame}
{
    if (lame_set_num_channels(m_lame.get(), num_channels) < 0) {
        throw Lame::ConfigurationFailure("Could not set number of channels");
    }

    if (lame_set_in_samplerate(m_lame.get(), sample_rate) < 0) {
        throw Lame::ConfigurationFailure("Could not set input sample rate");
    }

    if (lame_set_out_samplerate(m_lame.get(), sample_rate) < 0) {
        throw Lame::ConfigurationFailure("Could not set output sample rate");
    }

    if (lame_set_mode(m_lame.get(), m_mono ? MONO : STEREO) < 0) {
        throw Lame::ConfigurationFailure("Could not set mode");
    }

    if (lame_set_quality(m_lame.get(), 3) < 0) {
        throw Lame::ConfigurationFailure("Could not set quality");
    }

    if (lame_init_params(m_lame.get()) < 0) {
        throw Lame::ConfigurationFailure("Could not initialize LAME");
    }
}

size_t cin::Lame::encode(const std::vector<int16_t>& samples, std::vector<uint8_t>& data) const
{
    const auto sample_data{const_cast<int16_t *>(samples.data())};
    const ssize_t size{m_mono
        ? lame_encode_buffer(m_lame.get(), sample_data, nullptr, samples.size(), data.data(), data.size())
        : lame_encode_buffer_interleaved(m_lame.get(), sample_data, samples.size() / 2, data.data(), data.size())
    };

    throw_on_error(size);
    data.resize(size);
    return size;
}

size_t cin::Lame::flush(std::vector<uint8_t>& data) const
{
    const ssize_t size{lame_encode_flush(m_lame.get(), data.data(), data.size())};
    throw_on_error(size);
    data.resize(size);
    return size;
}
