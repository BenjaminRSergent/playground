#include "shm_funcs.hpp"

int32_t main(int32_t argc, char *argv[]) {
    const int32_t SIZE = 128;

    int32_t fd = get_shm(MEMORY_NAME);
    if (fd < 0){
        std::cout << "Error opening shared memory\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Truncating\n";
    ftruncate(fd, SIZE);

    std::cout << "Mapping to shared memory\n";
    uint8_t* shared_memory = (uint8_t*)mmap(NULL, MEMORY_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    std::cout << "Shared address " << (uint64_t)shared_memory << "\n";

    std::cout << "Clearing memory\n";
    memset(shared_memory, 0, MEMORY_SIZE);
    std::cout << "About to write\n";
    uint8_t cnt = 0;
    while(true) {
        for(int32_t index = 0; index < MEMORY_SIZE; index++) {
            shared_memory[index] = cnt;
        }   
        cnt++;
        usleep (250 * 1000); // 250 ms
    }

    munmap(shared_memory, SIZE);

    return 0;
}
