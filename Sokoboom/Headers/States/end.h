#pragma once

#include "state.h"
#include "../button.h"
#include "../data.h"
#include "../utilities.h"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>

class End : public State
{
private:
	Font m_font;
	std::vector<Button> m_buttons;

	std::shared_ptr<GameData> m_data;
public:
	End(std::shared_ptr<GameData> data) : m_data(data) 
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
	};

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};

inline void End::leave()
{
	UnloadFont(this->m_font);
}