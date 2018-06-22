#! /bin/bash

# The script builds the pru_blinky project and configures the pinmuxing for $HEADER$PIN_NUM

#If changing these variables, make sure the given pin can be muxed to the given pru.  

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

echo "*******************************************************"
echo "This must be compiled on the BEAGLEBONE BLACK itself"
echo "It was tested on 4.9.78-ti-r94 kernel version" 
#read

echo "-Building project"
	#cd PRU_gpioToggle
	make clean
	make

echo "-Placing the firmware"
	cp gen/*.out /lib/firmware/am335x-pru$PRU_CORE-fw

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
	
echo "-Rebooting"
	if [ $PRU_CORE -eq 0 ]
	then
		echo "Rebooting pru-core 0"
	    echo "stop" >  /sys/class/remoteproc/remoteproc1/state
	    echo "start" > /sys/class/remoteproc/remoteproc1/state
#	else
#		echo "Rebooting pru-core 1 -old redos"
#		echo "4a338000.pru1"  > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
#		echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind
	fi
