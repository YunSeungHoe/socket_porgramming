#include <stdio.h>
#include <pigpio.h>

void chainge_led(int r, int y, int g){
    gpioWrite(r, 1);
    gpioWrite(g, 0);
    gpioDelay(1000000);
    gpioWrite(r, 0);
    gpioWrite(y, 1);
    gpioDelay(1000000);
    gpioWrite(y, 0);
    gpioWrite(g, 1);
    gpioDelay(1000000);
}

int main(){
    int red = 18;
    int yellow = 23;
    int green = 25;
    gpioInitialise();

    gpioSetMode(red, PI_OUTPUT);
    gpioSetMode(yellow, PI_OUTPUT);
    gpioSetMode(green, PI_OUTPUT);
    while(1)
        chainge_led(red, yellow, green);
    // 값을 0으로 바꿔주는 용도 
    // gpioWrite(red, 0);
    // gpioWrite(yellow, 0);
    // gpioWrite(green, 0);
    gpioTerminate();
    return 0;
}