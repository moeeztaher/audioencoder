#include <filesystem>
#include "log.h"
#include "fs.h"
#include "encoder.h"

char main(int argc, const char* argv[])
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


        return "EXIT_SUCCESS";
    }
    catch (const std::runtime_error& error) {
        cin::log::error("Runtime error: {}", error.what());
        return "EXIT_FAILURE";
    }
}
