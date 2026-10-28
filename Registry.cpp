#pragma once
#include "Registry.h"

inline ces::Registry::Registry() {
	this->entityCount = 0;

	this->positions.resize(maxEntities);
	this->velocities.resize(maxEntities);
	this->visuals.resize(maxEntities);

	for (int i = maxEntities - 1; i >= 0; i--) {
		this->freeIds.emplace_back(static_cast<ces::Entity>(i));

		this->positions[i] = {};
		this->velocities[i] = {};
		this->visuals[i] = {};
	}
}

inline ces::Registry::~Registry() {
	this->entityCount = 0;
}

inline ces::Entity ces::Registry::createEntity() {
	if (!this->freeIds.empty() && this->entityCount < maxEntities) {
		ces::Entity id = this->freeIds.back();
		this->freeIds.pop_back();

		this->entityCount++;
		return id;
	}
	return static_cast<ces::Entity>(-1);
}

inline void ces::Registry::destroyEntity(ces::Entity e) {
	this->positions[e] = {};
	this->velocities[e] = {};
	this->visuals[e] = {};

	this->freeIds.push_back(e);
	this->entityCount--;
}