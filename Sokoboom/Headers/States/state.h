#pragma once

#include "../Controllers/entity_controller.h"

#include <memory>

class State
{
public:
	virtual void awake() {};
	virtual void process() {};
	virtual void render() {};
	virtual void leave() {};
};

