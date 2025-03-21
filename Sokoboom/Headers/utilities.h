#pragma once

#include <raylib.h>

#include <string>
#include <filesystem>

namespace utilities
{
	Texture2D load_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadTexture(legacy.c_str());
	}
}