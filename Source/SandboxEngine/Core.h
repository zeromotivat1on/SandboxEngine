#pragma once

#ifdef SND_BUILD_DLL
	#define SND_API __declspec(dllexport)
#else
	#define SND_API __declspec(dllimport)
#endif

#define SND_EXPAND_MACRO(x) x
#define SND_STRINGIFY_MACRO(x) #x

#include "Log.h"
#include "Error.h"
