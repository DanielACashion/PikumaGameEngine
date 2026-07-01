#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <iostream>
#include <vector>

class IComponent {
protected:
  static int nextId;
};

template <typename T> class Component : IComponent {
public:
  Component(const Component &Component) { std::cout << "Copy was called"; }
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
  bool operator!=(const Entity &other) const { return this->_id != other._id; }
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

class Registry {
  //
};

template <typename TComponent> void System::RequireComponent() {
  const auto componentId = Component<TComponent>::GetId();
  this->_componentSignature.set(componentId);
}

#endif