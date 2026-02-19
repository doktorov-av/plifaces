//
// Created by doktorov-av on 2/12/26.
//

#include "Directory.hpp"

#include <cstring>
#include <dirent.h>

Directory::Directory(const std::filesystem::path& path) {
    Open(path);
}

void Directory::Open(const std::filesystem::path& path) {
    m_Path = path;
    m_Dir = opendir(path.c_str());
    if (!m_Dir) {
        throw std::runtime_error(strerror(errno));
    }
}

std::filesystem::path Directory::GetPath() const {
    return m_Path;
}

void Directory::ForEachEntry(const EntryProcessor& f) {
    if (!m_Dir) {
        throw std::runtime_error("can't iterate other null dir");
    }

    dirent* entry = nullptr;
    while ((entry = readdir(m_Dir)) != nullptr) {
        f(DirEntry(this, entry));
    }
}

void Directory::Close() {
    if (m_Dir) {
        closedir(m_Dir);
        m_Dir = nullptr;
    }
}
