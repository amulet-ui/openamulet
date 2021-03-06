#define NEW_HEADERS

// This string indicates the version of OA
#define Am_VERSION "OpenAmulet 4.3 alpha N"

// define this if you want to use OpenAmulet, when this is defined, all of
// the changes to the original Amulet version will be enabled and used.
#define OA_VERSION

// Provide an installation default for AMULET_DIR, so users may not
// set this variable every time. One can redefine this default from
// the Makefile.vars.* to adapt to local directory structures.
#ifndef DEFAULT_AMULET_DIR
#ifdef _WIN32
#define DEFAULT_AMULET_DIR "..\\..\\data"
#else
#define DEFAULT_AMULET_DIR "/usr/local/lib/amulet"
#endif
#endif

// OpenAmulet support non-rectangular regions on X-Window systems. This
// feature isn't smoothly integrated into the library, it's more a start
// implementation for an upcoming extension. Therefore the feature is
// DISABLED by default, if you want to use this feature, uncomment the
// define. NOTE: Your code won't be portable any longer.

// #define POLYGONAL_REGIONS

// The define INCLUDE_INSPECTOR has the effect that the call to inspector
// initializer will be included in the compiled library, which is the default
// behavior for a debug library. If you want to make a release library with
// inspector code, define the INCLUDE_INSPECTOR on the compiler command-line
// and compile a new library
#ifdef DEBUG
#define INCLUDE_INSPECTOR
#endif

// WIN_KEYBINDINGS
// Use Windows-compatible keybindings
#ifdef _WIN32
#define WIN_KEYBINDINGS
#endif

// Define DLL to make a dll version of the OA library
// #if (defined(__MINGW32__) || defined(_MSC_VER)) && !defined(STATIC)
// # define DLL
// #endif

#ifdef __GNUC__
#define AM_NORETURN __attribute__((noreturn))
#define AM_FORMATLIKE(a, b) __attribute__((format(printf, a, b)))
#elif defined(_MSC_VER)
#define AM_NORETURN __declspec(noreturn)
#define AM_FORMATLIKE(a, b)
#else
#define AM_NORETURN
#define AM_FORMATLIKE(a, b)
#endif
