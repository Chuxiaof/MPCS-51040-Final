#include "util.h"

// NOTE: I haven't been consistent on when to pass point/rectangle by pointer or by value
// (the idea behind passing by pointer is to avoid having to copy on the stack to pass it to the function,
// but in many cases the compiler can optimize away the copy anyway... especially if the function can be inlined)

// COMPLETE ME
