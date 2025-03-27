#pragma once

#include <functional>
#include <string>

#include <raylib.h>
#include <raymath.h>

class Button
{
private:
	Font m_font;
	std::string m_text;

	Vector2 m_position;
	Rectangle m_bounds;

	float m_size;
public:
	Color colour = WHITE;
	std::function<void(Button*)> on_click;

	Button(Font font, std::string text, float size, Vector2 pos) : 
		m_font(font), m_text(text), m_size(size), m_position(pos)
	{
		Vector2 dim = MeasureTextEx(
			this->m_font, this->m_text.c_str(),
			this->m_size, 0.1f
		);

		this->m_bounds = Rectangle(
			this->m_position.x, this->m_position.y,
			dim.x, dim.y
		);
	}

	void set_text(std::string new_text)
	{
		this->m_text = new_text;

		Vector2 dim = MeasureTextEx(
			this->m_font, this->m_text.c_str(),
			this->m_size, 0.1f
		);

		this->m_bounds = Rectangle(
			this->m_position.x, this->m_position.y,
			dim.x, dim.y
		);
	}

	void render();
	void process(Vector2 mouse);
	void leave();
};

inline void Button::process(Vector2 mouse)
{
	if (CheckCollisionPointRec(mouse, this->m_bounds))
	{
		this->colour = YELLOW;
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			if (this->on_click != nullptr)
			{
				this->on_click(this);
			}
		}
	}
	else
	{
		this->colour = WHITE;
	}
}

inline void Button::render()
{
	DrawTextPro(
		this->m_font, this->m_text.c_str(),
		this->m_position, Vector2Zero(),
		0, this->m_size, 0.1f, this->colour
	);
}

inline void Button::leave()
{
	UnloadFont(this->m_font);
}