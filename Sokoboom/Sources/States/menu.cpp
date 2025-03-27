#include "../../Headers/States/menu.h"

#include "../../Headers/utilities.h"
#include "../../Headers/States/game.h"
#include "../../Headers/States/settings.h"

#include <filesystem>
#include <format>

#include <raymath.h>

Menu::Menu(std::shared_ptr<GameData> data) : m_data(data)
{
	this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
	SetTextureFilter(this->m_font.texture, TEXTURE_FILTER_POINT);
}


// Build UI here since `this` will be incomplete in Menu::Menu
void Menu::awake()
{
	Vector2 play_dim = MeasureTextEx(this->m_font, "play", 10.0f, 0.1f);
	Button play = Button(
		this->m_font,
		"play", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - play_dim.x) / 2,
			50
		)
	);

	play.on_click = [this](Button* self) {
		this->m_data->state_handler->set(
			std::make_unique<Game>(this->m_data, this->m_data->maps[this->m_data->active_map_index])
		);
	};

	this->m_buttons.push_back(play);

	Vector2 options_dim = MeasureTextEx(this->m_font, "options", 10.0f, 0.1f);
	Button options = Button(
		this->m_font,
		"options", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - options_dim.x) / 2,
			60
		)
	);

	options.on_click = [this](Button* self) {
		this->m_data->state_handler->set(
			std::make_unique<Settings>(this->m_data)
		);
	};

	this->m_buttons.push_back(options);

	Vector2 exit_dim = MeasureTextEx(this->m_font, "exit", 10.0f, 0.1f);
	Button exit = Button(
		this->m_font,
		"exit", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - exit_dim.x) / 2,
			70
		)
	);

	exit.on_click = [this](Button* self) {
		this->m_data->exit = true;
	};

	this->m_buttons.push_back(exit);
}

void Menu::process()
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(this->m_data->virtual_mouse);
	}
}

void Menu::render()
{
	ClearBackground(SKYBLUE);

	Vector2 dim = MeasureTextEx(this->m_font, "SOKOBOOM", 10.0f, 0.1f);
	DrawTextPro(
		this->m_font,
		"SOKOBOOM",
		Vector2(
			(GameData::GAME_SIZE.x - dim.x) / 2,
			15
		),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}