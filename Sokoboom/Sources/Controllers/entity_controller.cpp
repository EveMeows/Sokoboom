#include "../../Headers/Controllers/entity_controller.h"

template<typename T>
T* EntityController::get_first()
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
T* EntityController::get_id(int id)
{
	for (const std::unique_ptr<Entity>& e : this->m_entities)
	{
		if (e->id == id)
		{
			return e;
		}
	}

	return nullptr;
}

void EntityController::add(std::unique_ptr<Entity> e)
{
	this->m_entities.push_back(std::move(e));
}
