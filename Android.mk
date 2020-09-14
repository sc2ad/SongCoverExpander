LOCAL_PATH := $(call my-dir)

TARGET_ARCH_ABI := arm64-v8a

include $(CLEAR_VARS)
LOCAL_MODULE := hook

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Build the beatsaber-hook shared library, SPECIFICALLY VERSIONED!
include $(CLEAR_VARS)
LOCAL_MODULE	        := bs-hook
LOCAL_SRC_FILES         := ./extern/debug_libbeatsaber-hook_0_5_8.so
LOCAL_EXPORT_C_INCLUDES := ./extern/beatsaber-hook/shared/
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
# Include the two libraries
LOCAL_SHARED_LIBRARIES += bs-hook
LOCAL_LDLIBS     := -llog
LOCAL_CFLAGS     := -D'MOD_ID="SongCoverExpander"' -D'VERSION="0.2.0"' -isystem"./extern" -isystem"./include" -isystem 'extern/libil2cpp/il2cpp/libil2cpp'
LOCAL_MODULE     := songcoverexpander
LOCAL_CPPFLAGS   := -std=c++2a
LOCAL_C_INCLUDES := ./include ./src
LOCAL_SRC_FILES  += $(call rwildcard,src/,*.cpp)
LOCAL_SRC_FILES  += $(call rwildcard,extern/beatsaber-hook/src/inline-hook/,*.cpp)
LOCAL_SRC_FILES  += $(call rwildcard,extern/beatsaber-hook/src/inline-hook/,*.c)
include $(BUILD_SHARED_LIBRARY)