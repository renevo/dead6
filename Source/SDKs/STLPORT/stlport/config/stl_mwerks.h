// STLport configuration file
// It is internal STLport header - DO NOT include it directly

// Bring in definition of __MSL__ and related items
#include <mslGlobals.h>
#include <ansi_parms.h>

//
//  Compiler features
//


// *** CodeWarrior Compiler Common Features ***
#  if __option(longlong)
#   define _STLP_LONG_LONG  long long
#  endif

#  define _STLP_USE_UNIX_EMULATION_IO  1

#  define _STLP_USE_AUTO_PTR_CONVERSIONS  1

# ifdef __INTEL__
#  define _STLP_LITTLE_ENDIAN
# else
#  define _STLP_BIG_ENDIAN
# endif

#if defined(_MSL_NO_LOCALE)
#define _STLP_NO_IMPORT_LOCALE
#endif
#if !__option( wchar_type )
# define _STLP_WCHAR_T_IS_USHORT
#endif

#  if __MWERKS__ < 0x3000
// *** CodeWarrior Compiler Common Bugs ***
#   define __MSL_FIX_ITERATORS__(myType)    // Some MSL headers rely on this
#   define _STLP_NO_FRIEND_TEMPLATES 1  // Bug mysteriously reintroduced in this version.
#   define _STLP_THROW_RETURN_BUG  1
#  endif

//  *** Version-specific settings ***

#  if __MWERKS__ >= 0x2405
#   define _STLP_HAS_NATIVE_FLOAT_ABS
#  endif

#  if __MWERKS__ < 0x2405
#   define _STLP_STATIC_CONST_INIT_BUG
#  endif

#  if __MWERKS__ <= 0x2303
#   define _STLP_NO_TEMPLATE_CONVERSIONS  1
#   define _STLP_NO_MEMBER_TEMPLATE_KEYWORD  1
#  endif

#  if __MWERKS__ < 0x2301
#   define _STLP_MEMBER_SPECIALIZATION_BUG  1
#  endif

#  if __MWERKS__ < 0x2300    // CW Pro5 features
#   define _STLP_INLINE_MEMBER_TEMPLATES 1
#   define _STLP_RELOPS_IN_STD_BUG   1
#   define _STLP_DEF_CONST_PLCT_NEW_BUG 1
#   define _STLP_DEF_CONST_DEF_PARAM_BUG 1
#   define _STLP_NO_TYPENAME_ON_RETURN_TYPE
#  endif

#  if __MWERKS__ < 0x2200    // CW Pro4 features
#   define _STLP_BROKEN_USING_DIRECTIVE  1
#   define _STLP_NO_MEMBER_TEMPLATES 1
#   define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#   define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
#   define _STLP_NO_FRIEND_TEMPLATES 1
#   define _STLP_NO_QUALIFIED_FRIENDS 1
#   define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  endif

#  if __MWERKS__ < 0x2100      // CW Pro3 features
#   define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#   define _STLP_HAS_NO_NAMESPACES 1
#   define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS 1

#   define _STLP_NEED_TYPENAME        1
#   define _STLP_NO_ARROW_OPERATOR 1
#   define _STLP_TEMPLATE_PARAM_SUBTYPE_BUG  1
#   define _STLP_FORCED_INLINE_INSTANTIATION_BUG  1
#   define _STLP_MULTI_CONST_TEMPLATE_ARG_BUG  1
#   define _STLP_INLINE_NAME_RESOLUTION_BUG  1
// *** Metrowerks Standard Library Bug ***
#   define _STLP_MSVC50_COMPATIBILITY 1
#  endif

#  if __MWERKS__ < 0x2000      // v. 2.0 features
#   define _STLP_NO_WCHAR_T 1
#   define _STLP_NO_DEFAULT_NON_TYPE_PARAM 1
#   define _STLP_NON_TYPE_TMPL_PARAM_BUG  1  // dwa 8/21/97 - this bug fixed for CWPro2
#   define _STLP_UNINITIALIZABLE_PRIVATE  1    // dwa 10/23/97 - this bug fixed for CWPro2
#  endif

#  if __MWERKS__ < 0x1900                 // dwa 8/19/97 - 1.9 Compiler feature defines
#   define _STLP_LIMITED_DEFAULT_TEMPLATES 1
#   define _STLP_BASE_TYPEDEF_BUG        1
#   define _STLP_BASE_MATCH_BUG   1
#   define _STLP_NONTEMPL_BASE_MATCH_BUG 1
#   define _STLP_DEFAULT_TYPE_PARAM  1      // More limited template parameters

#   if __MWERKS__ < 0x1800
    __GIVE_UP_WITH_STL(CW_18)
#   endif

#  endif


// fixes to native inclusion wrappers. 
# if __MWERKS__ >= 0x2300  // CWPro5 changes paths - dwa 2/28/99
#  ifdef N_PLAT_NLM
#   define _STLP_NATIVE_INCLUDE_PATH  ../nwsdk/include/nlm
#   define _STLP_NATIVE_C_INCLUDE_PATH  ../nwsdk/include/nlm
#   define _STLP_NATIVE_HEADER(header)     <../nwsdk/include/nlm/##header>  
#   define _STLP_NATIVE_CPP_C_HEADER(header)     <../Libraries/MSL C++/Include/##header>
#   define _STLP_NATIVE_C_HEADER(header)     <../nwsdk/include/nlm/##header>
#   define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../Libraries/MSL C++/Include/##header>

#   define _STLP_VENDOR_GLOBAL_CSTD  1
#   define _STLP_NO_VENDOR_STDLIB_L  1
#   define _STLP_NO_VENDOR_MATH_F    1
#   define _STLP_NO_VENDOR_MATH_L    1
     //#   define _MSL_NO_THROW_SPECS
     //#   define _STD
     //#   define _CSTD 

#  else
#  define _STLP_NATIVE_INCLUDE_PATH  ../include
#  define _STLP_NATIVE_C_INCLUDE_PATH  ../include
#  define _STLP_NATIVE_HEADER(header)     <../include/##header>  
#  define _STLP_NATIVE_CPP_C_HEADER(header)     <../include/##header>
#  define _STLP_NATIVE_C_HEADER(header)     <../include/##header>
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../include/##header>
#  endif

# else

#  define _STLP_NATIVE_INCLUDE_PATH  Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C++:Include
#  define _STLP_NATIVE_C_INCLUDE_PATH  Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C:MSL Common:Public Includes
#  define _STLP_NATIVE_HEADER(header)     <Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C++:Include:##header>
#  define _STLP_NATIVE_CPP_C_HEADER(header)     <Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C++:Include:##header>
#  define _STLP_NATIVE_C_HEADER(header)     <Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C:MSL Common:Public Includes:##header>
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <Macintosh HD:Codewarrior Pro 4:Metrowerks CodeWarrior:Metrowerks Standard Library:MSL C:MSL Common:Public Includes:##header>

# endif

     // fbp
# if !defined( __MSL_CPP__ ) || __MSL_CPP__ <= 0x4105
#   define _STLP_NO_NATIVE_WIDE_STREAMS 1
#  endif

# if defined(__MACH__)
#  define _STLP_MAC
#  define O_BINARY 0
# elif defined(macintosh)
#  define _NOTHREADS
# endif
# define _STLP_DLLEXPORT_NEEDS_PREDECLARATION 1
