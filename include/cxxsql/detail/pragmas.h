#pragma once

#if !defined(__cplusplus)
#error "C++ heaer"
#endif

#ifndef PRAGMAS_H
#define PRAGMAS_H



#include <cstdarg>

//-------------------------------------------------------------------
// Macros with compiler name [gcc,msvc] are recognised on specified compiler only
// For gcc compatiblity read explanation at bottom of file, hoever when not supported it could be tuned what to do in that case
//
//
// attribute_align(a)
//
// PRAGMA_GCC_PACK_PUSH( n )
// PRAGMA_GCC_PACK_POP
// PRAGMA_MSC_PACK_PUSH( n )
// PRAGMA_MSC_PACK_POP
// PRAGMA_PACK_PUSH( n )
// PRAGMA_PACK_POP

// PRAGMA_GCC_WARNING_PUSH - gcc 4.6+
// PRAGMA_GCC_WARNING_POP  - gcc 4.6+
// PRAGMA_GCC_WARNING_OFF(-Wwarning_name) - gcc 4.2+
// PRAGMA_GCC_WARNING_ON(-Wwarning_name) - gcc 4.2+
// PRAGMA_GCC_WARNING_PUSH_OFF - gcc 4.6+ (works as PUSH + OFF needs POP)
// PRAGMA_MSC_WARNING_PUSH
// PRAGMA_MSC_WARNING_POP
// PRAGMA_MSC_WARNING_OFF( id )
// PRAGMA_MSC_WARNING_SUPPRESS( id )
// PRAGMA_MSC_WARNING_DEFAULT( id )
//
// TODO(x) - msvc, gcc 4.4+
// MESSAGE(x) - msvc, gcc 4.4+
// WARNING(x) - msvc, gcc 4.4+
//
// PRAGMA_WARNING_PUSH - msvc, gcc 4.6+
// PRAGMA_WARNING_POP  - msvc, gcc 4.6+
//
// PUSH_MACRO(macro_name) - msvc, gcc 4.4+
// POP_MACRO(macro_name) - msvc, gcc 4.4+
//
// attribute_pure
// attribute_const
// DEPRECATED(a)
//
// ATTRIBUTE_EXPORT, DLL_IMPORT, DLL_LOCAL_DEF, SO_LOCAL, DLL_LOCAL
//
//-------------------------------------------------------------------
#define STRX(x) #x
#define STR(x) STRX(x)
#define WSTR(x) L# x

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define WFILE_ WIDEN(__FILE__)
#define WLINE_ WIDEN(__LINE__)
#define WDATE_ WIDEN(__DATE__)
#define WTIME_ WIDEN(__TIME__)
#if defined(__MINGW32__) || defined(__MINGW64__)
   #define __USE_MINGW_ANSI_STDIO 1
#endif
#if defined(__GNUC__)
//    #define __FUNCTION__ __PRETTY_FUNCTION__
   #define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)
#endif

#if defined(__GNUC__)
  #define restrict __restrict__ 
//   #define ATTRIBUTE_GCC(a) __attribute__((a))
  #define ATTRIBUTE_GCC(...) __attribute__((__VA_ARGS__))
  #define attribute_gcc __attribute__
  #define ATTRIBUTE_FORMAT_FN(m,n)    __attribute__((format(printf,m,n)))
  #define ATTRIBUTE_GCC_ALIGN(a) __attribute__((aligned(a)))
#else
  #define ATTRIBUTE_GCC(a)
  #define attribute_gcc(a)
  #define ATTRIBUTE_FORMAT_FN(m,n)
  #define ATTRIBUTE_GCC_ALIGN(a)
#endif


  
#if defined(__GNUC__)
   #define LIB_CONSTRUCTOR_API __attribute__ ((constructor))
   #define LIB_DESTRUCTOR_API __attribute__ ((destructor))
#else
   #define LIB_CONSTRUCTOR_API
   #define LIB_DESTRUCTOR_API
#endif

#if defined(__clang__)
   #define CXX_VERSION_STRING L"clang " WIDEN(__clang_version__)
   #define CXX_VERSION_STRING_CHAR "clang " __clang_version__
