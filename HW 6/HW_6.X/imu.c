#include "imu.h"
#include "i2c_master_noint.h"

void imu_setup(){
    
    unsigned char who = 0;

    // read from IMU_WHOAMI
    
    i2c_master_start();
    i2c_master_send(0b11010110);
    i2c_master_send(0x0F);
    i2c_master_restart();
    i2c_master_send(0b11010111);
    who = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
    if (who != 0b01101001)
    {
        while(1){;}
    }

    // init IMU_CTRL1_XL
    
    i2c_master_start();
    i2c_master_send(0b11010110);
    i2c_master_send(0x10);
    i2c_master_send(0b10000010);
    i2c_master_stop();
    
    // init IMU_CTRL2_G
    
    i2c_master_start();
    i2c_master_send(0b11010110);
    i2c_master_send(0x11);
    i2c_master_send(0b10001000);
    i2c_master_stop();

    // init IMU_CTRL3_C
    
    i2c_master_start();
    i2c_master_send(0b11010110);
    i2c_master_send(0x12);
    i2c_master_send(0b00000100);
    i2c_master_stop();

}

void imu_read(unsigned char reg, signed short * data, int len){
    
    // read multiple from the imu, each data takes 2 reads so you need len*2 chars
    
    unsigned char da[len*2];

    I2C_read_multiple(0b11010110, reg, da, len*2);
    
    // turn the chars into the shorts
    
    int j = 0;
    for(j;j<len;j++){
        data[j] = (da[2*j + 1]) << 8 | da[2*j];
    }
    
}