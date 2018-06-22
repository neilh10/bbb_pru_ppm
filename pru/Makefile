# 
# Copyright (c) 2016 Zubeen Tolani <ZeekHuge - zeekhuge@gmail.com>
# Copyright (c) 2017 Texas Instruments - Jason Kridner <jdk@ti.com>
#
PROJ_NAME=ppm-pru

# PRU_CGT environment variable must point to the TI PRU compiler directory.
# PRU_SUPPORT points to pru-software-support-package
# Both are set in setup.sh
PRU_CGT:=/usr/share/ti/cgt-pru
PRU_SUPPORT:=/usr/lib/ti/pru-software-support-package

LINKER_COMMAND_FILE=./AM335x_PRU.cmd
LIBS=--library=$(PRU_SUPPORT)/lib/rpmsg_lib.lib
INCLUDE=--include_path=$(PRU_SUPPORT)/include --include_path=$(PRU_SUPPORT)/include/am335x
STACK_SIZE=0x100
HEAP_SIZE=0x100

CFLAGS=-v3 -O2 --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) -ppd -ppa --asm_listing --c_src_interlist # --absolute_listing
LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE) -m $(GEN_DIR)/file.map

GEN_DIR=gen

PRU0_FW		=$(GEN_DIR)/$(PROJ_NAME).out

# -----------------------------------------------------
# Variable to edit in the makefile

# add the required firmwares to TARGETS
TARGETS		=$(PRU0_FW) 

#------------------------------------------------------

.PHONY: all
all: $(TARGETS)
	@echo '-	Generated firmwares are : $^'

$(PRU0_FW): $(GEN_DIR)/$(PROJ_NAME).obj
	@echo 'LD	$^' 
	@lnkpru -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $@ $^  $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS) $^

$(GEN_DIR)/$(PROJ_NAME).obj: $(PROJ_NAME).c 
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<


.PHONY: install0 run0 

install0:
	@echo '-   copying firmware file $(PRU0_FW) to /lib/firmware/am335x-pru0-fw'
	@cp $(PRU0_FW) /lib/firmware/am335x-pru0-fw

run0: install0
	@echo '-   rebooting pru core 0'
	$(shell echo "stop" > /sys/class/remoteproc/remoteproc1/state)
	$(shell echo "start" > /sys/class/remoteproc/remoteproc1/state)
	@echo " *** $(PRU0_FW) is on PRU0"

#run1: install1
#	@echo '-	rebooting pru core 1'
#	$(shell echo "stop" > /sys/class/remoteproc/remoteproc2/state)
#	$(shell echo "start" > /sys/class/remoteproc/remoteproc2/state)
#	@echo "-YES	pru core 0 is now loaded with $(PRU0_FW)"

.PHONY: clean
clean:
	@echo 'CLEAN	.'
	@rm -rf $(GEN_DIR) $(PROJ_NAME).asm
