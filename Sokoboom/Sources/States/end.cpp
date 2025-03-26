#include "../../Headers/States/end.h"

#include "../../Headers/States/menu.h"

#include <format>

void End::awake()
{
	Vector2 menu_dim = MeasureTextEx(this->m_font, "menu", 10.0f, 0.1f);
	Button menu = Button(
		this->m_font,
		"menu", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - menu_dim.x) / 2,
			70
		)
	);

	menu.on_click = [this](Button* self) {
		this->m_data->active_map_index = 0;
		this->m_data->total_moves = 0;

		this->m_data->state_handler->set(
			std::make_unique<Menu>(this->m_data)
		);
	};

	this->m_buttons.push_back(menu);
}

void End::process()
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(this->m_data->virtual_mouse);
	}
}

void End::render()
{
	ClearBackground(SKYBLUE);
	
	Vector2 dim = MeasureTextEx(this->m_font, "GOOD JOB!", 10.0f, 0.1f);
	DrawTextPro(
		this->m_font,
		"GOOD JOB!",
		Vector2(
			(GameData::GAME_SIZE.x - dim.x) / 2,
			10
		),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	Vector2 end_dim = MeasureTextEx(this->m_font, "the end!", 5.0f, 0.1f);
	DrawTextPro(
		this->m_font,
		"the end!",
		Vector2(
			(GameData::GAME_SIZE.x - end_dim.x) / 2,
			22
		),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	DrawTextPro(
		this->m_font,
		"Hope you enjoyed! <3",
		Vector2(1, 40),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	DrawTextPro(
		this->m_font,
		std::format("{} total moves!", this->m_data->total_moves).c_str(),
		Vector2(1, 47),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}