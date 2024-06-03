#pragma once

#include <array>
#include <regex>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <algorithm>
#include <memory>
#include <string>
#include <string.h>
#include <functional>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <queue>
#include <fstream>
#include <filesystem>
#include <bit>
#include <map>
#include <random>
#include <stdexcept>
#include <future>

#include "ScopedTimer.h"
#include <stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <stdio.h>
#include "imgui_internal.h"

#define BIT(x) (1 << x)

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> MakeUnique(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Shared<T> MakeShared(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define BIND_FN(x) std::bind(&x, this, std::placeholders::_1)