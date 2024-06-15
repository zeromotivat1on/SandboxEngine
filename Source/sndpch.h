#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <cstdint>
#include <cstdlib>

#ifdef SND_PLATFORM_WINDOWS
	#define NOMINMAX // disable windows min/max macro definitions to avoid possible name conflicts
	#include "Windows.h"
#endif
