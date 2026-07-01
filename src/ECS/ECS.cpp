#include "ECS.h"
#include <algorithm>

int Entity::GetId() const { return this->_id; }

void System::AddEntityToSystem(Entity entity) {
  auto eid = entity.GetId();
  auto _ = eid;
  this->_entities.push_back(entity);
};
void System::RemoveEntityFromSystem(Entity entity) {
  const auto eid = entity.GetId();
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