#include "../include/ces.h"
#include <vector>
#include <random>
#include <cmath>
#include <iostream>

float getRandom(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int main() {
    constexpr unsigned int SCREEN_WIDTH = 1280;
    constexpr unsigned int SCREEN_HEIGHT = 720;

    auto& engine = ces::Engine::getInstance(
        { static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) },
        "CES Engine - Black Hole",
        false,
        1000.0f
    );

    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>();

    auto& registry = engine.getRegistry();

    constexpr size_t PARTICLE_COUNT = 1000000;
    std::vector<ces::Entity> particles;
    particles.reserve(PARTICLE_COUNT);

    float centerX = SCREEN_WIDTH / 2.0f;
    float centerY = SCREEN_HEIGHT / 2.0f;

    for (size_t i = 0; i < PARTICLE_COUNT; ++i) {
        ces::Entity p = registry.createEntity();

        float radius = getRandom(30.0f, 550.0f);
        float angle = getRandom(0.0f, 6.2831853f);

        float startX = centerX + std::cos(angle) * radius;
        float startY = centerY + std::sin(angle) * radius;

        registry.getPool<ces::Transform>().get(p) = {
            startX,
            startY,
            getRandom(1.2f, 3.2f),
            0.1f,
            ces::Color(100, 200, 255) 
        };

        registry.getPool<ces::Velocity>().get(p) = {
            -std::sin(angle) * getRandom(0.1f, 0.1f),
             std::cos(angle) * getRandom(0.1f, 0.1f)
        };

        particles.push_back(p);
    }

    while (engine.getWindow().isOpen()) {
        engine.run();

        engine.getWindow().setTitle("CES Engine - Black Hole | FPS: " + std::to_string(static_cast<int>(engine.getFPS())));

        auto& transforms = registry.getPool<ces::Transform>().getData();
        auto& velocities = registry.getPool<ces::Velocity>().getData();

        for (size_t i = 0; i < PARTICLE_COUNT; ++i) {
            auto& t = transforms[i];
            auto& v = velocities[i];

            float dx = centerX - t.x;
            float dy = centerY - t.y;
            float distSq = dx * dx + dy * dy + 80.0f;
            float dist = std::sqrt(distSq);

            float invDist = 1.0f / dist;
            float pullStrength = 2400.0f / distSq;

            v.vx += (dx * invDist) * pullStrength - (dy * invDist) * 0.35f;
            v.vy += (dy * invDist) * pullStrength + (dx * invDist) * 0.35f;

            v.vx *= 0.96f;
            v.vy *= 0.96f;

            t.x += v.vx;
            t.y += v.vy;

            if (distSq < (15.0f * 15.0f) || t.x < 0.0f || t.x > SCREEN_WIDTH || t.y < 0.0f || t.y > SCREEN_HEIGHT) {
                float angle = getRandom(0.0f, 6.2831853f);
                float radius = getRandom(500.0f, 650.0f);
                t.x = centerX + std::cos(angle) * radius;
                t.y = centerY + std::sin(angle) * radius;
                v.vx = -std::sin(angle) * getRandom(0.1f, 0.1f);
                v.vy = std::cos(angle) * getRandom(0.1f, 0.1f);
            }
        }
    }

    return 0;
}