#elif defined(__MINGW64__)
	#define CXX_VERSION_STRING L"MINGW64 " WIDEN(__MINGW64_VERSION_STR) L" GCC " WIDEN(STRINGIZE(__GNUC__)) L"." WIDEN(STRINGIZE(__GNUC_MINOR__) L"." WIDEN(STRINGIZE(__GNUC_PATCHLEVEL__)))
	#define CXX_VERSION_STRING_CHAR "MINGW64 " __MINGW64_VERSION_STR " GCC " STRINGIZE(__GNUC__) "." STRINGIZE(__GNUC_MINOR__) "." STRINGIZE(__GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
	#define CXX_VERSION_STRING L"GCC " WIDEN(STRINGIZE(__GNUC__)) L"." WIDEN(STRINGIZE(__GNUC_MINOR__) L"." WIDEN(STRINGIZE(__GNUC_PATCHLEVEL__)))
	#define CXX_VERSION_STRING_CHAR "GCC " STRINGIZE(__GNUC__) "." STRINGIZE(__GNUC_MINOR__) "." STRINGIZE(__GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
	#define CXX_VERSION_STRING L"MSC " WIDEN(STRINGIZE(_MSC_VER))
	#define CXX_VERSION_STRING_CHAR "MSC " STRINGIZE(_MSC_VER)
#else
	#error "Unknown CXX"
#endif

#if __SIZEOF_PTRDIFF_T__ == 8
			#define CPU_ARCH_STRING L"64bit"
#elif __SIZEOF_PTRDIFF_T__ == 4
			#define CPU_ARCH_STRING L"32bit"
#else
	#error "Unknown architecture"
#endif

#if defined(_MSC_VER)
	#pragma warning ( disable : 4100 ) //unreferenced formal parameter
	#pragma warning ( disable : 4481 ) //nonstandard extension used
	#pragma warning ( disable : 4505 ) //unreferenced local function has been removed
	#pragma warning(disable:4482) // nonstandard extension used
	#pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
	#pragma warning(disable:4748) // /GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function
#endif



#ifndef _MSC_VER
	#define STR2WSTR2_(str)    L##str
	#define STR2WSTR_(str)     STR2WSTR2_(str)
	#define FILEW_          STR2WSTR_(__FILE__)
	#define FUNCTIONW_      STR2WSTR_(__FUNCTION__)
#endif

#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define WFILE_ WIDEN(__FILE__)
#define WDATE_ WIDEN(__DATE__)


#if defined(__GNUC__)
	#define DEPRECATED(a) __attribute__((__deprecated__))
#elif defined(_MSC_VER)
	#define DEPRECATED(a) __declspec( deprecated(a))
#else
	#define DEPRECATED(a)
#endif

#if defined _WIN32 || defined __CYGWIN__

    #ifdef __GNUC__
      #define ATTRIBUTE_EXPORT __attribute__ ((dllexport))
      #define DLL_IMPORT __attribute__ ((dllimport))
      #define ATTRIBUTE_EXPORT_DECL ATTRIBUTE_EXPORT
      #define ATTRIBUTE_IMPORT_DECL DLL_IMPORT
    #else
      #define ATTRIBUTE_EXPORT __declspec(dllexport)
      #define DLL_IMPORT __declspec(dllimport)
      #define ATTRIBUTE_EXPORT_DECL ATTRIBUTE_EXPORT
      #define ATTRIBUTE_IMPORT_DECL DLL_IMPORT
    #endif
  #define DLL_LOCAL_DEF
#else
    #define ATTRIBUTE_EXPORT __attribute__ ((visibility ("default")))
    #define ATTRIBUTE_EXPORT_DECL
    #define ATTRIBUTE_IMPORT_DECL
	 #define DLL_IMPORT __attribute__ ((visibility ("default")))
    #define DLL_LOCAL_DEF  __attribute__ ((visibility ("hidden")))
#endif


#define DLL_LOCAL DLL_LOCAL_DEF
#define SO_LOCAL DLL_LOCAL_DEF

