#
# Copyright (C) 2018 The LineageOS Project
#                     Nemirtingas <nanaki89@hotmail.fr>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	lib_marvell_wireless.c

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/..

LOCAL_SHARED_LIBRARIES := \
	libc	\
    libcutils \
    libutils 

LOCAL_STATIC_LIBRARIES += 

LOCAL_MODULE:= libMarvellWireless
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Werror
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)





