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

template<typename T>
inline T* EntityController::get_first()
{
	for (const std::unique_ptr<Entity>& e : this->m_entities)
	{
		if (T* out = dynamic_cast<T*>(e.get()))
		{
			return out;
		}
	}

	return nullptr;
}

template<typename T>
inline T* EntityController::get_id(int id)
{
	for (const std::unique_ptr<Entity>& e : this->m_entities)
	{
		if (e->id == id)
		{
			if (T* out = static_cast<T*>(e.get()))
			{
				return out;
			}

			return nullptr;
		}
	}

	return nullptr;
}