#include "dicesim_physics_world.hpp"

namespace dice {

	DiceSimPhysicsWorld::DiceSimPhysicsWorld() {
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -10.0f };
		worldId_ = b2CreateWorld(&worldDef);
	}

	DiceSimPhysicsWorld::~DiceSimPhysicsWorld() {
		b2DestroyWorld(worldId_);
	}
	
	b2WorldId& DiceSimPhysicsWorld::worldId() {
		return worldId_;
	}

	std::weak_ptr<DiceSimRigidBody> DiceSimPhysicsWorld::addRigidBody(DiceSimGameObject& gameObject, TransformComponent& transform, b2Polygon& shape, bool isDynamic) {
		auto rigidBodyPtr = std::make_shared<DiceSimRigidBody>(
			worldId_, 
			gameObject, 
			transform, 
			shape, 
			isDynamic);
		
		rigidbodies_.push_back(rigidBodyPtr);
		return rigidBodyPtr;
	}
	void DiceSimPhysicsWorld::step(float timeStep, int subStepCount) {
		b2World_Step(worldId_, timeStep, subStepCount);
		for (auto& rigidBody : rigidbodies_) {
			rigidBody->update();
		}
	}
}