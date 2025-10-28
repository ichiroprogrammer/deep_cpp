SRCS:=\
    container_ut.cpp heap_allocation_elision_ut.cpp lock_ownership_wrapper_ut.cpp optional_ut.cpp thread_ut.cpp \
	type_traits_ut.cpp utility_ut.cpp variant_ut.cpp weak_ptr_ut.cpp comparison_stdlib_ut.cpp


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
