#pragma once

#include "dicesim_game_object.hpp"

// box2d
#include <box2d.h>

// stl
#define _USE_MATH_DEFINES
#include <cmath>

namespace dice {
	class DiceSimRigidBody {
	public:

		DiceSimRigidBody(b2WorldId& worldId, DiceSimGameObject& gameObject, TransformComponent& initialTransform, b2Polygon& shape, bool isDynamic = false);

		DiceSimRigidBody(const DiceSimRigidBody&) = delete;
		DiceSimRigidBody& operator=(const DiceSimRigidBody&) = delete;
		DiceSimRigidBody(DiceSimRigidBody&&) = default;
		DiceSimRigidBody& operator=(DiceSimRigidBody&&) = default;

		// Updates the position of the rigidbody after most recent world step.
		void update();

		// Sets transform of rigidbody to new value
		void setTransform(TransformComponent transform);
	private:
		bool isDynamic_;
		DiceSimGameObject& gameObject_;
		b2WorldId& worldId_;
		b2BodyId bodyId_;
	};
}