#if defined(_MSC_VER)
	#define PRAGMA_MSC_WARNING_PUSH __pragma( warning (push) )
	#define PRAGMA_MSC_WARNING_POP __pragma( warning (pop) )
	#define PRAGMA_MSC_WARNING_OFF( id ) __pragma( warning(disable : id) )
	#define PRAGMA_MSC_WARNING_DEFAULT( id ) __pragma( warning(default : id) )
	#define PRAGMA_MSC_WARNING_SUPPRESS( id ) __pragma( warning (suppress : id) )
	#define PRAGMA_WARNING_PUSH PRAGMA_MSC_WARNING_PUSH
	#define PRAGMA_WARNING_POP PRAGMA_MSC_WARNING_POP
	#define TODO(x) __pragma(message ("TODO - " x))
	#define WARNING(x) __pragma(message ("WARNING - " x))
	#if !defined(DISABLE_MESSAGES)
		#define MESSAGE(x) __pragma(message(" - " x))
	#else
		#define MESSAGE(x)
	#endif
	#define PUSH_MACRO(x) __pragma( push_macro ( # x ) )
	#define POP_MACRO(x) __pragma( pop_macro ( # x ) )
#else
	#define PRAGMA_MSC_WARNING_PUSH
	#define PRAGMA_MSC_WARNING_POP
	#define PRAGMA_MSC_WARNING_OFF( id )
	#define PRAGMA_MSC_WARNING_SUPPRESS( id )
#endif


//
// Explanation
// GCC support pragmas as follow
// 4.0 ---
// 4.2 diagnostic[ warning error ignored]
// 4.3 diagnostic[ warning error ignored]
// 4.4 diagnostic[ warning error ignored]          push_macro(X) pop_macro(X) message
// 4.5 diagnostic[ warning error ignored]          push_macro(X) pop_macro(X) message
// 4.6 diagnostic[ warning error ignored push pop] push_macro(X) pop_macro(X) message

//On different platforms there are used different versions of gcc

// Android NDK, Revision 1 gcc-4.2.1
// Android NDK, Revision 2
// Android NDK, Revision 3 gcc-4.4.0
// Android NDK, Revision 4b
// Android NDK, Revision 5 gcc-4.4.3
// Android NDK, Revision 5b
// Android NDK, Revision 6 gcc-4.4.3
// Android NDK, Revision 7 ????
// Bada 1.0 gcc-4.4.1
// Bada 1.2 gcc-4.4.1
// Bada 2.0 gcc-4.5.3

// Bada supports allways a diagnostic[ warning error ignored] push_macro(X) pop_macro(X) message
// Android supports allways only diagnostic[ warning error ignored]

//To enable use of supported functionality of gcc
//With gcc 4.0 up to gcc 4.1 definitions will issue an error
//With gcc 4.2 up to gcc 4.3 definitions will issue an warning about lack of macro push_macro(X) pop_macro(X) and diagnostic push pop
//                           but will allow to tune it as follows:
//                               with -DPRAGMA_GCC43_MACRO_PUSH_SUPPRESS will stop issuing warning and define PUSH_MACRO and POP_MACRO as empty statments
//                                                                       this will cause global scope of any changes to macros betwen POP and PUSH
//                               also will allow below tunes
//With gcc 4.4 up to gcc 4.5 definitions will issue an warning about lack of diagnostic push pop
//                           but will allow to tune it as follows:
//                               with -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS will stop issuing warning and define PRAGMA_GCC_WARNING_PUSH and PRAGMA_GCC_WARNING_POP as empty statments
//                               with -DPRAGMA_GCC_WARNING_ON_DISABLE will remove any use of PRAGMA_GCC_WARNING_ON(name) statments from preprocessed code
//                               with -DPRAGMA_GCC_WARNING_OFF_DISABLE will remove any use of PRAGMA_GCC_WARNING_OFF(name) statments from preprocessed code
//                               with -DPRAGMA_GCC_WARNING_AS_ERROR_DISABLE will remove any use of PRAGMA_GCC_WARNING_AS_ERROR(name) statments from preprocessed code
//For example safest solution for gcc 4.2 will be
//       -DPRAGMA_GCC43_MACRO_PUSH_SUPPRESS -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS -DPRAGMA_GCC_WARNING_OFF_DISABLE
//            most silent solution for gcc 4.2 will be
//       -DPRAGMA_GCC43_MACRO_PUSH_SUPPRESS -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS -DPRAGMA_GCC_WARNING_OFF_DISABLE -DPRAGMA_GCC_WARNING_AS_ERROR_DISABLE -DPRAGMA_GCC_WARNING_ON_DISABLE
//there are possible other solutions, for example chainging off statments into warnings i'm open, feel free to constact Artur
//Belows macros were tested on kubuntu linux and freebsd9 with gcc 4.2, 4.4, 4.5, 4.6

