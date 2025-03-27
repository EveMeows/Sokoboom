#include "../../Headers/States/settings.h"

#include "../../Headers/States/menu.h"

void Settings::awake()
{
	Button menu = Button(
		this->m_font,
		"< BACK", 10.0f,
		Vector2(
			1,
			GameData::GAME_SIZE.y - 11
		)
	);

	menu.on_click = [this](Button* self) {
		this->m_data->state_handler->set(
			std::make_unique<Menu>(this->m_data)
		);
	};

	this->m_buttons.push_back(menu);
}

void Settings::process()
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(this->m_data->virtual_mouse);
	}
}

void Settings::render()
{
	ClearBackground(SKYBLUE);

	// Background
	DrawRectangleV(Vector2Zero(), GameData::GAME_SIZE, Fade(BLACK, 0.8f));

	Vector2 dim = MeasureTextEx(this->m_font, "SETTINGS", 10.0f, 0.1f);
	DrawTextPro(
		this->m_font,
		"SETTINGS",
		Vector2(
			(GameData::GAME_SIZE.x - dim.x) / 2,
			7
		),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}