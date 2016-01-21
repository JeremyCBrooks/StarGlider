# Copyright (C) 2009 The Android Open Source Project
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
JNI_ROOT_DIR := $(LOCAL_PATH)
CORE_FILES_DIR := $(JNI_ROOT_DIR)/../../../src/core
$(warning ========Setup=Directories===================================)
$(warning Directory JNI_ROOT_DIR name: '$(JNI_ROOT_DIR)')
$(warning Directory CORE_FILES_DIR name: '$(CORE_FILES_DIR)')
$(warning ========Setup=Directories==END==============================)



#BUILD LIBRARIES
$(warning ============================================================)
$(warning ========BUILD=Prereq===================================)
include $(call all-subdir-makefiles)
$(warning ============================================================)
#setup directories
LOCAL_PATH:= $(call my-dir)
$(warning ============================================================)
$(warning ========LIST=FILES==========================================)
CORE_FILES := $(wildcard $(CORE_FILES_DIR)/*.cpp)
JNI_FILES := $(wildcard $(JNI_ROOT_DIR)/*.cpp)
ALLFILES := $(sort $(JNI_FILES)) $(sort $(CORE_FILES))
$(warning Found Core Files : '$(CORE_FILES)')
$(warning Found JNI Files : '$(JNI_FILES)')
$(warning Found ALL Files : '$(ALLFILES)')

$(warning ========LIST=FILES=END======================================)
include $(CLEAR_VARS)


BUILD_PATH := $(LOCAL_PATH)/$(SCONS_BUILD_ROOT)

LOCAL_MODULE    := StarGliderNative
#LOCAL_C_INCLUDES += include $(JNI_ROOT_DIR) $(CORE_FILES_DIR) libzip/ libpng/
#LOCAL_STATIC_LIBRARIES := libpng libzip 
LOCAL_C_INCLUDES += include $(JNI_ROOT_DIR) $(CORE_FILES_DIR)
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -O2 -Wall -D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_CPPFLAGS  := -std=c++11 $(LOCAL_C_INCLUDES:%=-I%) -O2 -fexceptions
LOCAL_SRC_FILES := $(ALLFILES)
LOCAL_LDLIBS    := -lGLESv2 -llog -ldl -lz



include $(BUILD_SHARED_LIBRARY)


