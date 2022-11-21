#include <stdio.h>
#include <pigpio.h>

void chainge_led(int g, int y, int r){
    gpioWrite(g, 1);
    gpioWrite(y, 0);
    gpioWrite(r, 0);
    gpioDelay(1000000);
    gpioWrite(g, 0);
    gpioWrite(y, 1);
    gpioWrite(r, 0);
    gpioDelay(1000000);
    gpioWrite(g, 0);
    gpioWrite(y, 0);
    gpioWrite(r, 1);
    gpioDelay(1000000);
}

int main(){
    int green = 18;
    int yellow = 23;
    int red = 25;
    gpioInitialise();

    gpioSetMode(green, PI_OUTPUT);
    gpioSetMode(yellow, PI_OUTPUT);
    gpioSetMode(red, PI_OUTPUT);
    while(1)
        chainge_led(green, yellow, red);
    gpioTerminate();
    return 0;
}