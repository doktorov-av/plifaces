//
// Created by doktorov-av on 2/12/26.
//

#include "../include/private/DirEntry.hpp"

#include <unistd.h>

#include "Directory.hpp"

DirEntry::DirEntry(Directory* parent, dirent* entry) : m_Parent(parent), m_Entry(entry) {}

DirEntry::DirEntry(dirent* entry) : m_Entry(entry) {}

std::optional<std::string> DirEntry::ReadlinkOptional() const {
    if (m_Entry->d_type != DT_LNK) {
        return std::nullopt;
    }
    std::filesystem::path fPath;
    if (m_Parent != nullptr) {
        fPath = m_Parent->GetPath();
    }
    fPath = fPath / m_Entry->d_name;

    std::array<char, PATH_MAX> link{};
    const auto bytesRead = readlink(fPath.c_str(), link.data(), link.size());
    if (bytesRead == -1) {
        perror("readlink");
        return std::nullopt;
    }

    return std::string(link.data(), bytesRead);
}

std::string_view DirEntry::GetName() const {
    return m_Entry->d_name;
}
