#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 64

int main(void){
    int fd = open("/dev/mydev",O_RDONLY);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    // OPEN
    if(fd < 0){
        perror("Error opening file");
        return -1;
    }
    // READ
    // It returns 0 when it reaches the End of File (EOF)
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE-1)) > 0){
        buffer[bytes_read]='\0';
        printf("%s", buffer); 
    }
   
    if (bytes_read == -1) {
        perror("Error reading file");
    }
    // CLOSE
    if (close(fd) < 0) {
        perror("Error closing file");
        return -1;
    }
    return 0;
}