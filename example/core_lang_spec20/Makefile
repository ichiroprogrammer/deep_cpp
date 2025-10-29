VPATH:=../core_lang_spec
SRCS:=\
    literal_ut.cpp enum_ut.cpp explicit_ut.cpp comparison_operator_ut.cpp if_switch_init_ut.cpp concept_ut.cpp \
    decltype_ut.cpp co_await_ut.cpp co_yield_ut.cpp  designated_init_ut.cpp constexpr_ut.cpp \
    abbreviated_func_template_ut.cpp

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
