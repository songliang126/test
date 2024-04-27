#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>


#define SIZE 0X1000
#define c_read(addr) ((char*)(addr))
#define c_write(addr, val) *(char*)(addr)=val

void main(int argc, char** argv)
{
    char *c_map;
    int fd = open(argv[1], O_CREAT|O_RDWR|O_TRUNC, 0777);
    if (fd < 0) {
        return;
    }
    ftruncate(fd, SIZE);
    c_map=(char*)mmap(NULL, 2*SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    c_write(c_map, 'k');
    printf("%s\n", c_read(c_map));
    munmap(c_map, 2*SIZE);
    close(fd);
}


