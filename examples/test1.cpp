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

ces::Color getBlackHoleColor(float progress, float distance) {
    ces::Color yellow(255, 230, 50);
    ces::Color pink(255, 50, 150);
    ces::Color blue(50, 150, 255);

    float t = std::fmod(progress + distance * 0.002f, 1.0f);
    if (t < 0.0f) t += 1.0f;

    std::uint8_t r, g, b;
    if (t < 0.33f) {
        float f = t / 0.33f;
        r = static_cast<std::uint8_t>(yellow.r + f * (pink.r - yellow.r));
        g = static_cast<std::uint8_t>(yellow.g + f * (pink.g - yellow.g));
        b = static_cast<std::uint8_t>(yellow.b + f * (pink.b - yellow.b));
    }
    else if (t < 0.66f) {
        float f = (t - 0.33f) / 0.33f;
        r = static_cast<std::uint8_t>(pink.r + f * (blue.r - pink.r));
        g = static_cast<std::uint8_t>(pink.g + f * (blue.g - pink.g));
        b = static_cast<std::uint8_t>(pink.b + f * (blue.b - pink.b));
    }
    else {
        float f = (t - 0.66f) / 0.34f;
        r = static_cast<std::uint8_t>(blue.r + f * (yellow.r - blue.r));
        g = static_cast<std::uint8_t>(blue.g + f * (yellow.g - blue.g));
        b = static_cast<std::uint8_t>(blue.b + f * (yellow.b - blue.b));
    }

    float brightness = std::clamp(400.0f / (distance + 40.0f), 0.4f, 1.8f);

    return ces::Color(
        static_cast<std::uint8_t>(std::min(r * brightness, 255.0f)),
        static_cast<std::uint8_t>(std::min(g * brightness, 255.0f)),
        static_cast<std::uint8_t>(std::min(b * brightness, 255.0f))
    );
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

        sf::Color initialColor = getBlackHoleColor(getRandom(0.0f, 1.0f), radius);

        registry.getPool<ces::Transform>().get(p) = {
            startX,
            startY,
            getRandom(1.2f, 3.2f),
            0.1f,
            initialColor
        };

        registry.getPool<ces::Velocity>().get(p) = {
            -std::sin(angle) * getRandom(0.1f, 0.1f),
             std::cos(angle) * getRandom(0.1f, 0.1f)
        };

        particles.push_back(p);
    }

    sf::Clock fpsClock;
    int frameCount = 0;

    while (engine.getWindow().isOpen()) {
        engine.run();

        for (auto p : particles) {
            auto& t = registry.getPool<ces::Transform>().get(p);
            auto& v = registry.getPool<ces::Velocity>().get(p);

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

            float currentDist = std::sqrt(dx * dx + dy * dy);
            t.color = getBlackHoleColor(currentDist * 0.005f, currentDist);

            if (currentDist < 15.0f || t.x < 0.0f || t.x > SCREEN_WIDTH || t.y < 0.0f || t.y > SCREEN_HEIGHT) {
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
