#include <stdio.h>
#include <pigpio.h>

int main(){
    int pin = 18;
    int switch_pin = 24;
    int flag = 0;

    gpioInitialise();

    gpioSetMode(pin, PI_OUTPUT);
    gpioSetMode(switch_pin, PI_INPUT);

    while(1){
        if(gpioRead(switch_pin) == 0){
            if(flag) flag = 0;
            else flag = 1;
            gpioWrite(pin, flag);
            gpioDelay(500000);
        }
    }
    gpioTerminate();
    return 0;
}