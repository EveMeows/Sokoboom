#pragma once

#include "state.h"
#include "../button.h"
#include "../data.h"
#include "../utilities.h"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <memory>

class End : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::vector<Button> m_buttons;

	std::shared_ptr<GameData> m_data;
public:
	End(std::shared_ptr<GameData> data) : m_data(data) 
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
		this->m_click = utilities::load_sound_relative(std::filesystem::path("Content/Audio/click.wav"));
	};

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};

inline void End::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}