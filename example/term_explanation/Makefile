SRCS20:=decltype_ut.cpp concept_ut.cpp  \
        if_switch_init_ut.cpp

SRCS:=\
    aaa.cpp hidden_friend_ut.cpp \
    deep_shallow_copy_ut.cpp func_try_block.cpp flold_expression_ut.cpp \
    generic_lambda_ut.cpp lambda.cpp \
    name_hiding.cpp name_lookup_adl_ut.cpp name_lookup_ut.cpp namespace_ut.cpp \
    range_for_ut.cpp ref_collapsing_ut.cpp \
    semantics_ut.cpp slice_ut.cpp \
    decltype_expression_ut.cpp two_phase_name_lookup_ut.cpp \
    constexpr_if_ut.cpp \
    dominance_ut.cpp ref_qualifiers_ut.cpp \
    shared_ptr_ownership_ut.cpp shared_ptr_cycle_ut.cpp weak_ptr_ut.cpp \
    diamond_inheritance_ut.cpp virtual_inheritance_ut.cpp type_traits_ut.cpp \
    structured_binding_ut.cpp dangling_ut.cpp utility_ut.cpp \
    attr_ut.cpp template_ut.cpp noexcept_ut.cpp \
    transient_lambda_ut.cpp deduction_guide_ut.cpp \
    rule_of_zero_ut.cpp optional_ut.cpp variant_ut.cpp container_ut.cpp thread_ut.cpp lock_ownership_wrapper_ut.cpp \

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
