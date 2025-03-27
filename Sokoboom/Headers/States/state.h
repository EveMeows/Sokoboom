#pragma once

#include "../Controllers/entity_controller.h"

class State
{
public:
	virtual void awake() {};
	virtual void process() {};
	virtual void render() {};
	virtual void leave() {};
};

