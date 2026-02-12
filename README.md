# Listening interfaces

This C++20 library provides utility functions and classes that allow user to fetch listening interfaces of specific process using it's pid

This library is designed to work on Linux and tested under Ubuntu 24.04.3 LTS

## Usage

    1` auto fds = proc::ReadFileDescriptors(42428);
    2` auto sockets = proc::ExtractSockets(std::move(fds));
    3` auto nodes = proc::ExtractSocketsNodes(sockets);
    4` auto addrs = proc::ExtractLocalAddress(nodes);

1. This function returns entries of /proc/{pid}/fd directory that are also readlink()'ed
2. This function extracts socket symbolic links like socket:[2248868] (see [man7.org](https://man7.org/linux/man-pages/man5/proc_pid_fd.5.html))
3. This function drops everything but inode from step 2
4. This function extracts localaddr:ip information from /proc/net/tcp file similarly to `ss` or `netstat`

No you have all your TCP listening interface addresses of specific process extracted by its id
