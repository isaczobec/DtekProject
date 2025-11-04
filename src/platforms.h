#pragma once

#define __WINDOWS__

// check which platform we are compiling to
#if defined(__DTEK__)
#define PLATFROM 1

#elif defined(__WINDOWS__)
#define PLATFROM 2

#else
#error Unknown platform, please define either '__WINDOWS__' or '__DTEK__'
#endif

// define more readable macros for windows and dtek
#define DTEK 1
#define WINDOWS 2