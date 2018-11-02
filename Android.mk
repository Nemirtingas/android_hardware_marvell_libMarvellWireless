ifneq ($(filter pxa1908,$(TARGET_BOARD_PLATFORM)),)
    include $(call all-named-subdir-makefiles,pxa1908)
endif
