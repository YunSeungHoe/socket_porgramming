#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int ledControl(int gpio){
    printf("111\n");
    int fd;
    char buf[BUFSIZ];
    fd = open("/sys/class/gpio/export", O_WRONLY);

    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);
    printf("111\n");
    sprintf(buf, "/sys/class/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    write(fd, "out", 3);
    close(fd);
    printf("222\n");

    sprintf(buf, "/sys/class/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);
    write(fd, "1", 1);
    close(fd);

    getchar();
    printf("/sys/class/gpio%d/value", gpio);

    fd = open("/sys/class/gpio/gpio%d/value", O_WRONLY);
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}

int main(int argc, char** argv[]){
    int gno;
    if(argc < 2){
        printf("input error\n");
        return -1;
    }
    gno = atoi(argv[1]);
    printf("input %d\n", gno);
    printf("111\n");
    ledControl(gno);
    return 0;
}

