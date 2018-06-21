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
#define GPIO15 15 //P8_11 r30_15 GPIO1_13
#define GPIO14 14 //P8_12 r30_14 GPIO1_12

#define CUST_OUT1  GPIO07
//Output Channel mapping increasing with board Pin num. P9_25..31 then P8_11..12
const char flowpin_out[8] = {GPIO07,GPIO05,GPIO03,GPIO01,GPIO02,GPIO00,GPIO15,GPIO14};

#define PRU0_GPIO ((1<<GPIO07) |(1<<GPIO05) |(1<<GPIO03) | (1<<GPIO01) | (1<<GPIO02)|(1<<GPIO00) ) 

#define CYCLES_500mS 100000000
#define CYCLES_5mS     1000000
#define CYCLES_1mS      200000
#define CYCLES_0_5mS    100000

#define DELAY_CYCLES_FREQ CYCLES_500mS
#define DELAY_CYCLES_PULSE_ON CYCLES_1mS
#define PULSE_WIDTH_MS 4  //Actually 5msin downcount

#define SECS_IN_MINUTE 60
#define SHARED_RAM_ADDRESS 0x10000
uint32_t volatile __far * const SHARED_RAM = (uint32_t *) (SHARED_RAM_ADDRESS);
//struct channels_s volatile __far * const channels = (struct channels_s *) &SHARED_RAM;
struct channels_s  channels;
    
struct port_pulses_s {
	uint16_t ppm;  //Requested Pulses Per Min
	uint16_t pps;  // Calc Pulses per Sec.
	uint16_t channel_reload_ms; //0-60*1000 for every second
	uint16_t channel_cnt_ms; //On reaching channel_reload_ms, resets channel counter to 0 and activates bit
	uint16_t pulse_cnt_ms; //On reload set count width - when !0 a pulse out, when 0 no pulse
};
struct port_pulses_s port_pulses[8];

void main(void) {
	//int i=7;
	int sec_i,chnl_i;
	int ms_i;
	int jjj;
	int offset;
	int32_t out_R30;
   volatile uint32_t gpo; 
   volatile uint32_t value_ms = 100;
   struct port_pulses_s *pp;

	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	
	//*SHARED_RAM = 500;//inital value till changed
	//channels.sr[0].ppm = 500;//inital value till changed
	*SHARED_RAM = (int) 104;  //Treat as simple version on startup
	offset =400; //
	for(jjj=1; jjj<8; jjj++) {
		channels.chn[jjj].sr = offset;//inital value till changed
		offset+=50;
    }
    /* Channel[out].sr has ppm
       if >60 - then pps = ppm/60
       For 1second processing period_ms[out] = 1000/pps
       Need up counter per channel
    */    

	while(1){ 
	   //for(sec_i = 0; sec_i <60; sec_i++) //Represents 60Secs passing
	   {
	      //every sec
         memcpy((void *)&channels,(void *)SHARED_RAM,sizeof(channels));
         for(chnl_i = 0; chnl_i <CHNL_TOT; chnl_i++) {
         	pp = &port_pulses[chnl_i];  
		      port_pulses[chnl_i].ppm = channels.chn[chnl_i].sr;			
		      port_pulses[chnl_i].pps = port_pulses[chnl_i].ppm/SECS_IN_MINUTE;
		      port_pulses[chnl_i].channel_reload_ms = 1000/port_pulses[chnl_i].pps; //period(ms) for pulse in 1000mS
		      port_pulses[chnl_i].channel_cnt_ms =port_pulses[chnl_i].channel_reload_ms;
		      port_pulses[chnl_i].pulse_cnt_ms = PULSE_WIDTH_MS+1;//Compensate for 1st pass
         }
         for(ms_i = 0; ms_i <999; ms_i++) {  //mS loop - 
            out_R30 = 0; //Start 0 and construct pulses by setting bit
            for(chnl_i = 0; chnl_i <CHNL_TOT; chnl_i++)  {
            	/* loop through every channel to see if timer has expired 
            	 * and time for pulse activation */
               if ((0==port_pulses[chnl_i].channel_cnt_ms)||(port_pulses[chnl_i].pulse_cnt_ms)) { 
		            out_R30 |= (1<<flowpin_out[chnl_i]); //Set pulse bit
		            if (0!=port_pulses[chnl_i].pulse_cnt_ms) {port_pulses[chnl_i].pulse_cnt_ms--;}
		            if (0==port_pulses[chnl_i].channel_cnt_ms) {   
			            port_pulses[chnl_i].channel_cnt_ms= port_pulses[chnl_i].channel_reload_ms;
			            port_pulses[chnl_i].pulse_cnt_ms = PULSE_WIDTH_MS;
		            }
               }
               port_pulses[chnl_i].channel_cnt_ms--;
            }
            __R30 = out_R30; //Set Out port for pulse conditions
            //__delay_cycles(CYCLES_1mS-2200);//Creates exactly 1ms loop timing
            __delay_cycles(CYCLES_1mS-2100);
           }
	 	}
	}

	__halt();
}
