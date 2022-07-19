release: all
	psp-packer $(TARGET).prx
    
TARGET = 660_plugins_on_661
OBJS = main.o

INCDIR = include
CFLAGS = -Os -G0 -Wall -fno-pic -fshort-wchar
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

PSP_FW_VERSION = 661

USE_KERNEL_LIBS=1
USE_KERNEL_LIBC=1

LIBDIR = ../../lib
LDFLAGS = -mno-crt0 -nostartfiles
LIBS =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

