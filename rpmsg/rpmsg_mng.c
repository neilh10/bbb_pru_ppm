/*
 * Copyright (C) 2018 Jason Kridner <jdk@ti.com>, Texas Instruments Incorporated
 *
 * Concept based on example code from:
 * http://credentiality2.blogspot.com/2015/09/beaglebone-pru-ddr-memory-access.html
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

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
//typedef unsigned int uint32_t;
#include <stdint.h>
#include <string.h>
#include "../pru_tables.h"

#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000

void main() {
	unsigned int iii, jjj;
	unsigned int offset;
    struct channels_s  channels;
#define COMMS_PORT_SIZE 64
	/* Allocate shared memory pointer to PRU0 DATARAM */
	int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
	volatile void *shared_dataram = mmap(NULL,
	   COMMS_PORT_SIZE,	/* grab 8*4 bytes or 8 words */
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		mem_dev,
		AM33XX_PRUSS_SHAREDRAM_BASE
	);
	volatile uint32_t *shared_dataram2;

	
    uint32_t pps_delay_ms =200;
    
	offset = 1;
	channels.chn[offset].sr_uint32 =pps_delay_ms;
	printf("shared_dataram = %p, offset=%d Read:", shared_dataram,offset);
    shared_dataram2 = (uint32_t *) shared_dataram;
		    for(jjj=0; jjj<8; jjj++) {
		    	printf(" %04d",  *shared_dataram2);
		    	shared_dataram2++;
		    	if (0x3 ==(jjj & 0x3)){printf("   ");}
            }
            printf("\n");
	//for(jjj=0; jjj<8; jjj++) {
    //   channels.chn[jjj].sr_uint32=0;
    //}
    //memset((void *)&channels,0,sizeof(channels));
   
    //while (1) 
    {
	    for(iii=0; iii<8; iii++) {
	    	pps_delay_ms+=100;

		    channels.chn[offset].sr_uint32 =pps_delay_ms;
		    memcpy((void *)shared_dataram,(void *)&channels,sizeof(channels));
			
		    printf("Writing %04d Read:", pps_delay_ms);
		    shared_dataram2 = (uint32_t *) shared_dataram;
		    for(jjj=0; jjj<8; jjj++) {
		    	printf(" %04d",  *shared_dataram2);
		    	shared_dataram2++;
		    	if (0x3 ==(jjj & 0x3)){printf("   ");}
            }
            printf("\n");
	//printf("Read 0x%08x\n", j);
		    //usleep(1000);
		    sleep(1);
		    //j = *(unsigned int *)shared_dataram;
		    //printf("Read 0x%08x (0x%08x)\n", j, j^0xAAAAAAAA);
	    }
	}
	munmap((void *)shared_dataram,COMMS_PORT_SIZE); //njh guess at releasing it.
}