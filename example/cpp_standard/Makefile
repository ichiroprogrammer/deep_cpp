SRCS20:=literal_ut.cpp 

SRCS:=\
    integral_promotion_ut.cpp etc_ut.cpp heap_allocation_elision_ut.cpp float_ut.cpp trivial_ut.cpp \
    incomplete_type_ut.cpp class_layout_ut.cpp rtti_ut.cpp interface_class.cpp constructor_ut.cpp \
    implicit_conversion_ut.cpp undefined_ut.cpp argument.cpp aliging_ut.cpp user_defined_literal_ut.cpp nsdmi.cpp \
    rvalue_lvalue_ut.cpp rvalue_move_ut.cpp universal_ref_ut.cpp perfect_forwarding_ut.cpp 


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
