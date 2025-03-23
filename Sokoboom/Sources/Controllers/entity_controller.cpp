#include "../../Headers/Controllers/entity_controller.h"

void EntityController::add(std::shared_ptr<Entity> e)
{
	this->m_entities.push_back(e);
}

void EntityController::process()
{
	for (const std::shared_ptr<Entity>& entity : this->m_entities)
	{
		entity->process();
	}
}

void EntityController::render()
{
	for (const std::shared_ptr<Entity>& entity : this->m_entities)
	{
		entity->render();
	}
}

void EntityController::leave()
{
	for (const std::shared_ptr<Entity>& entity : this->m_entities)
	{
		entity->leave();
	}
}