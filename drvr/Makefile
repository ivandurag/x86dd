#************************************************************
# <Name>
# (C) Copyright 2021 MyCompany, Inc.
# MyTown, MyState   All Rights Reserved.
#
# <Description>
#************************************************************

include $(DD_BIN_DIR)/Makefile.flags
include $(ENV_PREFIX)/sys/OBJ_RULES

SYS_CFLAGS += $(EXTRA_CFLAGS)
ifeq ($(LW_DEBUG),true)
OPTIMIZE=
DEBUG_FLAGS=-g3
else
OPTIMIZE=-O2
DEBUG_FLAGS=
endif

DRIVER_NAME = dd
DRIVER_OBJS = $(DRIVER_NAME)drvr.o
OUTPUT_OBJS = $(DRIVER_OBJS:%=$(DD_BIN_DIR)/%)
DRIVER_SRC = $(DRIVER_NAME)drvr.c

VPATH = .

ifeq ($(DRV_IS_DLDD),true)
# Dynamic Driver.
SPECIAL_CFLAGS += -DDLDD
DYNAMIC_DRIVER_BINARY = $(DD_BIN_DIR)/dd.dldd
DRIVER_INFO_FILE = $(DD_BIN_DIR)/dd.info
BINARIES = $(DYNAMIC_DRIVER_BINARY) $(DRIVER_INFO_FILE)
DYNAMIC_DRIVER_PRE = $(DD_BIN_DIR)/$(DRV_NAME).pre
DYNAMIC_DRIVER_IMPORT = $(DD_BIN_DIR)/$(DRV_NAME).import
else
# Static Driver.
DRIVER_INFO_FILE = $(DD_BIN_DIR)/$(DRIVER_NAME)info.o
DD_LIB = $(DD_BIN_DIR)/libdriver.a
OUTPUT_OBJS += $(DRIVER_INFO_FILE)
BINARIES = $(DD_LIB)
endif

all: $(BINARIES)

ifeq ($(DRV_IS_DLDD),true)
# Dynamic driver.
ifeq ($(OBJSFORMAT),xcoff)
$(DYNAMIC_DRIVER_PRE): $(OUTPUT_OBJS)
	rm -f $(DYNAMIC_DRIVER_PRE)
	$(KERN_LD) -r -o $@ $(EXTRA_LDFLAGS) $^
                                                                                
$(DYNAMIC_DRIVER_IMPORT): $(DYNAMIC_DRIVER_PRE)
	rm -f $(DYNAMIC_DRIVER_IMPORT)
	nm -u $^ | egrep -v ':$$|^$$' | cut -d. -f2- | sort -u > $@
                                                                                
$(DYNAMIC_DRIVER_BINARY): $(DYNAMIC_DRIVER_PRE) $(DYNAMIC_DRIVER_IMPORT)
	rm -f $(DYNAMIC_DRIVER_BINARY)
	$(KERN_LD) -bM:SRE -bimport:$(DYNAMIC_DRIVER_IMPORT) -o $@ \
	  $(DYNAMIC_DRIVER_PRE)
else
$(DYNAMIC_DRIVER_BINARY): $(OUTPUT_OBJS)
	rm -f $(DYNAMIC_DRIVER_BINARY)
	$(KERN_LD) -r -o $@ $(EXTRA_LDFLAGS) $(OUTPUT_OBJS)
	objcopy -R .group $@
endif

$(DRIVER_INFO_FILE): $(DRIVER_NAME)info.c $(DRIVER_NAME)info.h
	rm -f $(DRIVER_INFO_FILE)
	$(KERN_CC) $(SPECIAL_CFLAGS) -c $< -o $(DRIVER_INFO_FILE).o
	objcopy -O binary $(DRIVER_INFO_FILE).o $@
else
# Static driver.
$(DD_LIB): $(OUTPUT_OBJS)
	rm -f $(DD_LIB)
	$(archive) $(OUTPUT_OBJS)
	$(KERN_RANLIB) $@

$(DRIVER_INFO_FILE): $(DRIVER_NAME)info.c $(DRIVER_NAME)info.h
	rm -f $(DRIVER_INFO_FILE)
	$(compile) $(OPTIMIZE) 
endif

clean:
	$(RM) $(BINARIES) $(OUTPUT_OBJS)

$(DD_BIN_DIR)/%.o: %.c $(DRIVER_NAME)drvr.h $(DRIVER_NAME)info.h \
		  $(DD_BIN_DIR)/Makefile.flags
	rm -f $@ $(DYNAMIC_DRIVER_BINARY) $(DD_LIB)
	$(compile) $(OPTIMIZE) $(DEBUG_FLAGS) $(CFLAGS_$(@F))

# End of file.
