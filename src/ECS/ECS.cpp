#include "ECS.h"
#include <algorithm>
#include <string>
#include <typeindex>
#include <utility>

int IComponent::nextId = 0;

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
  int entityId = _numEntities++;
  if (entityId >= this->_entityComponentSignatures.size()) {
    this->_entityComponentSignatures.resize(entityId + 1);
  }
  Entity entity(entityId);
  this->_entitiesToBeAdded.insert(entity);
  Logger::Log("Entity created with id: " + std::to_string(entityId));
  return entity;
};

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();
  if (componentId >= this->_componentPools.size()) {
    this->_componentPools.resize(componentId + 1, nullptr);
  }

  if (!this->_componentPools[componentId]) {
    Pool<TComponent> *newComponentPool = new Pool<TComponent>;
    this->_componentPools[componentId] = newComponentPool;
  }
  Pool<TComponent> *componentPool = this->_componentPools[componentId];

  if (entityId >= componentPool->GetSize()) {
    componentPool->Resize(this->_numEntities);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);
  componentPool->Set(entityId, newComponent);
  this->_entityComponentSignatures[entityId].set(componentId);
};

template <typename TComponent> bool Registry::HasComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  return this->_entityComponentSignatures[entityId].test(componentId);
};

template <typename TComponent> void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  this->_entityComponentSignatures[entityId].set(componentId, false);
};

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args) {
  TSystem *system(new TSystem(std::forward<TArgs>(args)...));
  this->_systems.insert(
      std::make_pair(std::type_index(typeid(TSystem)), system));
}

template <typename TSystem> void Registry::RemoveSystem() {
  const auto system = this->_systems.find(std::type_index(typeid(TSystem)));
  this->_systems.erase(system);
}

template <typename TSystem> bool Registry::HasSystem() const {
  return this->_systems.find(std::type_index(typeid(TSystem))) !=
         this->_systems.end();
}
template <typename TSystem> TSystem &Registry::GetSystem() const {
  auto system = this->_systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}
void Registry::AddEntityToSystem(Entity entity) {
  const auto entityId = entity.GetId();
  const auto &entityComponentSignature =
      this->_entityComponentSignatures[entityId];
  for (auto &system : this->_systems) {
    const auto &systemComponentSignature =
        system.second->GetComponentSignature();
    bool isInterested = (entityComponentSignature & systemComponentSignature) ==
                        systemComponentSignature;
    if (isInterested) {
      system.second->AddEntityToSystem(entity);
    }
  }
};