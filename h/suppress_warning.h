#pragma once

#include "suppress_warning_clang.h"
#include "suppress_warning_gcc.h"

#if (!defined(SUPPRESS_WARN_OFF)) && defined(__clang__)

#define SUPPRESS_WARN_BEGIN SUPPRESS_WARN_CLANG_BEGIN
#define SUPPRESS_WARN_END SUPPRESS_WARN_CLANG_END

#define SUPPRESS_WARN_ADDRESS SUPPRESS_WARN_CLANG_ADDRESS
#define SUPPRESS_WARN_ARRAY_BOUNDS SUPPRESS_WARN_CLANG_ARRAY_BOUNDS
#define SUPPRESS_WARN_COMP SUPPRESS_WARN_CLANG_COMP
#define SUPPRESS_WARN_COMP_SWITCH SUPPRESS_WARN_CLANG_ENUM_COMP_SWITCH
#define SUPPRESS_WARN_COMMENT SUPPRESS_WARN_CLANG_COMMENT
#define SUPPRESS_WARN_CONV_NULL SUPPRESS_WARN_CLANG_CONV_NULL
#define SUPPRESS_WARN_DEL_INCOMP SUPPRESS_WARN_CLANG_DEL_INCOMP
#define SUPPRESS_WARN_EMPTY_BODY SUPPRESS_WARN_CLANG_EMPTY_BODY
#define SUPPRESS_WARN_FORMAT_SEC SUPPRESS_WARN_CLANG_FORMAT_SEC
#define SUPPRESS_WARN_IGNORE_QUAL SUPPRESS_WARN_CLANG_IGNORE_QUAL
#define SUPPRESS_WARN_MISLEADING_INDENT SUPPRESS_WARN_CLANG_MISLEADING_INDENT
#define SUPPRESS_WARN_DEPRECATED_COPY SUPPRESS_WARN_CLANG_DEPRECATED_COPY
#define SUPPRESS_WARN_DEPRECATED_DECL SUPPRESS_WARN_CLANG_DEPRECATED_DECL
#define SUPPRESS_WARN_IMPLICIT_INT SUPPRESS_WARN_CLANG_IMPLICIT_INT
#define SUPPRESS_WARN_INIT_OVERRIDE SUPPRESS_WARN_CLANG_INIT_OVERRIDE
#define SUPPRESS_WARN_MISSING_BRACES SUPPRESS_WARN_CLANG_MISSING_BRACES
#define SUPPRESS_WARN_NON_VIRTUAL_DTOR SUPPRESS_WARN_CLANG_NON_VIRTUAL_DTOR
#define SUPPRESS_WARN_PARENTHESES SUPPRESS_WARN_CLANG_PARENTHESES
#define SUPPRESS_WARN_PESSIMIZING_MOVE SUPPRESS_WARN_CLANG_PESSIMIZING_MOVE
#define SUPPRESS_WARN_SHIFT_COUNT_OVERFLOW SUPPRESS_WARN_CLANG_SHIFT_COUNT_OVERFLOW
#define SUPPRESS_WARN_RETURN_TYPE SUPPRESS_WARN_CLANG_RETURN_TYPE
#define SUPPRESS_WARN_SIGN_COMPARE SUPPRESS_WARN_CLANG_SIGN_COMPARE
#define SUPPRESS_WARN_SIZEOF_ARRAY_ARG SUPPRESS_WARN_CLANG_SIZEOF_ARRAY_ARG
#define SUPPRESS_WARN_SIZEOF_POINTER_DIV SUPPRESS_WARN_CLANG_SIZEOF_POINTER_DIV
#define SUPPRESS_WARN_UNINIT SUPPRESS_WARN_CLANG_UNINIT
#define SUPPRESS_WARN_UNUSED_FUNC SUPPRESS_WARN_CLANG_UNUSED_FUNC
#define SUPPRESS_WARN_UNUSED_LOCAL_TYPEDEF SUPPRESS_WARN_CLANG_UNUSED_LOCAL_TYPEDEF
#define SUPPRESS_WARN_UNUSED_LABEL SUPPRESS_WARN_CLANG_UNUSED_LABEL
#define SUPPRESS_WARN_UNUSED_PARAM SUPPRESS_WARN_CLANG_UNUSED_PARAM
#define SUPPRESS_WARN_UNUSED_RESULT SUPPRESS_WARN_CLANG_UNUSED_RESULT
#define SUPPRESS_WARN_UNUSED_VAL SUPPRESS_WARN_CLANG_UNUSED_VAL
#define SUPPRESS_WARN_UNUSED_VAR SUPPRESS_WARN_CLANG_UNUSED_VAR
#define SUPPRESS_WARN_USER_DEFINED_LITERAL SUPPRESS_WARN_CLANG_USER_DEFINED_LITERAL
#define SUPPRESS_WARN_SWITCH SUPPRESS_WARN_CLANG_SWITCH
#define SUPPRESS_WARN_CONVERSION_VEXING_PARSE SUPPRESS_WARN_CLANG_CONVERSION_VEXING_PARSE

