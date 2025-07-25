SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/.metadata/product.json --product $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler ticlang --product $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/.metadata/product.json --product $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../mcuboot.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = mcuboot_config_led_debug.obj mcuboot_app.obj keys.obj flash_map_backend_flash_map_backend.obj flash_map_backend_ext_flash.obj bootutil_src_security_cnt.obj bootutil_src_boot_record.obj bootutil_src_image_ed25519.obj bootutil_src_image_ecdsa.obj bootutil_src_bootutil_misc.obj bootutil_src_image_rsa.obj bootutil_src_image_validate.obj bootutil_src_bootutil_public.obj bootutil_src_loader.obj bootutil_src_caps.obj bootutil_src_swap_misc.obj bootutil_src_encrypted.obj bootutil_src_swap_move.obj bootutil_src_fault_injection_hardening.obj bootutil_src_fault_injection_hardening_delay_rng_mbedtls.obj bootutil_src_swap_scratch.obj bootutil_src_tlv.obj mbedtls-asn1_asn1parse.obj mbedtls-asn1_platform_util.obj ti-crypto_sl_crypto.obj ecdsa_lpf3.obj flash_map_backend_bsp_spi_lpf3.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = mcuboot

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -Oz \
    -DEXCLUDE_TRACE \
    -DBOOT_LOADER \
    -DSPI_CTL0_SPO_LOW=SPI_CTL0_SPO_LO \
    -DSPI_CTL0_SPO_HIGH=SPI_CTL0_SPO_HI \
    -std=gnu9x \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -Wall \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/kernel/nortos" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/kernel/nortos/posix" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/ecc/include" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/ti/devices/cc23x0r5/driverlib" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/include" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/mbedtls-asn1/include" \
    "-I$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/ti/source/mcuboot_app" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += "-L$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source" \
    "-L$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/kernel/nortos" \
    "-L$(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/ecc/lib/ticlang/m0/ecc.a" \
    -L. \
    ti_utils_build_linker.cmd.genlibs \
    -lthird_party/ecc/lib/ticlang/m0p/ecc.a \
    ../mcuboot_cc2340r5.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: postbuild

.PHONY: postbuild
postbuild: $(NAME).out
	$(TICLANG_ARMCOMPILER)/bin/tiarmhex -order MS --memwidth=8 --romwidth=8 --intel -o $(NAME).hex $(NAME)

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../mcuboot.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../mcuboot.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

mcuboot_config_led_debug.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/ti/common/cc26xx/debug/led_debug.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mcuboot_app.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/ti/source/mcuboot_app/mcuboot_app.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

keys.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/ti/source/mcuboot_app/keys.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flash_map_backend_flash_map_backend.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/ti/source/mcuboot_app/flash_map_backend/flash_map_backend.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flash_map_backend_ext_flash.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/ti/common/flash/no_rtos/extFlash/ext_flash.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_security_cnt.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/ti/source/mcuboot_app/security_cnt.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_boot_record.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/boot_record.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_image_ed25519.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/image_ed25519.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_image_ecdsa.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/image_ecdsa.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_bootutil_misc.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/bootutil_misc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_image_rsa.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/image_rsa.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_image_validate.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/image_validate.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_bootutil_public.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/bootutil_public.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_loader.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/loader.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_caps.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/caps.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_swap_misc.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/swap_misc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_encrypted.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/encrypted.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_swap_move.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/swap_move.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_fault_injection_hardening.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/fault_injection_hardening.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_fault_injection_hardening_delay_rng_mbedtls.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_swap_scratch.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/swap_scratch.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

bootutil_src_tlv.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/src/tlv.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbedtls-asn1_asn1parse.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/mbedtls-asn1/src/asn1parse.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbedtls-asn1_platform_util.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/mbedtls-asn1/src/platform_util.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti-crypto_sl_crypto.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/ti-crypto/sl_crypto.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ecdsa_lpf3.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/ti/common/ecdsa_lpf3/ecdsa_lpf3.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flash_map_backend_bsp_spi_lpf3.obj: $(SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR)/source/ti/common/flash/no_rtos/extFlash/bsp_spi_lpf3.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