//Bear in mind that not all pragmas can be switched to errors or controled
//From manual
//Modifies the disposition of a diagnostic. Note that not all diagnostics are modifiable; at the moment only warnings (normally controlled by `-W...')
//can be controlled, and not all of them.
//gcc42 requires also to put all macro definitions of diagnostics outside functions


  #if defined(__GNUC__) && !defined(__clang__)
    #define GCC_DO_PRAGMA(x) _Pragma (#x)

    //diagnostic[ warning error ignored]
    #define PRAGMA_GCC_WARNING_OFF(name) 		GCC_DO_PRAGMA(GCC diagnostic ignored #name)
    #define PRAGMA_GCC_WARNING_ON(name) 		GCC_DO_PRAGMA(GCC diagnostic warning #name)

    #if !defined(DISABLE_PRAGMA_MESSAGE)
          #define TODO(x) GCC_DO_PRAGMA( message ( "TODO - " #x) )
          #define WARNING(x) GCC_DO_PRAGMA( message ( "WARNING - " #x) )
          #define MESSAGE(x) GCC_DO_PRAGMA( message (#x) )
    #else
          #define TODO(x)
          #define WARNING(x)
          #define MESSAGE(x)
    #endif
    #define PUSH_MACRO(x) GCC_DO_PRAGMA( push_macro (#x) )
    #define POP_MACRO(x) GCC_DO_PRAGMA( pop_macro (#x) )

    #define PRAGMA_GCC_WARNING_PUSH GCC_DO_PRAGMA(GCC diagnostic push)
    #define PRAGMA_GCC_WARNING_POP GCC_DO_PRAGMA(GCC diagnostic pop)

    #define PRAGMA_GCC_WARNING_PUSH_OFF(name) PRAGMA_GCC_WARNING_PUSH PRAGMA_GCC_WARNING_OFF(name)
    #define PRAGMA_WARNING_PUSH PRAGMA_GCC_WARNING_PUSH
    #define PRAGMA_WARNING_POP PRAGMA_GCC_WARNING_POP
    #define PRAGMA_WARNING_PUSH_OFF(name) PRAGMA_GCC_WARNING_PUSH_OFF(name)
  #else
    #define PRAGMA_GCC_WARNING_PUSH
    #define PRAGMA_GCC_WARNING_POP
    #define PRAGMA_GCC_WARNING_ON(name)
    #define PRAGMA_GCC_WARNING_OFF(name)
    #define PRAGMA_GCC_WARNING_PUSH_OFF(name)
  #endif

#if defined(__clang__)
  #pragma clang diagnostic ignored "-Wunused-parameter"
  
  #define CLANG_DO_PRAGMA(x) _Pragma (#x)
  #define GCC_DO_PRAGMA(x) _Pragma (#x)
  #define PRAGMA_CLANG_WARNING_OFF(name)    CLANG_DO_PRAGMA(clang diagnostic ignored #name)
  #define PRAGMA_CLANG_WARNING_ON(name)     CLANG_DO_PRAGMA(clang diagnostic warning #name)
  #define TODO(x) CLANG_DO_PRAGMA( message ( "TODO - "  x) )
  #define WARNING(x) CLANG_DO_PRAGMA( message ( "WARNING - "  x) )
  #define MESSAGE(x) CLANG_DO_PRAGMA( message ( x ) )
  #define PUSH_MACRO(x) CLANG_DO_PRAGMA( push_macro ( #x ) )
  #define POP_MACRO(x) CLANG_DO_PRAGMA( pop_macro ( #x ) )
  #define PRAGMA_CLANG_WARNING_PUSH GCC_DO_PRAGMA(clang diagnostic push)
  #define PRAGMA_CLANG_WARNING_POP GCC_DO_PRAGMA(clang diagnostic pop)
  #define PRAGMA_CLANG_WARNING_PUSH_OFF(name) PRAGMA_CLANG_WARNING_PUSH PRAGMA_CLANG_WARNING_OFF(name)
  #define PRAGMA_WARNING_PUSH PRAGMA_CLANG_WARNING_PUSH
  #define PRAGMA_WARNING_POP PRAGMA_CLANG_WARNING_POP
  #define PRAGMA_WARNING_PUSH_OFF(name) PRAGMA_CLANG_WARNING_PUSH_OFF(name)
