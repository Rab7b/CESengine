#include "../include/ces.h" // or #include <ces.h>
#include <cmath>
#include <cstdint>
#include <functional>
#include <string>
#include <thread>

namespace {
    inline float fastRandom(float min, float max) {
        thread_local std::uint32_t state = []() {
            const auto tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
            const std::uint32_t seed = static_cast<std::uint32_t>(tid ^ 0x9E3779B9u);
            return seed != 0u ? seed : 1337u;
            }();

        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;

        const float normalized = static_cast<float>(state) * (1.0f / 4294967296.0f);
        return min + normalized * (max - min);
    }
}

int main() {
    constexpr unsigned int SCREEN_WIDTH = 1280;
    constexpr unsigned int SCREEN_HEIGHT = 720;
    constexpr float SCREEN_WIDTH_F = static_cast<float>(SCREEN_WIDTH);
    constexpr float SCREEN_HEIGHT_F = static_cast<float>(SCREEN_HEIGHT);
    constexpr float TWO_PI = 6.2831853f;
    constexpr std::size_t PARTICLE_COUNT = 1000001;

    auto& engine = ces::Engine::getInstance(
        { SCREEN_WIDTH_F, SCREEN_HEIGHT_F },
        "CES Engine - Black Hole",
        false,
        1000.0f
    );

    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>();

    auto& registry = engine.getRegistry();
    auto& transformPool = registry.getPool<ces::Transform>();
    auto& velocityPool = registry.getPool<ces::Velocity>();

    const float centerX = SCREEN_WIDTH_F * 0.5f;
    const float centerY = SCREEN_HEIGHT_F * 0.5f;
    const ces::Color defaultColor(100, 200, 255);

    for (std::size_t i = 0; i < PARTICLE_COUNT; ++i) {
        const ces::Entity p = registry.createEntity();

        const float radius = fastRandom(30.0f, 550.0f);
        const float angle = fastRandom(0.0f, TWO_PI);
        const float sinA = std::sin(angle);
        const float cosA = std::cos(angle);

        transformPool.add(p, {
            centerX + cosA * radius,
            centerY + sinA * radius,
            fastRandom(1.2f, 3.2f),
            0.1f,
            defaultColor
            });

        velocityPool.add(p, {
            -sinA * 0.1f,
             cosA * 0.1f
            });
    }

    auto& transforms = transformPool.getData();
    auto& velocities = velocityPool.getData();
    const int count = static_cast<int>(transforms.size());

    int titleFrameCounter = 0;

    while (engine.getWindow().isOpen()) {
        engine.run();

        if (++titleFrameCounter >= 30) {
            engine.getWindow().setTitle(
                "CES Engine - Black Hole | FPS: " + std::to_string(static_cast<int>(engine.getFPS()))
            );
            titleFrameCounter = 0;
        }

#pragma omp parallel for schedule(static)
        for (int i = 0; i < count; ++i) {
            auto& t = transforms[i];
            auto& v = velocities[i];

            const float dx = centerX - t.x;
            const float dy = centerY - t.y;
            const float distSq = dx * dx + dy * dy + 80.0f;
            const float invDist = 1.0f / std::sqrt(distSq);
            const float pullStrength = 2400.0f / distSq;

            const float dirX = dx * invDist;
            const float dirY = dy * invDist;

            v.vx = (v.vx + dirX * pullStrength - dirY * 0.35f) * 0.96f;
            v.vy = (v.vy + dirY * pullStrength + dirX * 0.35f) * 0.96f;

            if (distSq < 225.0f || t.x < 0.0f || t.x > SCREEN_WIDTH_F || t.y < 0.0f || t.y > SCREEN_HEIGHT_F) {
                const float angle = fastRandom(0.0f, TWO_PI);
                const float radius = fastRandom(500.0f, 650.0f);
                const float sinA = std::sin(angle);
                const float cosA = std::cos(angle);

                t.x = centerX + cosA * radius;
                t.y = centerY + sinA * radius;
                v.vx = -sinA * 0.1f;
                v.vy = cosA * 0.1f;
            }
        }
    }

    return 0;
}
