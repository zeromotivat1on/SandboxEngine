#pragma once

// STD
#include <chrono>
#include <algorithm>

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

// Core
#include "SandboxEngine/Core/FundamentalTypes.h"
#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/StringId.h"
#include "SandboxEngine/Core/Delegate.h"

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
