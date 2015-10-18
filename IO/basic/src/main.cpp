#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

const char* file_name = "test_file.txt";

void read_stdin() {
    std::cout << "Reading stdin\n";
    char buffer[255];

    read(STDIN_FILENO, buffer, 255);

    std::cout << "read:" << buffer << "\n";
}

void write_stdout(const char* msg) {
    std::cout << "Writing to stdout with file\n";
    int fd = STDOUT_FILENO;
    int cloned_fd = dup(STDOUT_FILENO);
    write(fd, msg, strlen(msg));
    
    std::cout << "About to close stdout\n";
    close(fd);
    std::cout << "Writing after closing\n";    
    dup2(cloned_fd, fd);
    std::cout << "Writing after reopeneing. The last message (Writing After Closing) shouldn't have appeared\n";   
}

void read_all(const char* name) {
    std::cout << "Reading all contents of " << name << "\n";
    int fd = open(name, O_RDONLY);

    const int MAX_SIZE = 255;
    char buff[MAX_SIZE] = {0};
    char* buffer = buff;
    int len = MAX_SIZE;
    int ret = 0;
    while(len != 0 && (ret = read(fd, buffer, len)) != 0) {
        if(ret == -1) {
            if(errno == EINTR) {
                continue;
            } else {
                perror("read");
                break;
            }
        }
        len -= ret;
        buffer = &buffer[ret];
    }
    std::cout << buff << "\n";

    close(fd);
}

void write(const char* name, const char* content, bool append) {
    std::cout << "About to write " << content << " to " << name << (append ? " not " : " ") << "appending\n";
    int opts = O_WRONLY | (append ? O_APPEND : 0);
    int fd = open(name, opts);

    write(fd, content, strlen(content));

    close(fd);
}

int32_t main(int32_t argc, char *argv[]) {
    write_stdout("Test Message\n");
    read_stdin();

    int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY);
    
    write(file_name, "First Write", false);
    read_all(file_name);
    write(file_name, "Second", false);
    read_all(file_name);
    write(file_name, "Third", true);
    read_all(file_name);

    std::cout << "Truncating\n";
    truncate(file_name, 10);
    
    read_all(file_name);

    std::cout << "Position write at 5\n";
    const char* msg = "Test message";   
    pwrite(fd, msg, strlen(msg), 5);    
    read_all(file_name);

    close(fd);
    return 0;
}
