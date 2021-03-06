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
#include <stdint.h>
#include <string.h>
#include "../pru_tables.h"

/*ARM-A8 PRU memory Page 184 am335x TRM
 * For AM33XX_DATARAMx_PHYS_BASE allow for compiler stack which start at 000 and works up
 *RAM0 & RAM1 are 8Kbytes
 * SHAREDRAM is 12kbytes - shared both PRU
*/
#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000
#define SHARED_RAM_SZ sizeof(struct channels_s)  /* grab API size */

volatile uint16_t *shared_dataram;

/* Define an API to be calleable from JS - not implemented yet */

void bbbio_ppm_api(struct channels_s *channels){
   memcpy((void *)shared_dataram,(void *)channels,SHARED_RAM_SZ);
}

void bbbio_ppm_init() {
   /* Allocate shared memory pointer to PRU0 DATARAM */
	int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
	volatile void *shared_dataram_init = mmap(NULL,
	   SHARED_RAM_SZ,	
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		mem_dev,
		AM33XX_PRUSS_SHAREDRAM_BASE
	);
	
	shared_dataram = (uint16_t *) shared_dataram_init;
}

void bbbio_ppm_cleanup() {
	munmap((void *)shared_dataram,SHARED_RAM_SZ); //njh guess at releasing it.
}

#ifdef MAIN_LOCAL
void main() {
	unsigned int sec_i, chnl_j;
	unsigned int offset;
   struct channels_s  channels;
   volatile uint16_t *shared_dataram2;

   bbbio_ppm_init();
	
   uint16_t ppm =60;//actually 
    
	offset = 0;
	channels.chn[offset].sr =ppm;
	printf("shared_datara2 = %p, offset=%d At init Read:", shared_dataram,offset);
   shared_dataram2 = (uint16_t *) shared_dataram;
	for(chnl_j=0; chnl_j<8; chnl_j++) {
		printf(" %04d",  *shared_dataram2);
	   shared_dataram2++;
	   if (0x3 ==(chnl_j & 0x3)){printf("   ");}
   }
   printf("\n");
    
   ppm =0;
	for(chnl_j=0; chnl_j<8; chnl_j++) {
      channels.chn[chnl_j].sr=ppm;
      ppm += 60;
    }
    //Fut - wait for ack from PRU and then init shared ram

    //while (1) 
    {
	    ppm=60;
	    for(sec_i=0; sec_i<8; sec_i++) {
	       channels.chn[offset].sr +=ppm;
			 bbbio_ppm_api(&channels);
		    printf("Writing %04d Read:", ppm);
		    shared_dataram2 = (uint16_t *) shared_dataram;
		    for(chnl_j=0; chnl_j<8; chnl_j++) {
		       printf(" %04d",  *shared_dataram2);
		       shared_dataram2++;
		       if (0x3 ==(chnl_j & 0x3)){printf("   ");}
         }
         printf("\n");
         sleep(1);
	    }
	}
	bbbio_ppm_cleanup();
}

#endif //MAIN_LOCAL
