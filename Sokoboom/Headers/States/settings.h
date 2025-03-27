#pragma once

#include "state.h"
#include "../data.h"
#include "../utilities.h"
#include "../button.h"

#include <memory>
#include <vector>
#include <filesystem>

#include <raylib.h>
#include <raymath.h>

class Settings : public State
{
private:
	std::vector<Button> m_buttons;

	Font m_font = { 0 };
	std::shared_ptr<GameData> m_data;
public:
	Settings(std::shared_ptr<GameData> data) : m_data(data)
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
	};

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};

inline void Settings::leave()
{
	UnloadFont(this->m_font);
}