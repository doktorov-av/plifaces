
//
// Created by doktorov-av on 2/12/26.
//

#ifndef SHMEM_DIRECTORY_HPP
#define SHMEM_DIRECTORY_HPP
#include <dirent.h>
#include <filesystem>
#include <functional>

#include "DirEntry.hpp"

class Directory {
public:
    using EntryProcessor = std::function<void(DirEntry&&)>;

    Directory() = default;
    explicit Directory(const std::filesystem::path& path);

    void Open(const std::filesystem::path& path);

    [[nodiscard]] std::filesystem::path GetPath() const;

    void ForEachEntry(const EntryProcessor& f);

    void Close();

    ~Directory() {
        Close();
    }

private:
    std::filesystem::path m_Path = "";
    DIR* m_Dir = nullptr;
};

#endif//SHMEM_DIRECTORY_HPP