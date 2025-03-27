#pragma once

#include "state.h"
#include "../data.h"
#include "../button.h"

#include <vector>
#include <memory>

#include <raylib.h>

class Menu : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::shared_ptr<GameData> m_data;

	std::vector<Button> m_buttons;
public:
	Menu(std::shared_ptr<GameData> data);

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};

inline void Menu::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}