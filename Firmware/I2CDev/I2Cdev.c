// I2Cdev library collection - Main I2C device class header file
// Abstracts bit and byte I2C R/W functions into a convenient class
// 6/9/2012 by Jeff Rowberg <jeff@rowberg.net>
// 03/28/2017 by Kamnev Yuriy <kamnev.u1969@gmail.com>
//
// Changelog:
//     2017-03-28 - ported to STM32 using Keil MDK Pack

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg
Copyright (c) 2017 Kamnev Yuriy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/


#include "I2Cdev.h"
#include "Basile.h"
#include <stm32l4xx_hal.h>

extern I2C_HandleTypeDef hi2c1;
#define I &hi2c1

/** Read several byte from an 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to read from
 * @param len 		How many bytes to read
 * @param data 		Buffer to save data into
 * @return Status of read operation (0 = success, <0 = error)
 */
int8_t I2Cdev_readBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, uint8_t *data) {
   return -HAL_I2C_Mem_Read(I, dev_addr << 1, reg_addr, 1, data, len, 1000);
}


/** Read a single byte from a 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register reg_addr to read from
 * @param data 		Buffer to save data into
 * @return Status of read operation (0 = success, <0 = error)
 */
int8_t 	I2Cdev_readByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
   return -HAL_I2C_Mem_Read(I, dev_addr << 1, reg_addr, 1, data, 1, 1000);
}

/** Read a single bit from a 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to read from
 * @param bitn 		Bit position to read (0-15)
 * @param data 		Container for single bit value
 * @return Status of read operation (0 = success, <0 = error)
 */
int8_t I2Cdev_readBit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bitn, uint8_t *data) {
	int8_t err;

	err = I2Cdev_readByte(dev_addr, reg_addr, data);
	*data = (*data >> bitn) & 0x01;

	return err;
}


/** Read several bits from a 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to read from
 * @param start_bit First bit position to read (0-7)
 * @param len		Number of bits to read (<= 8)
 * @param data 		Container for right-aligned value
 * @return Status of read operation (0 = success, <0 = error)
 */
int8_t I2Cdev_readBits(uint8_t dev_addr, uint8_t reg_addr, uint8_t start_bit, 
		uint8_t len, uint8_t *data) 
{
	int8_t err;

	uint8_t b;
	if ((err = I2Cdev_readByte(dev_addr, reg_addr, &b)) == 0) {
		uint8_t mask = ((1 << len) - 1) << (start_bit - len + 1);
		b &= mask;
		b >>= (start_bit - len + 1);
		*data = b;
	}

	return err;
}


/** Read a single bit from a 16-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to read from
 * @param bit_n 	Bit position to read (0-15)
 * @param data 		Container for single bit value
 * @return Status of read operation (true = success)
 */
int8_t 	I2Cdev_readBitW(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_n, uint16_t *data) {
	int8_t err;

	err = I2Cdev_readWord(dev_addr, reg_addr, data);
	*data = (*data >> bit_n) & 0x01;

	return err;
}


/** Read several bits from a 16-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to read from
 * @param start_bit First bit position to read (0-15)
 * @param len		Number of bits to read (<= 16)
 * @param data 		Container for right-aligned value
 * @return Status of read operation (0 = success, <0 = error)
 */
int8_t I2Cdev_readBitsW(uint8_t dev_addr, uint8_t reg_addr, uint8_t start_bit,
		uint8_t len, uint16_t *data)
{
    int8_t err;
    uint16_t w;

    if ((err = I2Cdev_readWord(dev_addr, reg_addr, &w)) == 0) {
        uint16_t mask = ((1 << len) - 1) << (start_bit - len + 1);
        w &= mask;
        w >>= (start_bit - len + 1);
        *data = w;
    }

    return err;
}


/** Write multiple bytes to an 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	First register address to write to
 * @param len 		Number of bytes to write
 * @param data 		Buffer to copy new data from
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t 	I2Cdev_writeBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, uint8_t *data) {
	return -HAL_I2C_Mem_Write(I, dev_addr << 1, reg_addr, 1, data, len, 100);
}


/** Write single byte to an 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register address to write to
 * @param data 		New byte value to write
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t 	I2Cdev_writeByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
	return -HAL_I2C_Mem_Write(I, dev_addr << 1, reg_addr, 1, &data, 1, 100);
}

/** write a single bit in an 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to write to
 * @param bit_n 	Bit position to write (0-7)
 * @param data 		New bit value to write
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t 	I2Cdev_writeBit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_n, uint8_t data) {
	uint8_t b;
	int8_t err;

	err = I2Cdev_readByte(dev_addr, reg_addr, &b);
	if(err < 0) {
		return err;
	}

	b = (data != 0) ? (b | (1<<bit_n)) : (b &= ~(1<<bit_n));

	return I2Cdev_writeByte(dev_addr, reg_addr, b);
}


/** write a single bit in a 16-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to write to
 * @param bit_n 	Bit position to write (0-15)
 * @param data 		New bit value to write
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t 	I2Cdev_writeBitW(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_n, uint16_t data) {
	uint16_t w;
	I2Cdev_readWord(dev_addr, reg_addr, &w);

	w = (data != 0) ? (w | (1<<bit_n)) : (w &= ~(1<<bit_n));

	return I2Cdev_writeWord(dev_addr, reg_addr, w);
}


/** Write multiple bits in an 8-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to write to
 * @param start_bit First bit position to write (0-7)
 * @param len 		Number of bits to write (not more than 8)
 * @param data 		Right-aligned value to write
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t I2Cdev_writeBits(uint8_t dev_addr, uint8_t reg_addr, uint8_t start_bit,
		uint8_t len, uint8_t data)
{
    uint8_t b;
    int8_t err;

    if ((err = I2Cdev_readByte(dev_addr, reg_addr, &b)) == 0) {
        uint8_t mask = ((1 << len) - 1) << (start_bit - len + 1);
        data <<= (start_bit - len + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte

        return I2Cdev_writeByte(dev_addr, reg_addr, b);
    }
    else {
        return err;
    }
}


/** Write multiple bits in a 16-bit device register.
 * @param dev_addr 	I2C slave device address
 * @param reg_addr 	Register regAddr to write to
 * @param start_bit First bit position to write (0-15)
 * @param len 		Number of bits to write (not more than 16)
 * @param data 		Right-aligned value to write
 * @return Status of operation (0 = success, <0 = error)
 */
int8_t I2Cdev_writeBitsW(uint8_t dev_addr, uint8_t reg_addr, uint8_t start_bit,
		uint8_t len, uint16_t data)
{
    uint16_t w;
    int8_t err;

    if ((err = I2Cdev_readWord(dev_addr, reg_addr, &w)) != 0) {
        uint16_t mask = ((1 << len) - 1) << (start_bit - len + 1);
        data <<= (start_bit - len + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return I2Cdev_writeWord(dev_addr, reg_addr, w);
    }
    else {
        return err;
    }
}

