#include "ECS.h"
#include <algorithm>
#include <string>

int Entity::GetId() const { return this->_id; }

void System::AddEntityToSystem(Entity entity) {
  auto eid = entity.GetId();
  auto _ = eid;
  this->_entities.push_back(entity);
};

void System::RemoveEntityFromSystem(Entity entity) {
  // const auto eid = entity.GetId();
  this->_entities.erase(
      std::remove_if(this->_entities.begin(), this->_entities.end(),
                     [entity](Entity current) { return entity == current; }),
      this->_entities.end());
};

std::vector<Entity> System::GetSystemEntities() const {
  return this->_entities;
};

const Signature &System::GetComponentSignature() const {
  return this->_componentSignature;
};

Entity Registry::CreateEntity() {
  //
  int entityId = numEntities++;
  if (entityId >= this->_entitiyComponentSignatures.size()) {
    this->_entitiyComponentSignatures.resize(entityId + 1);
  }
  Entity entity(entityId);
  entitiesToBeAdded.insert(entity);
  Logger::Log("Entity created with id: " + std::to_string(entityId));
  return entity;
};