/* these are basic types I just can't live without */

#ifndef JAMTYPES_H
#define JAMTYPES_H

// I feel horrible doing this, but this is a dumb warning.
// it warns when the compiler is generating overhead by converting the value to
// a bool type, which in C++ means it actually forces it to be either 0 or 1, which
// requires extra code.
#pragma warning( disable : 4800 )

#ifndef NULL
#define NULL (0)
#endif

#define FIXSHIFT (8)
#define FIXAMT   (256)

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;

#endif