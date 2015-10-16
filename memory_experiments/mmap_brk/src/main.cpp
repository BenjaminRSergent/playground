#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

void print_data_end() {
    std::cout << "Current end " << sbrk(0) << "\n";
}

int main(int argc, char *argv[])
{
    print_data_end();
    const int size = 100 * sizeof(int);
    int fd = open("test_map", O_CREAT | O_RDWR);

    print_data_end();

    std::cout << "Mapping file\n";
    int* map = (int*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    std::cout << "Anonymous mapping\n";
    int* anon = (int*)mmap( 0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0 );

    anon[99] = 100;
    std::cout << "Wrote to mmap'd memory\n";

    srand(time(0));
    int to_malloc = rand() % 1000;
    std::cout << "Going to malloc " << to_malloc << "\n";
    int* memory = (int*)malloc(to_malloc);
    memory[10] = 1;
    print_data_end();
    free(memory);

    std::cout << "Calling sbrk\n";
    int* end = (int*)sbrk(0);
    //end[1] = 1; Would segfault
    sbrk(100);
    end[1] = 1;
    std::cout << "Wrote to expanded data section\n";
    print_data_end();

    if (map == MAP_FAILED) {
	    close(fd);
	    perror("Error mmapping the file");
	    exit(EXIT_FAILURE);
    }
    
    if(argc < 2 || argv[1][0] == '0') {
        int result = lseek(fd, size-1, SEEK_SET);
        if (result == -1) {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            exit(EXIT_FAILURE);
        }
        write(fd, "", 1);

        for(int index = 0; index < size/sizeof(int); index++) {
            map[index] = index * 2;                        
        }
    } else {
        for(int index = 0; index < size/sizeof(int); index++) {
            std::cout << "Reading: " << index << ": " << map[index] << "\n";
        }
    }

    if (munmap(map, size) == -1) {
	    perror("Error un-mmapping the file");
    }
    if (munmap(anon, size) == -1) {
	    perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}
