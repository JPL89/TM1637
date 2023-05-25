// @File		MAIN.C	 		
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
#include "utils.h"
#include "tm1637.h"

static const unsigned char font[] =
{
    0x3f, // 0
    0x06, // 1
    0x5b, // 2
    0x4f, // 3
    0x66, // 4
    0x6d, // 5
    0x7d, // 6
    0x27, // 7
    0x7f, // 8
    0x6f, // 9
};

int main(void) 
{
    __delay_ms(500);        
    
    PORTA = 0x00;
    PORTB = 0x00;    
    
    CMCON = 0x07;

    unsigned char key = 0;
    
    TM1637_init();
    
    while(1)
    {
        key = TM1637_key();
        
        if(key == 255) key = 0;
        
        TM1637_data(3, font[ (key/1)%10 ]);
        TM1637_data(2, font[ (key/10)%10 ]);
        TM1637_data(1, font[ (key/100)%10 ]);
        TM1637_data(0, font[ (key/1000)%10 ]);
    }
}
