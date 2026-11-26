#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

namespace ces {

	using namespace sf;

	constexpr std::uint32_t maxEntities{ 1'000'001 };

	struct Entity {
		std::uint32_t id{};
		std::vector<std::uint32_t> component_ids{};
	};

	struct Transform {
		float x{};
		float y{};
		float width{};
		float height{};

		ces::Color color{};
	};
	struct Velocity {
		float vx{};
		float vy{};
	};
	struct Circle {
		sf::CircleShape circle{};
	};
	struct Triangle {
		ces::CircleShape circle{};

		Triangle() {
			circle.setPointCount(3);
		}
	};
	struct RectShape {
		ces::RectangleShape rect{};
	};

}
