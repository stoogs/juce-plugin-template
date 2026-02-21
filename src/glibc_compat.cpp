// Compatibility shim to avoid requiring glibc >= 2.43 in the compiled plugin.
//
// On modern distros (e.g. CachyOS), the linker picks up atan2f / sqrtf at
// their GLIBC_2.43 symbol version by default, which breaks loading in hosts
// that only have glibc 2.38-2.42 (e.g. Bitwig shipped on older Debian/Ubuntu).
//
// Strategy: declare private aliases that reference the GLIBC_2.2.5 versions of
// these symbols, then define public replacements that forward to them.  Because
// this is a shared library, all internal callers will end up using our
// definitions, which only pull in the old-versioned symbols.
//
// Compiled without LTO so the .symver directives are not optimised away.

#if defined(__linux__) && defined(__x86_64__)
#include <math.h>

extern "C" {
    // Private aliases → old versioned symbols
    float __compat_sqrtf(float x);
    float __compat_atan2f(float y, float x);
}

__asm__(".symver __compat_sqrtf,  sqrtf@GLIBC_2.2.5");
__asm__(".symver __compat_atan2f, atan2f@GLIBC_2.2.5");

extern "C" {
    // Override sqrtf / atan2f inside the plugin so we never reference 2.43.
    float sqrtf(float x)           { return __compat_sqrtf(x); }
    float atan2f(float y, float x) { return __compat_atan2f(y, x); }
}
#endif
