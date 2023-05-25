// @File		TM1637.H	 		
// @Author		JOSIMAR PEREIRA LEITE
// @country		BRAZIL
// @Date		25/05/23
//
//
// Copyright (C) 2023 JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
#ifndef TM1637_H
#define TM1637_H

#define TM1637_DIO PORTAbits.RA0
#define TM1637_SCK PORTAbits.RA1

#define TM1637_DIO_TRIS TRISAbits.TRISA0
#define TM1637_SCK_TRIS TRISAbits.TRISA1

#define TM1637_COMMAND2 0x40
#define TM1637_COMMAND3 0xC0
#define TM1637_COMMAND4 0x80

// COMMAND2 BYTE
#define TM1637_COMMAND2_WRITE_DATA 0x00
#define TM1637_COMMAND2_READ_KEY 0x02
#define TM1637_COMMAND2_FIXED_ADDRESS 0x04
#define TM1637_COMMAND2_MODE_NORMAL 0x00
#define TM1637_COMMAND2_MODE_TEST 0x01

// COMMAND3 BYTE
#define TM1637_COMMAND3_ADDRESS0 0x00
#define TM1637_COMMAND3_ADDRESS1 0x01
#define TM1637_COMMAND3_ADDRESS2 0x02
#define TM1637_COMMAND3_ADDRESS3 0x03

// COMMAND4 BYTE
#define TM1637_COMMAND4_PULSE_1_16 0x00
#define TM1637_COMMAND4_PULSE_2_16 0x01
#define TM1637_COMMAND4_PULSE_4_16 0x02
#define TM1637_COMMAND4_PULSE_10_16 0x03
#define TM1637_COMMAND4_PULSE_11_16 0x04
#define TM1637_COMMAND4_PULSE_12_16 0x05
#define TM1637_COMMAND4_PULSE_13_16 0x06
#define TM1637_COMMAND4_PULSE_14_16 0x07
#define TM1637_COMMAND4_DISPLAY_ON 0x08

void TM1637_start(void)
{
    TM1637_SCK = 1;
    __asm__ __volatile__("nop");
    TM1637_DIO = 1;
    __asm__ __volatile__("nop");
    TM1637_DIO = 0;    
    __asm__ __volatile__("nop");
    TM1637_SCK = 0;
}

void TM1637_stop(void)
{
    TM1637_SCK = 0;
    __asm__ __volatile__("nop");
    TM1637_DIO = 0;
    __asm__ __volatile__("nop");
    TM1637_SCK = 1;
    __asm__ __volatile__("nop");
    TM1637_DIO = 1;
}

void TM1637_write(unsigned char data)
{
    for (unsigned char mask = 0x01; mask; mask <<= 1)
    {        
        if(data & mask) TM1637_DIO = 1; else TM1637_DIO = 0;

        TM1637_SCK = 1;
        __asm__ __volatile__("nop");
        TM1637_SCK = 0;
        __asm__ __volatile__("nop");
    }

    TM1637_DIO = 0;
    __asm__ __volatile__("nop");
    TM1637_SCK = 1;
    __asm__ __volatile__("nop");    
    TM1637_SCK = 0;
}

void TM1637_command(unsigned char data)
{
    TM1637_start();
    TM1637_write(data);
    TM1637_stop();    
}

void TM1637_data(unsigned char address, unsigned char data)
{
    TM1637_command(TM1637_COMMAND2|TM1637_COMMAND2_FIXED_ADDRESS); 

    TM1637_start();
    TM1637_write(TM1637_COMMAND3 | address);
    TM1637_write(data);
    TM1637_stop();    
}

unsigned char TM1637_read(void)
{
    unsigned char data = 0x00;
    
    TM1637_DIO = 1;
    
    TM1637_DIO_TRIS = 1;
    
    for (unsigned char mask = 0x80; mask; mask >>= 1)
    {
        if(TM1637_DIO) data |= mask;
        
        TM1637_SCK = 1;
        __asm__ __volatile__("nop");
        TM1637_SCK = 0;
        __asm__ __volatile__("nop");
    }

    TM1637_DIO_TRIS = 0;
    
    TM1637_DIO = 0;
    __asm__ __volatile__("nop");
    TM1637_SCK = 1;
    __asm__ __volatile__("nop");
    TM1637_SCK = 0;
    
    return ( data );
}

unsigned char TM1637_key()
{
    unsigned char key = 0;
    
    TM1637_start();        
    TM1637_write(TM1637_COMMAND2|TM1637_COMMAND2_READ_KEY);
    key |= TM1637_read();
    TM1637_stop();                
    
    return key;
}

void TM1637_init(void)
{
    TM1637_SCK_TRIS = 0;
    TM1637_DIO_TRIS = 0;
   
    TM1637_command(TM1637_COMMAND4|TM1637_COMMAND4_DISPLAY_ON|TM1637_COMMAND4_PULSE_14_16);   
}

#endif
