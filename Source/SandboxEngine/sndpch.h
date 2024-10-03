#pragma once

// STD
#include <chrono>
#include <algorithm>
#include <filesystem>

// STL
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <unordered_map>

// CSTD
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdarg.h>

// Core
#include "SandboxEngine/Core/FundamentalTypes.h"
#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/String.h"
#include "SandboxEngine/Core/StringId.h"
#include "SandboxEngine/Core/Delegate.h"

// Memory
#include "SandboxEngine/Memory/Memory.h"
#include "SandboxEngine/Memory/Buffer.h"
#include "SandboxEngine/Memory/SparseBuffer.h"

// Math
#include "SandboxEngine/Math/Math.h"
#include "SandboxEngine/Math/Euler.h"
#include "SandboxEngine/Math/Vector.h"
#include "SandboxEngine/Math/Matrix.h"
#include "SandboxEngine/Math/Quat.h"

#ifdef SND_PLATFORM_WINDOWS
	#define NOMINMAX // disable windows min/max macro definitions to avoid possible name conflicts
	#include "Windows.h"
#endif
