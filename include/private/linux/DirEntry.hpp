//
// Created by doktorov-av on 2/12/26.
//

#ifndef SHMEM_DIRENTRY_HPP
#define SHMEM_DIRENTRY_HPP
#include <dirent.h>
#include <filesystem>
#include <optional>
#include <string>

class Directory;

class DirEntry {
public:
    explicit DirEntry(Directory* parent, dirent* entry);
    explicit DirEntry(dirent* entry);

    [[nodiscard]] std::optional<std::string> ReadlinkOptional() const;

    [[nodiscard]] std::string_view GetName() const;

private:
    Directory* m_Parent = nullptr;
    dirent* m_Entry = nullptr;
};

#endif//SHMEM_DIRENTRY_HPP