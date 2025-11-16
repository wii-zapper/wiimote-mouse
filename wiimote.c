#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>
#include "hardware/gpio.h"


/*
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
    I2C_write(0x68, 0x6B, 0x00);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
}
*/

/* 
int main() {
    stdio_init_all();
    sleep_ms(2000);  // wait for USB connection

    while (true) {
        printf("Pico is alive!\n");
        sleep_ms(1000);
    }
}
*/

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43
#define I2C_SDA 11
#define I2C_SCL 12

void mpu6050_reset() {
    uint8_t  buf[2];
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;
    i2c_write_blocking(i2c0, MPU6050_ADDR, buf, 2, false);
}

int16_t  read_raw_data(uint8_t  reg) {
    uint8_t  data[2];
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, data, 2, false);
    return (data[0] << 8 | data[1]);
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    i2c_init(i2c0, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    sleep_ms(500);
    mpu6050_reset(); //Something about this line causes the code to crash
    sleep_ms(100);

    while (true) {
        printf("The while true is working");
        
        int16_t  accel_x = read_raw_data(ACCEL_XOUT_H); //I think these 7 lines also cause code to crash
        int16_t  accel_y = read_raw_data(ACCEL_XOUT_H + 2);
        int16_t  accel_z = read_raw_data(ACCEL_XOUT_H + 4);

        int16_t  gyro_x = read_raw_data(GYRO_XOUT_H);
        int16_t  gyro_y = read_raw_data(GYRO_XOUT_H + 2);
        int16_t  gyro_z = read_raw_data(GYRO_XOUT_H + 4);


        printf("ACCEL: X=%d  Y=%d  Z=%d | GYRO: X=%d  Y=%d  Z=%d\n",
               accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z);

        sleep_ms(200);
    }
}