#else
  #define PRAGMA_CLANG_WARNING_PUSH 
  #define PRAGMA_CLANG_WARNING_POP
  #define PRAGMA_CLANG_WARNING_ON(name)
  #define PRAGMA_CLANG_WARNING_OFF(name)
  #define PRAGMA_CLANG_WARNING_PUSH_OFF(name)
#endif

#if defined(__GNUC__) 
	#define attribute_pure __attribute__((pure))
	#define attribute_const __attribute__((const))

	//http://gcc.gnu.org/onlinedocs/gcc-4.2.4/gcc/Structure_002dPacking-Pragmas.html#Structure_002dPacking-Pragmas

	#define PRAGMA_GCC_PACK_PUSH( n ) GCC_DO_PRAGMA( pack ( push, n ) )
	#define PRAGMA_GCC_PACK_POP GCC_DO_PRAGMA( pack ( pop ) )

	#define PRAGMA_PACK_PUSH( n ) PRAGMA_GCC_PACK_PUSH( n )
	#define PRAGMA_PACK_POP PRAGMA_GCC_PACK_POP

	#define attribute_align(a) __attribute__((aligned(a)))
#else
	#define attribute_pure
	#define attribute_const
#endif

#if defined(_MSC_VER)
//http://msdn.microsoft.com/en-us/library/aa273913(v=VS.60).aspx
	#define PRAGMA_MSC_PACK_PUSH( n ) __pragma( pack ( push, n ) )
	#define PRAGMA_MSC_PACK_POP __pragma( pack ( pop ) )
	#define PRAGMA_PACK_PUSH( n ) PRAGMA_MSC_PACK_PUSH( n )
	#define PRAGMA_PACK_POP PRAGMA_MSC_PACK_POP
	#define attribute_align(a) __declspec(align(a))
#endif

#define ATTRIBUTE_ALIGN(a) attribute_align(a)

