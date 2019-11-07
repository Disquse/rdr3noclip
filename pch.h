// pch.h: is a precompiled header file
// Files below is compiling only once, so the compilation time is significantly reduces.
// Don't include here files, which you change oftenly.

#ifndef PCH_H
#define PCH_H

// Add here header files for precompilation
#include "framework.h"
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include "core/Hash.h"
#include "core/NativeEngine.h"
#include "core/NativeNames.h"
#include "alt-log.h"

#define DLL_EXPORT __declspec(dllexport)

using namespace alt;

#endif //PCH_H