#elif (!defined(SUPPRESS_WARN_OFF)) && defined(__GNUC__)

#define SUPPRESS_WARN_BEGIN SUPPRESS_WARN_GCC_BEGIN
#define SUPPRESS_WARN_END SUPPRESS_WARN_GCC_END

#define SUPPRESS_WARN_ADDRESS SUPPRESS_WARN_GCC_ADDRESS
#define SUPPRESS_WARN_ARRAY_BOUNDS SUPPRESS_WARN_GCC_ARRAY_BOUNDS
#define SUPPRESS_WARN_COMP SUPPRESS_WARN_GCC_COMP
#define SUPPRESS_WARN_COMP_SWITCH SUPPRESS_WARN_GCC_ENUM_COMP_SWITCH
#define SUPPRESS_WARN_COMMENT SUPPRESS_WARN_GCC_COMMENT
#define SUPPRESS_WARN_CONV_NULL SUPPRESS_WARN_GCC_CONV_NULL
#define SUPPRESS_WARN_DEL_INCOMP SUPPRESS_WARN_GCC_DEL_INCOMP
#define SUPPRESS_WARN_EMPTY_BODY SUPPRESS_WARN_GCC_EMPTY_BODY
#define SUPPRESS_WARN_FORMAT_SEC SUPPRESS_WARN_GCC_FORMAT_SEC
#define SUPPRESS_WARN_IGNORE_QUAL SUPPRESS_WARN_GCC_IGNORE_QUAL
#define SUPPRESS_WARN_MISLEADING_INDENT SUPPRESS_WARN_GCC_MISLEADING_INDENT
#define SUPPRESS_WARN_DEPRECATED_COPY SUPPRESS_WARN_GCC_DEPRECATED_COPY
#define SUPPRESS_WARN_DEPRECATED_DECL SUPPRESS_WARN_GCC_DEPRECATED_DECL
#define SUPPRESS_WARN_IMPLICIT_INT SUPPRESS_WARN_GCC_IMPLICIT_INT
#define SUPPRESS_WARN_INIT_OVERRIDE SUPPRESS_WARN_GCC_INIT_OVERRIDE
#define SUPPRESS_WARN_MISSING_BRACES SUPPRESS_WARN_GCC_MISSING_BRACES
#define SUPPRESS_WARN_NON_VIRTUAL_DTOR SUPPRESS_WARN_GCC_NON_VIRTUAL_DTOR
#define SUPPRESS_WARN_PARENTHESES SUPPRESS_WARN_GCC_PARENTHESES
#define SUPPRESS_WARN_PESSIMIZING_MOVE SUPPRESS_WARN_GCC_PESSIMIZING_MOVE
#define SUPPRESS_WARN_SHIFT_COUNT_OVERFLOW SUPPRESS_WARN_GCC_SHIFT_COUNT_OVERFLOW
#define SUPPRESS_WARN_RETURN_TYPE SUPPRESS_WARN_GCC_RETURN_TYPE
#define SUPPRESS_WARN_SIGN_COMPARE SUPPRESS_WARN_GCC_SIGN_COMPARE
#define SUPPRESS_WARN_SIZEOF_ARRAY_ARG SUPPRESS_WARN_GCC_SIZEOF_ARRAY_ARG
#define SUPPRESS_WARN_SIZEOF_POINTER_DIV SUPPRESS_WARN_GCC_SIZEOF_POINTER_DIV
#define SUPPRESS_WARN_UNINIT SUPPRESS_WARN_GCC_UNINIT
#define SUPPRESS_WARN_UNUSED_FUNC SUPPRESS_WARN_GCC_UNUSED_FUNC
#define SUPPRESS_WARN_UNUSED_LOCAL_TYPEDEF SUPPRESS_WARN_GCC_UNUSED_LOCAL_TYPEDEF
#define SUPPRESS_WARN_UNUSED_LABEL SUPPRESS_WARN_GCC_UNUSED_LABEL
#define SUPPRESS_WARN_UNUSED_PARAM SUPPRESS_WARN_GCC_UNUSED_PARAM
#define SUPPRESS_WARN_UNUSED_RESULT SUPPRESS_WARN_GCC_UNUSED_RESULT
#define SUPPRESS_WARN_UNUSED_VAL SUPPRESS_WARN_GCC_UNUSED_VAL
#define SUPPRESS_WARN_UNUSED_VAR SUPPRESS_WARN_GCC_UNUSED_VAR
#define SUPPRESS_WARN_USER_DEFINED_LITERAL SUPPRESS_WARN_GCC_USER_DEFINED_LITERAL
#define SUPPRESS_WARN_SWITCH SUPPRESS_WARN_GCC_SWITCH
#define SUPPRESS_WARN_CONVERSION_VEXING_PARSE SUPPRESS_WARN_GCC_CONVERSION_VEXING_PARSE

