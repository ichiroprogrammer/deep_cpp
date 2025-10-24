SRCS20:=literal_ut.cpp if_switch_init_ut.cpp concept_ut.cpp decltype_ut.cpp \

SRCS:=\
    integral_promotion_ut.cpp etc_ut.cpp heap_allocation_elision_ut.cpp float_ut.cpp trivial_ut.cpp \
    incomplete_type_ut.cpp class_layout_ut.cpp rtti_ut.cpp interface_class.cpp constructor_ut.cpp \
    implicit_conversion_ut.cpp undefined_ut.cpp argument.cpp aliging_ut.cpp user_defined_literal_ut.cpp nsdmi.cpp \
    rvalue_lvalue_ut.cpp rvalue_move_ut.cpp universal_ref_ut.cpp perfect_forwarding_ut.cpp \
	decltype_expression_ut.cpp ref_collapsing_ut.cpp dangling_ut.cpp utility_ut.cpp ref_qualifiers_ut.cpp \
	attr_ut.cpp func_try_block.cpp range_for_ut.cpp structured_binding_ut.cpp lambda.cpp transient_lambda_ut.cpp \
    template_ut.cpp constexpr_if_ut.cpp name_lookup_ut.cpp two_phase_name_lookup_ut.cpp name_lookup_adl_ut.cpp \
	hidden_friend_ut.cpp name_hiding.cpp diamond_inheritance_ut.cpp virtual_inheritance_ut.cpp \
	flold_expression_ut.cpp generic_lambda_ut.cpp deduction_guide_ut.cpp aaa.cpp dominance_ut.cpp namespace_ut.cpp \
    noexcept_ut.cpp type_traits_ut.cpp thread_ut.cpp lock_ownership_wrapper_ut.cpp weak_ptr_ut.cpp container_ut.cpp \
	optional_ut.cpp variant_ut.cpp \


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
