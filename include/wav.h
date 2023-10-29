#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include <sndfile.h>

namespace cin
{
    /**
     * WAV file reader abstraction.
     */
    class WavFile {
    public:
        /**
         * Thrown if data could not be read.
         */
        class CouldNotRead : public std::runtime_error {
        public:
            /**
             * Construct CouldNotRead error.
             *
             * @param msg Error message.
             */
            CouldNotRead(const char *msg) : std::runtime_error{msg} {}
        };

        /**
         * Construct a new reader from a file path.
         *
         * @param path Path to the WAV file.
         * @throws CouldNotRead if @p path cannot be read.
         * @throws cin::Encoder::UnsupportedFormat if @p path is an unsupported
         *   WAV file format.
         */
        WavFile(const std::filesystem::path& path);

        /**
         * Return number of channels.
         *
         * @return Number of channels.
         */
        int num_channels() const;

        /**
         * Return number of samples.
         *
         * @return Number of samples.
         */
        int num_samples() const;

        /**
         * Return sample rate.
         *
         * @return Sample rate in Hz.
         */
        int sample_rate() const;

        /**
         * Read samples from the file.
         *
         * Read samples @p num_frames frames into @p samples. A frame consists
         * of as many samples as number of channels.
         *
         * @param samples Output data vector that is resized if necessary.
         * @param num_frames Number of frames to read.
         * @return Size of @p samples in number of bytes.
         */
        size_t read_samples(std::vector<int16_t>& samples, int num_frames) const;

    private:
        SF_INFO m_info{0, 0, 0, 0, 0, 0};
        std::unique_ptr<SNDFILE, void(*)(SNDFILE *)> m_sf;
    };
}
