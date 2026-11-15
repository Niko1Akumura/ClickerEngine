#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <vector>
namespace Clicker {
struct HealthComponent {
  float maxHp{100.0f};
  float currentHp{100.f};

  float invulnerabilityDuration{0.25f};
  float invulnerabilityTimer{0.0f};

  std::function<void()> onDeath{nullptr};
  std::function<void(float)> onDamageTaken{nullptr};

  HealthComponent() = default;
  explicit HealthComponent(float maxHealth)
      : maxHp(maxHealth), currentHp(maxHealth) {}

  void update(sf::Time dt) {
    if (this->invulnerabilityTimer > 0.0f) {
      this->invulnerabilityTimer =
          std::max(0.0f, this->invulnerabilityTimer - dt.asSeconds());
    }
  }

  bool takeDamage(float amount) {
    if (this->invulnerabilityTimer > 0.0f || this->isDead() || amount <= 0.0f) {
      return false;
    }

    this->currentHp = std::max(0.0f, this->currentHp - amount);
    this->invulnerabilityTimer = this->invulnerabilityDuration;

    if (this->onDamageTaken) {
      this->onDamageTaken(amount);
    }

    if (this->isDead() && this->onDeath) {
      this->onDeath();
    }

    return true;
  }

  void heal(float amount) {
    if (this->isDead() || amount <= 0.0f)
      return;
    this->currentHp = std::min(this->maxHp, this->currentHp + amount);
  }

  [[nodiscard]] bool isDead() const { return this->currentHp <= 0.0f; }

  [[nodiscard]] bool isInvulnerable() const {
    return this->invulnerabilityTimer > 0.0f;
  }
};

struct MovementComponent {
  sf::Vector2f velocity{0.0f, 0.0f};
  float maxSpeed{300.0f};
  float friction{0.0f};

  void update(sf::Time dt) {
    if (this->friction > 0.0f) {
      float damping = std::max(0.0f, 1.0f - this->friction * dt.asSeconds());
      this->velocity *= damping;
    }
  }
};

struct AnimationClip {
  std::vector<sf::IntRect> frames;
  float frameDuration{0.1f};
  bool loop{true};
};

class AnimationComponent {
private:
  std::unordered_map<std::string, AnimationClip> m_clips;
  std::string m_currentClipName;
  size_t m_currentFrame{0};
  float m_elapsedTime{0.0f};
  bool m_isFinished{false};

public:
  void addClip(const std::string &name, AnimationClip clip) {
    this->m_clips[name] = std::move(clip);
    if (this->m_currentClipName.empty()) {
      this->play(name);
    }
  }

  void play(const std::string &name, bool resetIfSame = false) {
    if (this->m_currentClipName == name && !resetIfSame) {
      return;
    }

    auto it = this->m_clips.find(name);
    if (it != this->m_clips.end()) {
      this->m_currentClipName = name;
      this->m_currentFrame = 0;
      this->m_elapsedTime = 0.0f;
      this->m_isFinished = false;
    }
  }

  void update(sf::Time dt, sf::Sprite &sprite) {
    if (this->m_currentClipName.empty() || this->m_isFinished) {
      return;
    }

    const auto &clip = this->m_clips[this->m_currentClipName];
    if (clip.frames.empty()) {
      return;
    }

    this->m_elapsedTime += dt.asSeconds();

    if (this->m_elapsedTime >= clip.frameDuration) {
      this->m_elapsedTime -= clip.frameDuration;
      this->m_currentFrame++;

      if (this->m_currentFrame >= clip.frames.size()) {
        if (clip.loop) {
          this->m_currentFrame = 0;
        } else {
          this->m_currentFrame = clip.frames.size() - 1;
          this->m_isFinished = true;
        }
      }
    }

    sprite.setTextureRect(clip.frames[this->m_currentFrame]);
  }

  [[nodiscard]] bool isFinished() const { return this->m_isFinished; }

  [[nodiscard]] const std::string &getCurrentClip() const {
    return this->m_currentClipName;
  }
};
} // namespace Clicker
