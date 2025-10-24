SRCS20:=

SRCS:=\
     ambiguous_ownership_ut.cpp unique_ptr_ownership_ut.cpp shared_ptr_ownership_ut.cpp shared_ptr_cycle_ut.cpp \
     deep_shallow_copy_ut.cpp semantics_ut.cpp slice_ut.cpp rule_of_zero_ut.cpp \

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
