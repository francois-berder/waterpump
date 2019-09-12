CC := arm-none-eabi-gcc
CP := arm-none-eabi-objcopy

CONFIG ?= release

BUILDDIR := build

OBJDIR := $(BUILDDIR)/$(CONFIG)/obj
DEPDIR := $(BUILDDIR)/$(CONFIG)/dep
BINDIR := $(BUILDDIR)/$(CONFIG)/bin

CFLAGS += -mcpu=cortex-m0plus -mthumb -mthumb-interwork -mlittle-endian -mfloat-abi=soft
LDFLAGS += -mcpu=cortex-m0plus -mthumb -mthumb-interwork -mlittle-endian -mfloat-abi=soft

CFLAGS += -Wall -Wextra
CFLAGS += -ffreestanding
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -I CMSIS/Include -I src
DEPFLAGS = -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BINDIR)/$(TARGET).map
LDFLAGS += -specs=nosys.specs
LDFLAGS += -L ldscripts -T stm32l051xx.ld

ifeq ($(CONFIG),release)
CFLAGS += -O2 -fno-delete-null-pointer-checks
LDFLAGS += -O2 -fno-delete-null-pointer-checks
else ifeq ($(CONFIG),debug)
CFLAGS += -ggdb -g3 -gz
LDFLAGS += -ggdb -g3 -gz
endif

SRCS := src/app/main.c src/mcu/gpio.c src/mcu/rtc.c src/mcu/startup.c
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS := $(SRCS:%.c=$(DEPDIR)/%.d)

TARGET := waterpump-$(CONFIG)

.PHONY: all
all: $(BINDIR)/$(TARGET).hex

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	$(CP) -O ihex $^ $@

$(BINDIR)/$(TARGET).elf: $(OBJS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@mkdir -p $(DEPDIR)/$(<D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/$(CONFIG)

.PHONY: distclean
distclean:
	rm -rf $(BUILDDIR)

.PHONY: flash-target
flash-target: $(BINDIR)/$(TARGET).hex
	$(CURDIR)/tools/flash.sh ${JLINK_DEVICE} $^

.PHONY: debug-target
debug-target: flash-target
	$(CURDIR)/tools/debug.sh ${JLINK_DEVICE} $(BINDIR)/$(TARGET).elf

-include $(DEPS)
