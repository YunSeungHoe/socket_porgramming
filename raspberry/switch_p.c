#include <stdio.h>
#include <pigpio.h>

int main(){
    int pin = 18;
    int led_pin = 23;
    int switch_pin = 25;
    int flag = 0;
    gpioInitialise();
    gpioSetMode(pin, PI_OUTPUT);
    gpioSetMode(led_pin, PI_OUTPUT);
    gpioSetMode(switch_pin, PI_INPUT);

    gpioWrite(pin, 1);
    while(1){
        printf("%d\n", flag);
        if(gpioRead(switch_pin) == 1){
            if(flag)
                flag = 0;
            else
                flag = 1;
            gpioDelay(500000);
        }
        gpioWrite(led_pin, flag);
    }
    gpioTerminate();
    return 0;
}