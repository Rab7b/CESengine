#pragma once
#include <vector>
#include "types.h"
#include <algorithm>
#include <iostream>

namespace ces {
	class Registry
	{
	private:
		std::vector<ces::Position> positions;
		std::vector<ces::Velocity> velocities;
		std::vector<ces::Visual> visuals;

		ces::Entity entityCount;
		std::vector<ces::Entity> freeIds;

	public:
		Registry();
		~Registry();

		inline ces::Entity createEntity();
		inline void destroyEntity(ces::Entity e);

		inline ces::Position& getPosByEntity(ces::Entity e) {
			return this->positions[e]; 
		}
		inline ces::Velocity& getVelByEntity(Entity e) {
			return this->velocities[e]; 
		}
		inline ces::Visual& getVisByEntity(ces::Entity e) {
			return this->visuals[e]; 
		}
		inline std::vector<ces::Position>& getAllPositions() {
			return this->positions; 
		}
		inline std::vector<ces::Velocity>& getAllVelocities() {
			return this->velocities; 
		}
		inline std::vector<ces::Visual>& getAllVisuals() {
			return this->visuals; 
		}

		inline ces::Entity getEntitiesCount() {
			return this->entityCount; 
		}
	};
}

#include "Registry.cpp"