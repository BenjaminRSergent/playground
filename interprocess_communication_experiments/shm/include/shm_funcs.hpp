#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>


#define MEMORY_NAME "shm_test"
#define MEMORY_SIZE 128
#define ERROR_RC -1

inline int32_t get_shm(const char* name) {
    int32_t fd = shm_open(name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR | S_IWUSR);

    if (fd < 0){
        if (errno == EEXIST){
            close(fd);
            std::cout << "File existed, Opening as read write\n";
            fd = shm_open("Shm_test", O_RDWR, S_IRUSR | S_IWUSR);
        } else {
            fd = shm_open("Shm_test", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
            std::cout << "File did not exist, creating\n";
        }
    }
    
    if (fd < 0){
        std::cout << "Error opening shared memory\n";
        return ERROR_RC;
    }
    return fd;
}

