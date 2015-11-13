#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>

int32_t main(int32_t argc, char *argv[]) {
    const bool is_write = argc < 2 || argv[1][0] == 'w';
    const int32_t SIZE = 128;

    int32_t fd = shm_open("shm_test", O_CREAT|O_EXCL|O_RDWR, S_IRUSR | S_IWUSR);

    if (fd < 0) {
        if (errno == EEXIST) {
            close(fd);
            std::cout << "File existed, Opening as read write\n";
            fd = shm_open("Shm_test", O_RDWR, S_IRUSR | S_IWUSR);
        } else {
            fd = shm_open("Shm_test", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
            std::cout << "File did not exist, creating\n";
        }
    }

    if (fd < 0) {
        std::cout << "Error opening shared memory\n";
        exit(EXIT_FAILURE);
    }

    if(is_write) {
        std::cout << "Truncating\n";
        ftruncate(fd, SIZE);
    }

    std::cout << "Mapping to shared memory\n";
    uint8_t* shared_memory = (uint8_t*)mmap(NULL, SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    std::cout << "Shared address " << (uint64_t)shared_memory << "\n";

    if(is_write) {
        std::cout << "Clearing memory\n";
        memset(shared_memory, 0, SIZE);
        std::cout << "About to write\n";
        uint8_t cnt = 0;
        while(true) {
            for(int32_t index = 0; index < SIZE; index++) {
                shared_memory[index] = cnt;
            }
            cnt++;
            usleep (250 * 1000); // 250 ms
        }
    } else {
        while(true) {
            for(int32_t index = 0; index < SIZE; index++) {
                std::cout << "Just read [" << index << "]: " << (int)shared_memory[index] << "\n";
            }
            usleep (1000 * 1000); // 1sec
        }
    }

    munmap(shared_memory, SIZE);

    return 0;
}
