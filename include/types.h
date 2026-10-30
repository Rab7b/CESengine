#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <variant>

namespace ces {
	using Entity = uint32_t;
	inline constexpr int maxEntities = 1000;
	inline constexpr Entity invalidEntity = 0xFFFFFFFF;

	struct Position {
		float x, y;
	};

	struct Velocity {
		float vx, vy;
	};

	struct Visual {
    	std::variant<sf::CircleShape, sf::RectangleShape, sf::ConvexShape> it;
	};

	using Vector2f = sf::Vector2f;
	using Vector2i = sf::Vector2i;
	using Vector2u = sf::Vector2u;
	using Vector3f = sf::Vector3f;
	using FloatRect = sf::FloatRect;
	using IntRect = sf::IntRect;
	using Color = sf::Color;
	using Time = sf::Time;
	using Clock = sf::Clock;

	using RenderWindow = sf::RenderWindow;
	using VideoMode = sf::VideoMode;
	using ContextSettings = sf::ContextSettings;
	using Event = sf::Event;

	namespace Keyboard = sf::Keyboard;
	namespace Mouse = sf::Mouse;

	using Texture = sf::Texture;
	using Sprite = sf::Sprite;
	using Image = sf::Image;
	using Font = sf::Font;
	using Text = sf::Text;
	using Shape = sf::Shape;
	using RectangleShape = sf::RectangleShape;
	using CircleShape = sf::CircleShape;
	using ConvexShape = sf::ConvexShape;
	using Vertex = sf::Vertex;
	using VertexArray = sf::VertexArray;
	using View = sf::View;
	using RenderStates = sf::RenderStates;
	using Shader = sf::Shader;

	using SoundBuffer = sf::SoundBuffer;
	using Sound = sf::Sound;
	using Music = sf::Music;
	namespace Listener = sf::Listener;

	using IpAddress = sf::IpAddress;
	using TcpSocket = sf::TcpSocket;
	using UdpSocket = sf::UdpSocket;
	using Packet = sf::Packet;
}
