LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := aEdax # should be renamed to lib..aEdax..so afterwords
LOCAL_CFLAGS += -DUNICODE
<<<<<<< HEAD
<<<<<<< HEAD
LOCAL_SRC_FILES := all.c board_sse.c.neon eval_sse.c.neon flip_neon_bitscan.c.neon android/cpu-features.c
LOCAL_ARM_NEON := false
=======
LOCAL_SRC_FILES := all.c
# LOCAL_ARM_NEON := true
>>>>>>> f2da03e (Refine arm builds adding neon support.)
=======
LOCAL_SRC_FILES := all.c board_sse.c.neon eval_sse.c.neon flip_neon_bitscan.c.neon android/cpu-features.c
LOCAL_ARM_NEON := false
>>>>>>> 343493d (More neon/sse optimizations; neon dispatch added for arm32)
# cmd-strip :=
include $(BUILD_EXECUTABLE)
