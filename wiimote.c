#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Gyro-tester branch
// I2C and pin setup
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

// Left mouse button (or any input pin)
#define LMB 15

// MPU-6050 I2C address (AD0 low by default)
#define MPU6050_ADDR 0x68

// MPU-6050 register map
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG_REG   0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define WHO_AM_I     0x75

void gyro_initialize();
void gyro_test_connection();

int main() {
    stdio_init_all();
    gyro_initialize();
    gyro_test_connection();

    gpio_init(LMB);
    gpio_set_dir(LMB, GPIO_IN);
    gpio_pull_up(LMB);

    while (true) {
        if (!gpio_get(LMB)) {
            printf("Hello!\n");
            sleep_ms(200);  // simple debounce
        }
    }
}

void gyro_initialize() {
    // Initialize I2C at 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    uint8_t buf[2];

    // Wake up the MPU-6050 (exit sleep mode)
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;  // Clear sleep bit
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    // Set sample rate divider to 7 -> 1kHz/(1+7) = 125Hz
    buf[0] = SMPLRT_DIV;
    buf[1] = 0x07;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    // Set DLPF config (low-pass filter)
    buf[0] = CONFIG_REG;
    buf[1] = 0x06;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    // Set gyro full scale ±2000°/s
    buf[0] = GYRO_CONFIG;
    buf[1] = 0x18;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    // Set accel full scale ±2g
    buf[0] = ACCEL_CONFIG;
    buf[1] = 0x00;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    sleep_ms(100);  // allow time to stabilize
}

// Optional — verify that MPU-6050 is connected correctly
void gyro_test_connection() {
    uint8_t reg = WHO_AM_I;
    uint8_t who = 0;

    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, &who, 1, false);

    if (who == 0x68) {
        printf("MPU-6050 detected! (WHO_AM_I = 0x%02X)\n", who);
    } else {
        printf("MPU-6050 not detected! (WHO_AM_I = 0x%02X)\n", who);
    }
}
