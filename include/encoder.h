#pragma once

#include "fs.h"

namespace cin
{
    /**
     * Encodes a list of potential WAV files.
     */
    class Encoder {
    public:
        /**
         * File format is not supported.
         */
        class UnsupportedFormat : public std::runtime_error {
        public:
            /**
             * Construct UnsupportedFormat error.
             *
             * @param msg Error message.
             */
            UnsupportedFormat(const char *msg) : std::runtime_error{msg} {}
        };

        /**
         * Construct a new encoder.
         *
         * @p paths List of potential WAV files.
         */
        Encoder(Paths&& paths);

        /**
         * Encode the list of files given in the constructor.
         *
         * @throws std::runtime_error in case of I/O or encoding issues.
         */
        void encodemulti() const;
        void encode() const;

    private:
        Paths m_paths;
    };
}