#else

#define SUPPRESS_WARN_BEGIN
#define SUPPRESS_WARN_END

#define SUPPRESS_WARN_ADDRESS
#define SUPPRESS_WARN_COMMENT
#define SUPPRESS_WARN_CONV_NULL
#define SUPPRESS_WARN_DEL_INCOMP
#define SUPPRESS_WARN_EMPTY_BODY
#define SUPPRESS_WARN_FORMAT_SEC
#define SUPPRESS_WARN_IGNORE_QUAL
#define SUPPRESS_WARN_MISLEADING_INDENT
#define SUPPRESS_WARN_DEPRECATED_COPY
#define SUPPRESS_WARN_DEPRECATED_DECL
#define SUPPRESS_WARN_IMPLICIT_INT
#define SUPPRESS_WARN_INIT_OVERRIDE
#define SUPPRESS_WARN_MISSING_BRACES
#define SUPPRESS_WARN_NON_VIRTUAL_DTOR
#define SUPPRESS_WARN_PARENTHESES
#define SUPPRESS_WARN_PESSIMIZING_MOVE
#define SUPPRESS_WARN_RETURN_TYPE
#define SUPPRESS_WARN_SIGN_COMPARE
#define SUPPRESS_WARN_SIZEOF_ARRAY_ARG
#define SUPPRESS_WARN_SIZEOF_POINTER_DIV
#define SUPPRESS_WARN_UNINIT
#define SUPPRESS_WARN_UNUSED_FUNC
#define SUPPRESS_WARN_UNUSED_LABEL
#define SUPPRESS_WARN_UNUSED_PARAM
#define SUPPRESS_WARN_UNUSED_RESULT
#define SUPPRESS_WARN_UNUSED_VAR
#define SUPPRESS_WARN_USER_DEFINED_LITERAL
#define SUPPRESS_WARN_SWITCH
#define SUPPRESS_WARN_CONVERSION_VEXING_PARSE

#endif

#ifdef __cplusplus
template <typename... ARGS>
inline void IGNORE_UNUSED_VAR(ARGS&&...) noexcept
{
}
#else
#define IGNORE_UNUSED_VAR(v__) (void)v__
#endif
