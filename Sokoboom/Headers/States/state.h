#pragma once

#include "../Controllers/entity_controller.h"

#include <memory>

class State
{
public:
	virtual void process() = 0;
	virtual void render() = 0;
};

