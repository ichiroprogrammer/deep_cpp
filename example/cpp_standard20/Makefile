VPATH:=../cpp_standard
SRCS:=\
    literal_ut.cpp enum_ut.cpp const_xxx_ut.cpp explicit_ut.cpp comparison_operator_ut.cpp 

OBJDIR:= obj20/
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
