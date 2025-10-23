VPATH:=../term_explanation
SRCS:=\
    decltype_ut.cpp concept_ut.cpp \
    co_await_ut.cpp co_yield_ut.cpp  \
	designated_init_ut.cpp

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