// Emample code
/*
#include <cstdio>
#warning "starting test"
#pragma message "starting test"

PRAGMA_GCC_WARNING_PUSH
PRAGMA_GCC_WARNING_ON(-Wunknown-pragmas)
#pragma region o---unknown pragma---o

PRAGMA_GCC_WARNING_POP
PRAGMA_GCC_WARNING_OFF(-Wunknown-pragmas)
#pragma region o---unknown pragma---o

PRAGMA_GCC_WARNING_ON(-Wunused-variable)
PRAGMA_GCC_WARNING_ON(-Wuninitialized)
void other2 ()
{
	int a, b;
	b = 10;
	printf("%d", a);
}

#if __GNUC_MINOR__ < 6
PRAGMA_GCC_WARNING_OFF(-Wunused-variable)
#else
PRAGMA_GCC_WARNING_OFF(-Wunused-but-set-variable)
#endif
PRAGMA_GCC_WARNING_OFF(-Wuninitialized)

int main(int argc, char ** argv)
{
	int a, b;
	b=10;
	printf("%d", a);
	return 0;
}
#if __GNUC_MINOR__ < 6
PRAGMA_GCC_WARNING_ON(-Wunused-variable)
#else
PRAGMA_GCC_WARNING_ON(-Wunused-but-set-variable)
#endif
PRAGMA_GCC_WARNING_ON(-Wuninitialized)
void other ()
{
	int a, b;
	b = 10;
	printf("%d", a);
}
*/
/*

gcc version 4.2.1 20070831 patched [FreeBSD]
gcc -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx -DPRAGMA_GCC43_MACRO_PUSH_SUPPRESS -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS
macrotest.cxx:2:2: warning: #warning "starting test"
macrotest.cxx:3: warning: ignoring #pragma message  [-Wunknown-pragmas]
macrotest.cxx:81: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85: warning: ignoring #pragma region o [-Wunknown-pragmas]

gcc version 4.4.6 (Ubuntu/Linaro 4.4.6-11ubuntu2)
gcc-4.4 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS
macrotest.cxx:2: warning: #warning "starting test"
macrotest.cxx:81: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3: note: #pragma message: starting test
macrotest.cxx: In function �void other()�:
macrotest.cxx:120: warning: �a� is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function �int main(int, char**)�:
macrotest.cxx:107: warning: �a� is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function �void other2()�:
macrotest.cxx:93: warning: �a� is used uninitialized in this function [-Wuninitialized]

gcc version 4.4.7 20111108 (prerelease) (FreeBSD Ports Collection)
gcc44 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS
macrotest.cxx:2:2: warning: #warning "starting test"
macrotest.cxx:81: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3: note: #pragma message: starting test
macrotest.cxx: In function 'void other()':
macrotest.cxx:113: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'int main(int, char**)':
macrotest.cxx:103: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'void other2()':
macrotest.cxx:93: warning: 'a' is used uninitialized in this function [-Wuninitialized]

gcc version 4.5.4 20111117 (FreeBSD Ports Collection)
gcc45 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx -DPRAGMA_GCC45_WARNING_PUSH_SUPPRESS
macrotest.cxx:2:2: warning: #warning "starting test"
macrotest.cxx:81:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3:17: note: #pragma message: starting test
macrotest.cxx: In function 'void other2()':
macrotest.cxx:93:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'int main(int, char**)':
macrotest.cxx:103:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'void other()':
macrotest.cxx:113:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]

gcc version 4.6.1 (Ubuntu/Linaro 4.6.1-9ubuntu3)
gcc-4.6 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx
macrotest.cxx:2:2: warning: #warning "starting test" [-Wcpp]
macrotest.cxx:81:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3:17: note: #pragma message: starting test
macrotest.cxx: In function �void other2()�:
macrotest.cxx:91:9: warning: variable �b� set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function �void other()�:
macrotest.cxx:115:9: warning: variable �b� set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function �void other2()�:
macrotest.cxx:93:17: warning: �a� is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function �void other()�:
macrotest.cxx:117:17: warning: �a� is used uninitialized in this function [-Wuninitialized]

gcc version 4.6.3 20111118 (FreeBSD Ports Collection)
gcc46 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx
macrotest.cxx:2:2: warning: #warning "starting test" [-Wcpp]
macrotest.cxx:81:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3:17: note: #pragma message: starting test
macrotest.cxx: In function 'void other2()':
macrotest.cxx:91:9: warning: variable 'b' set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function 'void other()':
macrotest.cxx:115:9: warning: variable 'b' set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function 'void other2()':
macrotest.cxx:93:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'void other()':
macrotest.cxx:117:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]

gcc version 4.7.0 20111105 (experimental) (FreeBSD Ports Collection)
gcc47 -fdiagnostics-show-option -Wall -Wunknown-pragmas -c macrotest.cxx
macrotest.cxx:2:2: warning: #warning "starting test" [-Wcpp]
macrotest.cxx:81:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:85:0: warning: ignoring #pragma region o [-Wunknown-pragmas]
macrotest.cxx:3:17: note: #pragma message: starting test
macrotest.cxx: In function 'void other2()':
macrotest.cxx:91:9: warning: variable 'b' set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function 'void other()':
macrotest.cxx:115:9: warning: variable 'b' set but not used [-Wunused-but-set-variable]
macrotest.cxx: In function 'void other2()':
macrotest.cxx:93:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]
macrotest.cxx: In function 'void other()':
macrotest.cxx:117:17: warning: 'a' is used uninitialized in this function [-Wuninitialized]


 */
#endif
