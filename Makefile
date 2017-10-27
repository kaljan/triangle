
TARGET=triangle

OUTPUT=build/$(TARGET)

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

CFLAGS = --specs=nosys.specs
CFLAGS += -v -O0
#CFLAGS += -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -mlittle-endian
CFLAGS += -mthumb
CFLAGS += -mthumb-interwork
CFLAGS += -mcpu=cortex-m3
CFLAGS += -fsingle-precision-constant
CFLAGS += -Wdouble-promotion

LDFLAGS += -Tsystem/stm32_flash.ld

DEFINES += -DSTM32F10X_MD_VL

INCLUDEPATH += -I$(PWD)/src
INCLUDEPATH += -I$(PWD)/system
INCLUDEPATH += -I$(HOME)/work/tools/arm
INCLUDEPATH += -I$(HOME)/work/tools/arm/st

SRC = \
	system/startup_stm32f10x_md_vl.s \
	system/systick.c \
	system/system_stm32f10x.c \
	system/interrupts.c \
	src/ssd1311.c \
	src/main.c \
	src/i2c.c \
	src/gpio.c

# .PHONY: $(TARGET)

$(TARGET): $(OUTPUT).elf

$(OUTPUT).elf: $(SRC)
		$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDEPATH) $^ -o $@
		$(CP) -O binary $(OUTPUT).elf $(OUTPUT).bin

clean:
	rm -rfv *.o *.elf *.bin

flash:
	$(SF) write $(TARGET).bin 0x8000000
