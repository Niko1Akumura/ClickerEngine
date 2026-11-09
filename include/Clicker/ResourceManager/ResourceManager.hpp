#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace Clicker::Resource {
template <typename Resource, typename Identifier> class ResourceManager {
private:
  std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resources;

public:
  void loadResource(const Identifier &id, const std::string &filePath,
                    bool smooth = false) {
    auto resource = std::make_unique<Resource>();
    if constexpr (requires { resource->loadFromFile(filePath); }) {
      if (!resource->loadFromFile(filePath)) {
        throw std::runtime_error("Failed to load: " + filePath);
      }
    } else if constexpr (requires { resource->openFromFile(filePath); }) {
      if (!resource->openFromFile(filePath)) {
        throw std::runtime_error("Failed to load: " + filePath);
      }
    } else {
      static_assert(sizeof(Resource) == 0,
                    "Resource type does not support loading from file");
    }
    if (this->has(id)) {
      throw std::runtime_error("Resource already exists");
    }

    if constexpr (requires { resource->setSmooth(smooth); }) {
      resource->setSmooth(smooth);
    }

    m_resources.emplace(id, std::move(resource));
  }

  const Resource &get(const Identifier &id) const {
    auto resource = this->m_resources.find(id);

    if (resource == this->m_resources.end()) {
      throw std::runtime_error("Resource not found");
    }

    return *resource->second;
  }

  bool has(const Identifier &id) const {
    return this->m_resources.contains(id);
  }
};
} // namespace Clicker::Resource
