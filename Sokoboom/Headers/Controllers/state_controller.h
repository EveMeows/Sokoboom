#pragma once

#include "entity_controller.h"
#include "../States/state.h"

#include "../data.h"

#include <memory>

class StateController
{
private:
	std::unique_ptr<State> m_state = nullptr;
	std::unique_ptr<State> m_temporary = nullptr;

	bool m_reverse = false;

	float m_speed = 5;
	float m_opactity = 0;
public:
	bool switching = false;

	Color colour() const;
	void set(std::unique_ptr<State> state);

	void process();
	void render();
	void leave();
};

inline Color StateController::colour() const
{
	return Fade(BLACK, this->m_opactity);
}

inline void StateController::set(std::unique_ptr<State> state)
{
	this->switching = true;
	this->m_temporary = std::move(state);
}

inline void StateController::process()
{
	if (this->switching)
	{
		if (!this->m_reverse)
		{
			this->m_opactity += this->m_speed * GetFrameTime();
			if (this->m_opactity >= 1)
			{
				this->m_reverse = true;
				this->m_opactity = 1;

				if (this->m_state != nullptr)
				{
					this->m_state->leave();
				}

				this->m_state = std::move(this->m_temporary);
				this->m_state->awake();
			}
		}
		else
		{
			this->m_opactity -= this->m_speed * GetFrameTime();
			if (this->m_opactity <= 0)
			{
				this->m_reverse = false;
				this->m_opactity = 0;

				this->switching = false;
			}
		}
	}

	if (this->m_state == nullptr) return;
	this->m_state->process();
}

inline void StateController::render()
{
	if (this->m_state == nullptr) return;
	this->m_state->render();
}

inline void StateController::leave()
{
	if (this->m_state == nullptr) return;
	this->m_state->leave();

	this->m_state.reset();
}