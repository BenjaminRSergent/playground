#include "shm_funcs.hpp"

int32_t main(int32_t argc, char *argv[]) {
    const int32_t SIZE = 128;

    int32_t fd = get_shm(MEMORY_NAME);

    if (fd < 0) {
        std::cout << "Error opening shared memory\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Mapping to shared memory\n";
    uint8_t* shared_memory = (uint8_t*)mmap(NULL, SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    std::cout << "Shared address " << (uint64_t)shared_memory << "\n";

    while(true) {
        for(int32_t index = 0; index < SIZE; index++) {
            std::cout << "Just read [" << index << "]: " << (int)shared_memory[index] << "\n";
        }
        usleep (1000 * 1000); // 1sec
    }

    munmap(shared_memory, SIZE);

    return 0;
}
