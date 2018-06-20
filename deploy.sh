#! /bin/bash

##############################################################################
#
# Copyright (C) 2016 Zubeen Tolani <ZeekHuge - zeekhuge@gmail.com>
#
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#	* Redistributions of source code must retain the above copyright
#	  notice, this list of conditions and the following disclaimer.
#
#	* Redistributions in binary form must reproduce the above copyright
#	  notice, this list of conditions and the following disclaimer in the
#	  documentation and/or other materials provided with the
#	  distribution
#
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#############################################################################


# The script builds the pru_blinky project and configures the pinmuxing for $HEADER$PIN_NUM

#If changing these variables, make sure the given pin can be muxed to the given pru.  

#PRU_CORE should be either 0 or 1
PRU_CORE=0
PIN_NUMBER1=P9_25
PIN_NUMBER2=P9_27
PIN_NUMBER3=P9_28
PIN_NUMBER4=P9_29
PIN_NUMBER5=P9_30
PIN_NUMBER6=P9_31
PIN_NUMBER7=P8_11
PIN_NUMBER8=P8_12
#PRU_CORE should be either 0 or 1
PRU_CORE=0

if [ $CONFIGURE_PINS eq 0 ]
   then
	CONFIGURE_PINS=1
	echo "-Configuring pinmux"
		config-pin -a $PIN_NUMBER1 pruout
		config-pin -q $PIN_NUMBER1
		config-pin -a $PIN_NUMBER2 pruout
		config-pin -q $PIN_NUMBER2
		config-pin -a $PIN_NUMBER3 pruout
		config-pin -q $PIN_NUMBER3
		config-pin -a $PIN_NUMBER4 pruout
		config-pin -q $PIN_NUMBER4
		config-pin -a $PIN_NUMBER5 pruout
		config-pin -q $PIN_NUMBER5
		config-pin -a $PIN_NUMBER6 pruout
		config-pin -q $PIN_NUMBER6
		config-pin -a $PIN_NUMBER7 pruout
		config-pin -q $PIN_NUMBER7
		config-pin -a $PIN_NUMBER8 pruout
		config-pin -q $PIN_NUMBER8
fi

echo "-Building project"
	cd pru
	sudo make clean
	make
	sudo make run0

cd ../rpmsg
sudo sh ./deploy.sh

echo "Done. Ppm pulse on pin"

