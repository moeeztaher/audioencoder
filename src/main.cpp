#include <filesystem>
#include "log.h"
#include "fs.h"
#include "encoder.h"
#include <chrono>

int main(int argc, const char* argv[])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

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

        auto t1 = high_resolution_clock::now();
        encoder.encode();
        auto t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as an integer. */
        auto ms_int = duration_cast<milliseconds>(t2 - t1);

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;

         cin::log::debug(" Total time: {:.2f} ms ",
            ms_double.count()
            );

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error) {
        cin::log::error("Runtime error: {}", error.what());
        return EXIT_FAILURE;
    }
}
