# ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ###
# Automatically-generated file. Do not edit!
LUM_ALL = driver kernel kdi
LUM_SPEC_NAME = dd.spec
LUM_MKIMAGE_FLAGS =
LUM_KDI_NAME = dd.kdi
LUM_LYNXOS_BOOT = 
# ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ###
LUM_OTHER_CLEAN = cleandrv
OTHER = 
DRVR = drvr
DRV_IS_DLDD = true
LUM_KERNEL_DIRECTORY = $(ENV_PREFIX)/sys/bsp.$(TARGET_BSP)
LUM_CONFIG_NAME = $(LW_CURRENT_BSPDIR)/config.tbl
LUM_UPARAM_H = $(LW_CURRENT_BSPDIR)/uparam.h

PWD := $(shell pwd)

## Configuration-related variables
# CFG directory
LUM_CFG_DIR = ./cfg.$(LYNXTARGET)
LUM_CONFIG = $(LW_CURRENT_BSPDIR)/config.tbl
LUM_CONFIG_OUT = $(LW_CURRENT_BSPDIR)/config.out
LUM_GENERATED_CONFIG = $(LUM_CONFIG_OUT) $(LW_CURRENT_BSPDIR)/CONFIG.h \
             $(LW_CURRENT_BSPDIR)/nodetab $(LW_CURRENT_BSPDIR)/sysdevices.h

include $(ENV_PREFIX)/ENVIRONMENT

all: lcsconfig $(LW_CURRENT_BSPDIR) $(LUM_CFG_DIR) $(LUM_ALL)

lcsconfig:
	(cd lcs; make INSTALL_DIR_CFG=$(PWD)/$(LUM_CFG_DIR) INSTALL_DIR_SYS_LIB=$(PWD)/lib install)

kernel: $(LW_CURRENT_BSPDIR) $(LUM_CFG_DIR) $(LW_CURRENT_BSPDIR)/a.out

clean: $(LW_CURRENT_BSPDIR) $(LUM_CFG_DIR) $(LUM_OTHER_CLEAN)
	(cd lcs; make INSTALL_DIR_CFG=$(PWD)/$(LUM_CFG_DIR) INSTALL_DIR_SYS_LIB=$(PWD)/lib clean)
	@-cp $(LW_CURRENT_BSPDIR)/VCT .
	@-cp $(LW_CURRENT_BSPDIR)/$(LUM_SPEC_NAME) .
	(cd $(LW_CURRENT_BSPDIR); make INSTALL_DIR_SYS_LIB=$(PWD)/lib clean)
	@-mv $(LUM_SPEC_NAME) $(LW_CURRENT_BSPDIR)/$(LUM_SPEC_NAME)
	@-mv VCT $(LW_CURRENT_BSPDIR)/VCT

$(LW_CURRENT_BSPDIR):
	mkdir -p $(LW_CURRENT_BSPDIR)
	for i in $(LUM_KERNEL_DIRECTORY)/*; do \
	  n=`basename $$i`; \
	  if [ -d $$i ]; then \
	    if [ $$n != "CVS" ]; then \
	      cp -dpR $$i $(LW_CURRENT_BSPDIR); \
	    fi; \
	    continue;
	  fi; \
	  if [ $$n != "VCT.in" -a $$n != "lynxos-178.spec.in" -a $$n != lynxos-178.spec ]; \
	  then cp $$i $(LW_CURRENT_BSPDIR); fi; \
	done

$(LUM_CFG_DIR):
	mkdir -p $(LUM_CFG_DIR)
	cp -pRL $(ENV_PREFIX)/sys/cfg/* $(LUM_CFG_DIR)

$(LW_CURRENT_BSPDIR)/a.out: $(LUM_GENERATED_CONFIG) $(LUM_UPARAM_H) lcs/lcsinfo.c
	(cd $(LW_CURRENT_BSPDIR); \
	 rm -f a.out; \
	 make INSTALL_DIR_SYS_LIB=$(PWD)/lib OTHER=$(OTHER) a.out)

ifneq ($(LUM_KDI_NAME),)
kdi: $(LW_CURRENT_BSPDIR) $(LW_CURRENT_BSPDIR)/$(LUM_SPEC_NAME) $(LW_CURRENT_BSPDIR)/a.out Makefile
	rm -f $(LW_CURRENT_BSPDIR)/$(LUM_KDI_NAME)
	(cd $(LW_CURRENT_BSPDIR); \
	 make INSTALL_DIR_SYS_LIB=$(PWD)/lib KDI_NAME=$(LUM_KDI_NAME) SPEC_NAME=$(LUM_SPEC_NAME) MKIMAGE="mkimage $(LUM_MKIMAGE_FLAGS)" OTHER=$(OTHER) kdi)
	@if [ -f $(LW_CURRENT_BSPDIR)/$(LUM_KDI_NAME) ]; then \
	  echo "**** KDI is successfully built ****"; \
	fi
endif

# Configuration-related staff
$(LUM_GENERATED_CONFIG): $(LUM_CONFIG)
	@make config

config: 
	@$(ECHO) Creating $(LUM_CONFIG_OUT) ...
	@$(ECHO) -n "m4 " > config.tmp
ifneq ($(RELEASE_FLAGS),)
	@for flag in $(RELEASE_FLAGS); \
	do \
	  $(ECHO) -n $$flag >> config.tmp; $(ECHO) -n "=1 " >> config.tmp; \
	done
endif
	@$(ECHO) -n " < $(LUM_CONFIG)" >> config.tmp
	@$(ECHO) " > $(LUM_CONFIG_OUT)" >> config.tmp
	@$(CHMOD) +x config.tmp
	@./config.tmp
	@$(RM) config.tmp
	@$(ECHO)
	@$(ECHO) Processing $(LUM_CONFIG_OUT) ...
	@$(ECHO)
	if [ -f local/passwd ]; then \
		config $(LUM_GENERATED_CONFIG) $(LUM_CFG_DIR) local/passwd $(ENV_PREFIX)/etc/group; \
	else \
		config $(LUM_GENERATED_CONFIG) $(LUM_CFG_DIR) $(ENV_PREFIX)/etc/passwd $(ENV_PREFIX)/etc/group; \
	fi

# Driver-related staff

DD_SRC_DIR = $(DRVR)
DD_BIN_DIR = $(LW_CURRENT_BSPDIR)/$(shell basename $(DRVR))
                                                                                
driver: $(DD_BIN_DIR)
	(cd $(DD_SRC_DIR); make DRV_IS_DLDD=$(DRV_IS_DLDD) \
	 LW_DEBUG=$(LW_DEBUG) \
	 DD_BIN_DIR=../$(DD_BIN_DIR) all)

$(DD_BIN_DIR):
	mkdir -p $@
	touch $@/Makefile.flags

cleandrv:
	(cd $(DD_SRC_DIR); make DRV_IS_DLDD=$(DRV_IS_DLDD) \
	 DD_BIN_DIR=../$(DD_BIN_DIR) clean)

