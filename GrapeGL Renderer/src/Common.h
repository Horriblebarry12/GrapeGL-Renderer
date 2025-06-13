#pragma once
#include <spdlog/spdlog.h>

#define Log(str) spdlog::info(str)
#define Debug(str) spdlog::debug(str)
#define Warn(str) spdlog::warn(str)
#define Error(str) spdlog::error(str)