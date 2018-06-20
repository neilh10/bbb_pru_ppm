/*
 * Source Modified by Zubeen Tolani < ZeekHuge - zeekhuge@gmail.com >
 * Based on the examples distributed by TI
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
				  
#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include <string.h>
#include "resource_table_empty.h"
#include "../pru_tables.h"

#define	INS_PER_US 200           // 5ns per instruction
#define INS_PER_DELAY_LOOP 2	 // two instructions per delay loop
#define DELAY_CYCLES_US (INS_PER_US / INS_PER_DELAY_LOOP)

#define GPIO1 0x4804C000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define USR0 (1<<21)
#define USR1 (1<<22)
#define USR2 (1<<23)
#define USR3 (1<<24)
unsigned int volatile * const GPIO1_CLEAR = (unsigned int *) (GPIO1 + GPIO_CLEARDATAOUT);
unsigned int volatile * const GPIO1_SET = (unsigned int *) (GPIO1 + GPIO_SETDATAOUT);

volatile register unsigned int __R30;
volatile register unsigned int __R31;

#define GPIO07 7 //P9_25 r31_07  GPIO3_21 
#define GPIO05 5 //P9_27 r31_05 gpio3_19
#define GPIO03 3 //P9_28 r31_03 gpio3_17
#define GPIO01 1 //P9_29 r31_01
#define GPIO02 2 //P9_30 r31_02 gpio3_16
#define GPIO00 0 //P9_31 r31_00

//P9_41 r31_6
//P9_42 r31_4


#define PRU0_GPIO ((1<<GPIO07) |(1<<GPIO05) |(1<<GPIO03) | (1<<GPIO01) | (1<<GPIO02)|(1<<GPIO00) ) 

#define CYCLES_500mS 100000000
#define CYCLES_5mS     1000000
#define CYCLES_1mS      200000
#define CYCLES_0_5mS    100000

#define DELAY_CYCLES_FREQ CYCLES_500mS
#define DELAY_CYCLES_PULSE_ON CYCLES_1mS

#define SHARED_RAM_ADDRESS 0x10000
uint32_t volatile __far * const SHARED_RAM = (uint32_t *) (SHARED_RAM_ADDRESS);
//struct channels_s volatile __far * const channels = (struct channels_s *) &SHARED_RAM;
 struct channels_s  channels;
    
void main(void) {
	int i=7;
	int j;
	int offset;
    volatile uint32_t gpo; 
    volatile uint32_t value_ms = 100;


	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	
	//*SHARED_RAM = 500;//inital value till changed
	//channels.sr[0].ppm = 500;//inital value till changed
	*SHARED_RAM = (int) 102;  //Treat as simple version on startup
	offset =1; //
	channels.chn[offset].sr_uint32 = 200;//inital value till changed
	channels.chn[offset+1].sr_uint32 = 250;
	
	//for(i = 1000000; i > 0; i = (i * decay) / 100) {
	 while(1){ 
	 	//for(i = 0; i < 8; i++) 
	 	{
#ifndef LED_DISABLE
			*GPIO1_SET = USR0;
#endif
			//__R30 |= PRU0_GPIO; //Set
			//value = *SHARED_RAM.sr[0].ppm;
			//channels.chn[0].sr_uint32 = *SHARED_RAM;
			memcpy((void *)&channels,(void *)SHARED_RAM,sizeof(channels));
			value_ms = channels.chn[offset].sr_uint32;			

			__R30 |= (1<<i); //Set

			 __delay_cycles(DELAY_CYCLES_PULSE_ON);
#ifndef LED_DISABLE
			*GPIO1_CLEAR = USR0;
#endif

			//__R30 ^= PRU0_GPIO;
			__R30 = 0;
		
			for(j=0;j<value_ms;j++){ __delay_cycles(CYCLES_1mS); }
			//__delay_cycles( CYCLES_1mS*value);
	 	}
	}

	//__halt();
}
