#pragma once

#include <vector>
#include <memory>

#include "../Entities/entity.h"

class EntityController
{
private:
	std::vector<std::shared_ptr<Entity>> m_entities;

public:
	template<typename T>
	void remove();

	void add(std::shared_ptr<Entity> e);

	void process();
	void render();
	void leave();
};

template<typename T>
inline void EntityController::remove()
{
	for (const std::shared_ptr<Entity>& entity : this->m_entities)
	{
		if (std::shared_ptr<T> out = std::dynamic_pointer_cast<T>(entity))
		{
			this->m_entities.erase(
				std::find(this->m_entities.begin(), this->m_entities.end(), entity)
			);

			return;
		}
	}
}