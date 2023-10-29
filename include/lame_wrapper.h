#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <lame/lame.h>

namespace cin
{
    /**
     * Wrap the LAME C API to encode 16 bit PCM samples.
     */
    class Lame {
    public:
        /**
         * Thrown by Lame::Lame() in case of configuration problems.
         */
        class ConfigurationFailure : public std::runtime_error {
        public:
            /**
             * Construct ConfigurationFailure.
             *
             * @param msg Error message.
             */
            ConfigurationFailure(const char *msg) : std::runtime_error{msg} {}
        };

        /**
         * Thrown by encode() or flush() in case encoding samples failed.
         */
        class EncodeError : public std::runtime_error {
        public:
            /**
             * Construct EncodeError.
             *
             * @param msg Error message.
             */
            EncodeError(const char *msg) : std::runtime_error{msg} {}
        };

        /**
         * Construct a new LAME wrapper.
         *
         * @param num_channels Number of channels.
         * @param sample_rate Sample rate of the input data.
         * @throws ConfigurationFailure in case @p num_channels or @p
         *   sample_rate is invalid.
         */
        Lame(int num_channels, int sample_rate);

        /**
         * Encode PCM samples into MP3 blocks.
         *
         * @param samples 16 bit PCM samples.
         * @param[out] data Output MP3 data, vector is resized if necessary.
         * @throws EncodeError in case encoding error.
         * @returns Size of @p data.
         */
        size_t encode(const std::vector<int16_t>& samples, std::vector<uint8_t>& data) const;

        /**
         * Encode remaining data.
         *
         * @param[out] data Output MP3 data, vector is resized if necessary.
         * @throws EncodeError in case encoding error.
         * @returns Size of @p data in number of bytes.
         */
        size_t flush(std::vector<uint8_t>& data) const;

    private:
        bool m_mono;
        std::unique_ptr<lame_global_flags, void(*)(lame_t)> m_lame;
    };
}
