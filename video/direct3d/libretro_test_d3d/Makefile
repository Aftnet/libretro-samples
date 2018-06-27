TARGET_NAME := libretro_test_d3d

include Makefile.common

ifneq (,$(findstring msvc,$(platform)))
CFLAGS += -DHAVE_D3D11
CXXLAGS += -DHAVE_D3D11
include Makefile.msvc
endif
