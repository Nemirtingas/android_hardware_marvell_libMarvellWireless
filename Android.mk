ifeq ($(TARGET_BOARD_SOC),pxa1908)
    PXADIRS := pxa1908
endif
ifeq ($(TARGET_BOARD_SOC),pxa1088)
    PXADIRS := pxa1088
endif

ifneq ($(PXADIRS),)
    include $(call all-named-subdir-makefiles,$(PXADIRS))
endif
