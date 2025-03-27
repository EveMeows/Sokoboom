#include "../../Headers/States/settings.h"
#include "../../Headers/States/menu.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include <iostream>

void Settings::awake()
{
	std::filesystem::path path = GetApplicationDirectory() / std::filesystem::path("Content/settings.json");
	std::ifstream f(path);
	if (f.is_open())
	{
		nlohmann::json json = nlohmann::json::parse(f);

		this->m_mute_sfx = json["mute_sfx"];
		this->m_mute_move = json["mute_move"];

		f.close();
	}

	Button menu = Button(
		this->m_font,
		"< BACK", 10.0f,
		Vector2(
			1,
			GameData::GAME_SIZE.y - 11
		)
	);

	menu.on_click = [this](Button* self) {
		if (!this->m_mute_sfx) PlaySound(this->m_click);

		std::filesystem::path path = GetApplicationDirectory() / std::filesystem::path("Content/settings.json");
		std::ofstream f(path);
		if (f.is_open())
		{
			nlohmann::json json;
			json["mute_sfx"] = this->m_mute_sfx;
			json["mute_move"] = this->m_mute_move;

			this->m_data->mute_sfx = this->m_mute_sfx;
			this->m_data->mute_move = this->m_mute_move;

			f << json.dump(4);
			f.close();
		}

		this->m_data->state_handler->set(
			std::make_unique<Menu>(this->m_data)
		);
	};

	this->m_buttons.push_back(menu);

	std::string sfx_text = this->m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX";
	Button sfx = Button(
		this->m_font,
		sfx_text, 5.0f,
		Vector2(1, 30)
	);

	sfx.on_click = [this](Button* self) {
		if (!this->m_mute_sfx) PlaySound(this->m_click);

		this->m_mute_sfx = !this->m_mute_sfx;
		self->set_text(this->m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX");
	};

	this->m_buttons.push_back(sfx);

	std::string move_text = this->m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE";
	Button move = Button(
		this->m_font,
		move_text, 5.0f,
		Vector2(1, 37)
	);

	move.on_click = [this](Button* self) {
		if (!this->m_mute_sfx) PlaySound(this->m_click);

		this->m_mute_move = !this->m_mute_move;
		self->set_text(this->m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE");
	};

	this->m_buttons.push_back(move);
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