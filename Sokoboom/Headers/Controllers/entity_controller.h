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
	std::shared_ptr<T> get_first();

	template<typename T>
	std::shared_ptr<T> get_id(int id);

	void add(std::shared_ptr<Entity> e);

	void process();
	void render();
	void leave();
};

template<typename T>
inline std::shared_ptr<T> EntityController::get_first()
{
	for (const std::shared_ptr<Entity>& e : this->m_entities)
	{
		if (std::shared_ptr<T> out = std::dynamic_pointer_cast<T>(e))
		{
			return out;
		}
	}

	return nullptr;
}

template<typename T>
inline std::shared_ptr<T> EntityController::get_id(int id)
{
	for (const std::unique_ptr<Entity>& e : this->m_entities)
	{
		if (e->id == id)
		{
			if (std::shared_ptr<T> out = std::dynamic_pointer_cast<T>(e))
			{
				return out;
			}
		}
	}

	return nullptr;
}