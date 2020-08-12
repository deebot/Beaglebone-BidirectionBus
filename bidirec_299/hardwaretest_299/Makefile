# Copyright (c) 2014, Dimitar Dimitrov
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of the copyright holders nor the names of
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.

# Very simple makefile to cross-compile for PRU


# Common flags
CROSS_COMPILE ?= pru-
CFLAGS += -g -Os
CFLAGS += -Wall -Wextra

# Add remoteproc headers
CFLAGS += -isystem include

# Define this to squeeze code size by removing atexit, exit, constructors
# and destructors from CRT.
CFLAGS += -minrt

# Per-PRU core flags. The -mmcu option will select the correct linker
# script and will predefine mcu-specific macros.
CFLAGS0 += -mmcu=am335x.pru0

# List of source files to compile for each PRU core.
SRC0 := main0.c

# GCC's -MMD does not yield the needed C dependencies when compiling all
# C source files at once. So manually list headers here.
HEADERS := $(wildcard *.h)

# Where to output compiled objects
OUT := out

# Final ELF image file names
ELF0 := $(OUT)/pru-core0.elf

TARGET      =$(OUT)/$(PROJ_NAME).elf
PRU0        :=/sys/class/remoteproc/remoteproc1
PRU1        :=/sys/class/remoteproc/remoteproc2
STATESRUNNING   :=running
STATESSTOPED   :=offline
CURRENTSTATE0:=$(shell cat $(PRU0)/state)
CURRENTSTATE1:=$(shell cat $(PRU1)/state)
# ============================ DO NOT TOUCH BELOW ============================
all: $(ELF0)
	@echo Success: $^

%.s : %.elf
	$(CROSS_COMPILE)objdump -S -d $< > $@

# Binary images for IMEM and DMEM memories. Might be useful
# for firmware loaders that cannot handle ELF.
%.imem.bin : %.elf
	$(CROSS_COMPILE)objcopy -O binary  --only-section=.text* $< $@

%.dmem.bin : %.elf
	$(CROSS_COMPILE)objcopy -O binary              \
	  --only-section=.data*                        \
	  --only-section=.bss*                         \
	  --set-section-flags .bss=alloc,load,contents \
	  $< $@

$(OUT):
	mkdir $(OUT)

$(ELF0): $(SRC0) $(HEADERS) | $(OUT)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(CFLAGS0) $(SRC0) $(LDFLAGS) -o $@

clean:
	$(RM) -fr $(ELF0) $(OUT)
install_PRU0:
	@echo 'Installation Process Initiated'
ifeq ($(CURRENTSTATE0),$(STATESRUNNING))
	@echo 'Stopping current PRU0 application ($(PRU0))'
	@echo stop | sudo tee $(PRU0)/state
endif
	@echo 'Installing firmware'
	@sudo cp $(ELF0) /lib/firmware/am335x-pru0-fw
	@echo 'Starting new PRU0 application'
	@echo start | sudo tee $(PRU0)/state

stop_PRU0:
	@echo 'Installation Process Initiated'
ifeq ($(CURRENTSTATE0),$(STATESRUNNING))
	@echo 'Stopping current PRU0 application ($(PRU0))'
	@echo stop | sudo tee $(PRU0)/state
	@echo 'Stopped PRU0'
endif

start_PRU0:
	@echo 'Installation Process Initiated'
ifeq ($(CURRENTSTATE0),$(STATESSTOPED))
	@echo 'Starting current PRU0 application ($(PRU0))'
	@echo start | sudo tee $(PRU0)/state
	@echo 'Started PRU0'
else
	@echo 'PRU0 Already in the running state. execute stop_PRU0 first '
	
endif

cscope:
	cscope -bRk

.PHONY: all clean cscope


