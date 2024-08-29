# vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*-
# vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
#****************************************************************************
# (C) Copyright 2022-2024 Lynx Software Technologies, Inc. All rights reserved.
#****************************************************************************
include $(ENV_PREFIX)/ENVIRONMENT

OS_NAME = lynxos-178
OBJS	= main.o
ASMS	= $(subst .o,.s,$(OBJS))
TARGET	= $(OS_NAME).efi
SHLIB	= $(OS_NAME).so
KDI		= $(OS_NAME).kdi

ifndef OUT_PATH
OUT_PATH = .
endif

ifndef NETKDI
NETKDI = $(KDI)
endif

EFIINC			= /usr/include/efi
EFIINCS			= -I$(EFIINC) -I$(EFIINC)/$(LYNXTARGET) -I$(EFIINC)/protocol

# Check to see where libgnuefi.a and libefi.a are installed
ifeq ($(and $(wildcard /usr/lib64/libgnuefi.a), $(wildcard /usr/lib64/libefi.a)),)
	LIB				= /usr/lib
else
	LIB				= /usr/lib64
endif

# Use $(LIB)/gnuefi if it exists, otherwise fall back on $(LIB)
EFILIB			= $(firstword $(wildcard $(LIB)/gnuefi $(LIB)))
EFI_CRT_OBJS	= $(EFILIB)/crt0-efi-$(LYNXTARGET).o
EFI_LDS			= $(ENV_PREFIX)/sys/bsp.$(THIS_BSP)/elf_$(LYNXTARGET)_efi.lds
LIBGNUEFI		= $(LIB)/libgnuefi.a
LIBEFI			= $(LIB)/libefi.a
RED				= \033[0;31m
NC				= \033[0m # No Color
# TODO: need to update CDK version of objcopy to recognize EFI target
# For now we are using host objcopy, hence depending on host binutils
HOST_OBJCOPY=/usr/bin/objcopy

GNU_EFI_PREREQUISITES = $(LIBGNUEFI) $(LIBEFI) $(EFI_LDS) $(EFI_CRT_OBJS)

CFLAGS	= $(EFIINCS) -fpic -fno-stack-protector -fshort-wchar -mno-red-zone \
	-Wall

ifeq ($(LYNXTARGET),x86_64)
CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS	= -nostdlib -T $(EFI_LDS) -shared -Bsymbolic \
	-L$(EFILIB) -L$(LIB) $(EFI_CRT_OBJS) 

install: all
	
all: $(KDI) check_gnu_efi $(TARGET)

%.s: %.c
	$(CC) -S $(CFLAGS) -o $@ $<

%.o: %.s
	$(AS) $(ASMFLAGS) -o $@ $<

$(SHLIB): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

CURDIR	=	$(shell pwd)

# Override implicit rule
%.o : %.c

check_gnu_efi:
	@for requisite in $(GNU_EFI_PREREQUISITES); do \
		if [ ! -f $$requisite ]; then \
			echo -e "${RED} $$requisite does not exist. Maybe Host doen't have gnu-efi lib.${NC}"; \
			echo -e "${RED} Please check $(CURDIR)/README for pre-requisites.${NC}"; \
			exit 1; \
		fi \
	done

$(TARGET): $(SHLIB) $(NETKDI)
	$(HOST_OBJCOPY) -j .kdi_body -j .kdi -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(LYNXTARGET) $< $@
	@echo "Copying $@ to $(OUT_PATH)"
	@cp -f $@ $(OUT_PATH)

clean:
	rm -f $(OBJS) $(TARGET) $(SHLIB) $(ASMS) $(KDI)

clobber: clean

