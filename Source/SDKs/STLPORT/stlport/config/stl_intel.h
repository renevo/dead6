// STLport configuration file
// It is internal STLport header - DO NOT include it directly

#if (__ICL >= 450)
#  define _STLP_DLLEXPORT_NEEDS_PREDECLARATION 1
#endif

#define _STLP_IMPORT_TEMPLATE_KEYWORD extern

#include <config/stl_msvc.h>

#if defined (_STLP_DONT_RETURN_VOID)
#  undef _STLP_DONT_RETURN_VOID
#endif /*_STLP_DONT_RETURN_VOID*/

#undef  _STLP_LONG_LONG
#define _STLP_LONG_LONG long long

#if (__ICL >= 800)
#  define _STLP_STATIC_CONST_INIT_BUG 1
#endif 
