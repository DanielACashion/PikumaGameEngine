#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

class IComponent {
protected:
  static int nextId;
};

template <typename T> class Component : IComponent {
public:
  Component(const Component &Component) { Logger::Log("Copy was called"); }
  static int GetId() {
    static auto id = nextId++;
    return id;
  };
};

class Entity {
  int _id;

public:
  Entity(int id) : _id(id) {};
  int GetId() const;
  Entity &operator=(const Entity &other) = default;
  bool operator==(const Entity &other) const { return this->_id == other._id; };
  bool operator!=(const Entity &other) const { return this->_id != other._id; };
  bool operator<(const Entity &other) const { return this->_id < other._id; };
  bool operator>(const Entity &other) const { return this->_id > other._id; };
};

//////////////////////
//     SYSTEMS      //
//////////////////////
const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

class System {
  Signature _componentSignature;
  std::vector<Entity> _entities;

public:
  System() = default;
  ~System() = default;
  void AddEntityToSystem(Entity entity);
  void RemoveEntityFromSystem(Entity entity);
  std::vector<Entity> GetSystemEntities() const;
  const Signature &GetComponentSignature() const;

  //
  template <typename T> void RequireComponent();
};
class IPool {
  //
public:
  virtual ~IPool() {};
};

template <typename T> class Pool : IPool {
  std::vector<T> data;

public:
  Pool(int size = 100) { this->data.resize(size); }
  ~Pool() = default;

  bool isEmpty() const { return this->data.empty(); }

  int GetSize() const { return this->data.size(); }

  void Resize(int size) { this->data.resize(size); }

  void Clear() { this->data.clear(); }

  void Add(T object) { this->data.push_back(object); }

  void Set(int index, T object) { data[index] = object; }

  T &Get(int index) const { return static_cast<T>(this->data[index]); }

  T &operator[](unsigned int index) const { return this->data[index]; }
};

class Registry {
  int _numEntities = 0;
  // each pool contains all the data for a comp type
  std::vector<IPool *> _componentPools;
  std::vector<Signature> _entityComponentSignatures;
  std::unordered_map<std::type_index, System *> _systems;
  std::set<Entity> _entitiesToBeAdded;
  std::set<Entity> _entitiesToBeKilled;

  void Update();
  template <typename TComponent> bool HasComponent(Entity entity);

public:
  Registry() = default;
  Entity CreateEntity();
  void AddEntityToSystem(Entity entity);
  void KillEntity(Entity entity);

  // AddComponent();
  template <typename TComponent, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args);

  template <typename TComponent> void RemoveComponent(Entity entity);
};

template <typename TComponent> void System::RequireComponent() {
  const auto componentId = Component<TComponent>::GetId();
  this->_componentSignature.set(componentId);
}

#endif