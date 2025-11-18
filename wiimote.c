#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define LMB 15

void gyro_initialize();

int main()
{
    stdio_init_all();

    gyro_initialize();

    gpio_init(LMB);
    gpio_set_dir(LMB, 0);
    gpio_pull_up(LMB);


    while (true) {
        if (!gpio_get(LMB)){
            printf("Hello!");
        }
    }
}

void gyro_initialize(){
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
}