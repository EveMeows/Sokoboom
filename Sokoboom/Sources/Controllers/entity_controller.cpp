#include "../../Headers/Controllers/entity_controller.h"

void EntityController::add(std::unique_ptr<Entity> e)
{
	this->m_entities.push_back(std::move(e));
}

void EntityController::process()
{
	for (const std::unique_ptr<Entity>& entity : this->m_entities)
	{
		entity->process();
	}
}

void EntityController::render()
{
	for (const std::unique_ptr<Entity>& entity : this->m_entities)
	{
		entity->render();
	}
}