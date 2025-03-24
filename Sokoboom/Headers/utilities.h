#pragma once

#include <raylib.h>

#include <string>
#include <filesystem>

namespace utilities
{
	inline Texture2D load_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadTexture(legacy.c_str());
	}

	inline Font load_font_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadFontEx(legacy.c_str(), 100, 0, 0);
	}
}