DISTFILES += \
    system/stm32_flash.ld \
    system/startup_stm32f10x_md_vl.s \
    Makefile

DEFINES += STM32F10X_MD_VL

HEADERS += \
    system/interrupts.h \
    src/ssd1311.h \
    src/gpio.h

SOURCES += \
    system/systick.c \
    system/system_stm32f10x.c \
    system/interrupts.c \
    src/ssd1311.c \
    src/main.c \
    src/i2c.c \
    src/gpio.c

INCLUDEPATH += \
	$(HOME)/work/tools/arm \
	$(HOME)/work/tools/arm/st
