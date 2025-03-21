#pragma once

#include <vector>
#include <memory>

#include "../Entities/entity.h"

class EntityController
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;

public:
	template<typename T>
	T* get_first();

	template<typename T>
	T* get_id(int id);

	void add(std::unique_ptr<Entity> e);

	void process();
	void render();
};

inline void EntityController::process()
{
}

inline void EntityController::render()
{
}