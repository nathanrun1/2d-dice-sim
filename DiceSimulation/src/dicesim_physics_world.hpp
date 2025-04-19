#pragma once

#include "dicesim_rigidbody.hpp"

// box2d
#include <box2d.h>

namespace dice {
	class DiceSimPhysicsWorld {
	public:
		DiceSimPhysicsWorld();
		~DiceSimPhysicsWorld();

		DiceSimPhysicsWorld(const DiceSimPhysicsWorld&) = delete;
		void operator=(const DiceSimPhysicsWorld&) = delete;
		DiceSimPhysicsWorld(DiceSimPhysicsWorld&&) = delete;
		DiceSimPhysicsWorld& operator=(DiceSimPhysicsWorld&&) = delete;

		b2WorldId& worldId();

		std::weak_ptr<DiceSimRigidBody> createRigidBody(DiceSimRigidBodyConfigInfo& configInfo);
		void step(float timeStep, int subStepCount);
	private:
		b2WorldId worldId_;
		std::vector<std::shared_ptr<DiceSimRigidBody>> rigidbodies_;
	};
}