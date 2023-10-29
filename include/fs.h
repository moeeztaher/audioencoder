#pragma once

#include <vector>
#include <filesystem>

namespace cin
{
    using Paths = std::vector<std::filesystem::path>;

    /**
     * Enumerate WAV files located in @p path.
     *
     * A "WAV" file is any regular file in @p path that ends in .wav. @p path is
     * not read recursively and files themselves are not opened or analyzed yet.
     *
     * @param path Path denoting a directory.
     * @return A vector of file paths.
     */
    Paths get_valid_wav_files(const std::filesystem::path& path);
}
