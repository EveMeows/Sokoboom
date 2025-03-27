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

#include <json.hpp>

class Settings : public State
{
private:
	std::vector<Button> m_buttons;

	Sound m_click = { 0 };
	Font m_font = { 0 };

	std::shared_ptr<GameData> m_data;

	bool m_mute_move = false;
	bool m_mute_sfx = false;
public:
	Settings(std::shared_ptr<GameData> data) : m_data(data)
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
		this->m_click = utilities::load_sound_relative(std::filesystem::path("Content/Audio/click.wav"));
	};

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};

inline void Settings::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}