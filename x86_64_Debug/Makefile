# vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*-
# vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
#****************************************************************************
# (C) Copyright 2023-2024 Lynx Software Technologies, Inc. All rights reserved.
#****************************************************************************

include $(ENV_PREFIX)/ENVIRONMENT
include ./BSP_RULES

SOC_PATH = ../soc.x86
CERTIFIED		= false
RSC_COMPONENT	= false
LSK_IS_USED		= false
BSP				= bsp.$(THIS_BSP).o
UPARAM_H		= uparam.h
CONFIG_TBL		= config.out
TIMESTAMP_OFILE	= timestamp.o
REMOTE_KERN_DIR	= $(SOC_PATH)
GNUEFI_DIR		= ../gnuefi

REMOTE_FILES = bsp_conf.x bsp_device.x bsp_drm.x bsp_env_am.x bsp_env.x \
	bsp_init_am.x bsp_init.x bsp_intr.x bsp_lowInit.x bsp_map_fs.x \
	bsp_pcibus.x bsp_reboot.x bsp_timer_am.x bsp_timer.x bsp_tod.x bsp_kputc.x
DATA_FILES = bsp_device_table.x drm_conf.x

ifeq ($(BUILD_MODE), $(MODE_DEV))
	DATA_FILES_DEV = sysctl_list.x
endif

SRC_FILES = $(REMOTE_FILES) bsp_relocate.x version.x

ASM_FILES = entry_64.x

ifeq ($(KERNEL_IS_INSTRUMENTED), yes)
SRC_FILES_UNINSTRUMENTED = bsp_env_am_uninstrumented.x \
	bsp_init_am_uninstrumented.x bsp_lowInit_uninstrumented.x \
	bsp_map_fs_uninstrumented.x bsp_relocate_uninstrumented.x \
	bsp_timer_am_uninstrumented.x conf_am_uninstrumented.x \
	entry_uninstrumented.x
endif

FILES = $(SRC_FILES) $(SRC_FILES_OPTIONAL) $(SRC_FILES_UNINSTRUMENTED) \
	$(ASM_FILES) $(DATA_FILES) $(DATA_FILES_DEV)

DOC_FILES = board_support_guide_$(THIS_BSP).x

include $(ENV_PREFIX)/sys/OBJ_RULES

all: $(PDF_FILES) re_all

$(BSP): $(STANDARD_OFILES) $(CONF_OFILES)
	@$(ECHO)
	@$(ECHO) Creating $@ ...
	@$(ECHO)
	$(KERN_LD) -r -o $@ $(STANDARD_OFILES)

include $(SOC_PATH)/Makefile.common

# The RSC_FILES_X variable is used by the 'make emit_rsc_files' command.
# Refer to the 'emit_rsc_files:' goal in sys/OBJ_RULES for details.
RSC_FILES_X = $(SRC_FILES) $(CONF_FILES)

BSP_H_FILES = $(KERN_INC_PATH)/bsps/$(THIS_BSP)/bsp_proto.h

FAMILY_H_FILES = $(FAM_INC_PATH)/arch.h $(FAM_INC_PATH)/arch_mem.h \
	$(FAM_INC_PATH)/context.h $(FAM_INC_PATH)/ctx_proto.h \
	$(FAM_INC_PATH)/exception.h

KERNEL_H_FILES = $(KERNEL_INC_PATH)/drm.h $(KERNEL_INC_PATH)/environ.h \
	$(KERNEL_INC_PATH)/pci_resource.h $(KERNEL_INC_PATH)/sysinit.h

SYS_H_FILES = $(KERN_INC_PATH)/drm/pcibus.h $(KERN_INC_PATH)/io/bsp_device.h

USR_H_FILES = $(USER_INC_PATH)/a.out.h $(USER_INC_PATH)/boot/boot_modes.h \
	$(USER_INC_PATH)/lynx/info.h $(USER_INC_PATH)/rk.h \
	$(USER_INC_PATH)/sys/queue.h

REMOTE_ARTIFACT_FILES = bsp_conf.c bsp_device.c bsp_drm.c bsp_env_am.c \
	bsp_env.c bsp_init_am.c bsp_init.c bsp_intr.c bsp_lowInit.c \
	bsp_map_fs.c bsp_pcibus.c bsp_reboot.c bsp_timer_am.c bsp_timer.c \
	bsp_tod.c entry_64.S

ARTIFACT_SRC_FILES = bsp_relocate.c conf_am.c conf.c info.c version.c \
	$(addprefix $(SOC_PATH)/, $(REMOTE_ARTIFACT_FILES)) $(BSP_H_FILES) \
	$(FAMILY_H_FILES) $(KERNEL_H_FILES) $(SYS_H_FILES) $(USR_H_FILES)

ifeq ($(KERNEL_IS_INSTRUMENTED), yes)
conf_am_uninstrumented.o: conf_kas.h $(UPARAM_H)
endif

bootable_img: $(IMG_PATH)$(IMG_EXTN)
	$(MAKE) -C $(GNUEFI_DIR) clean;
	$(CP) $(IMG_PATH)$(IMG_EXTN) $(GNUEFI_DIR)/lynxos-178.kdi
	$(MAKE) THIS_BSP=$(THIS_BSP) -C $(GNUEFI_DIR) OUT_PATH=$(IMG_PATH).efi

# End of file.
