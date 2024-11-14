SRCS:=\
    decltype_ut.cpp literal_ut.cpp comparison_operator_ut.cpp concept_ut.cpp \
    co_await_ut.cpp co_yield_ut.cpp  explicit_ut.cpp enum_ut.cpp const_xxx_ut.cpp \
    if_switch_init_ut.cpp

CPP_VER = c++20
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
