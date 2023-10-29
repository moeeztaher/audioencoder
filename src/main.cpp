#include <filesystem>
#include "log.h"
#include "fs.h"
#include "encoder.h"

int main(int argc, const char* argv[])
{
    cin::log::init();

    if (argc == 1) {
        cin::log::warn("Not enough arguments. Usage: {} <path-to-files>", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc > 2) {
        cin::log::warn("More than one argument given, ignoring everything after {}", argv[1]);
    }

    try {
        const cin::Encoder encoder{cin::get_valid_wav_files({argv[1]})};

        encoder.encode();

         cin::log::debug(" size reduced from {:.2f} KiB to {:.2f} KiB ({:.2f}x smaller) in {:.2f} ms",
            1.1234,
            2.1234,
            3.1234,
            4.1234
            );

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error) {
        cin::log::error("Runtime error: {}", error.what());
        return EXIT_FAILURE;
    }
}
