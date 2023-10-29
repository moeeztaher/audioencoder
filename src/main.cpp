#include <filesystem>
#include "log.h"
#include "fs.h"
#include "encoder.h"
#include <chrono>
#include <thread>
#include <iostream>

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

        auto t3 = high_resolution_clock::now();
        encoder.encodemulti();
        auto t4 = high_resolution_clock::now();

        /* Getting number of milliseconds as an integer. */
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        auto ms_int2 = duration_cast<milliseconds>(t4 - t3);

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        duration<double, std::milli> ms_double2 = t4 - t3;

        const unsigned int numCores = std::thread::hardware_concurrency();
        // std::cout<<"number of cores\n"<<numCores<<std::flush;

         cin::log::debug(" Single thread: {:.2f} ms, Multi thread: {:.2f} ms, number of cores: {}",
            ms_double.count(),
            ms_double2.count(),
            numCores
            );

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error) {
        cin::log::error("Runtime error: {}", error.what());
        return EXIT_FAILURE;
    }
}
