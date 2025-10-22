SRCS20:=literal_ut.cpp 

SRCS:=\
    integral_promotion_ut.cpp etc_ut.cpp heap_allocation_elision_ut.cpp float_ut.cpp trivial_ut.cpp \
	incomplete_type_ut.cpp \


SRCS+=$(SRCS20)

CPP_VER:= c++17
SHARED:=../../
include $(SHARED)make/env.mk

ifeq ($(CXX), g++)
# @@@ sample begin 0:0

CCFLAGS_ADD:=-fdump-lang-class
# @@@ sample end
CCFLAGS_ADD:=$(CCFLAGS_ADD) -fcoroutines
endif

include $(SHARED)make/example.mk
include $(SHARED)make/gtest.mk
