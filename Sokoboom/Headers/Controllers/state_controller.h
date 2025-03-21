#pragma once

#include "entity_controller.h"
#include "../States/state.h"

#include <memory>

class StateController
{
private:
	std::unique_ptr<State> m_state;

public:
	void set(std::unique_ptr<State> state);

	void process();
	void render();
};

inline void StateController::set(std::unique_ptr<State> state)
{
	this->m_state = std::move(state);
}

inline void StateController::process()
{
	this->m_state->process();
}

inline void StateController::render()
{
	this->m_state->render();
}
