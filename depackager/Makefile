TARGET = PSP_depackager
OBJS =  src/main.o src/exports.o src/lib.o src/setk1.o src/imports.o src/aes.o

all:
	rm -rf EBOOT.PBP
	pack-pbp EBOOT.PBP PARAM.SFO NULL NULL NULL NULL NULL PSP_depackager.prx NULL
	make clean
	mv EBOOT.PBP "PSP/GAME/PSP depackager/EBOOT.PBP"

CFLAGS = -Wall -Wextra -O2 -G0
ASFLAGS = $(CFLAGS)

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

PSP_FW_VERSION=660

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
