#pragma once
#include "../Engine/Engine.h"

namespace ces {

	class Engine;

	class System {
	protected:
		ces::Engine& engine;
	public:

		explicit System(ces::Engine& e) : engine(e) {}

		virtual ~System() = default;
		virtual inline void update() = 0;

		inline ces::Engine& getEngine() {
			return this->engine;
		}
	};
}