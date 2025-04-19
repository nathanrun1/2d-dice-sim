#include "dicesim_rigidbody.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace dice {

	DiceSimRigidBody::DiceSimRigidBody(b2WorldId worldId, DiceSimRigidBodyConfigInfo& configInfo)
		: gameObject_{ *configInfo.pGameObject }, worldId_ { worldId }, isDynamic_{ configInfo.isDynamic } {
		
		gameObject_.transform = *configInfo.pTransform;

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.position = { configInfo.pTransform->translation.x, -configInfo.pTransform->translation.y };
		bodyDef.rotation = b2MakeRot(configInfo.pTransform->rotation.z);
		if (isDynamic_) bodyDef.type = b2_dynamicBody;

		bodyId_ = b2CreateBody(worldId_, &bodyDef);

		b2SurfaceMaterial material = b2DefaultSurfaceMaterial();
		material.friction = 0.0f;
		material.restitution = 0.f;

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.material = material;
		shapeDef.density = 5.0f;
		if (isDynamic_) {
			// Arbitrary default density and friction values
			shapeDef.density = 1.0f;
		}

		b2CreatePolygonShape(bodyId_, &shapeDef, configInfo.pShape);
	}

	void DiceSimRigidBody::update() {
		if (!isDynamic_) return;

		b2Vec2 position = b2Body_GetPosition(bodyId_);
		b2Rot rotation = b2Body_GetRotation(bodyId_);
		gameObject_.transform.translation.x = position.x;
		gameObject_.transform.translation.y = -1 * position.y;
		gameObject_.transform.rotation.z = b2Rot_GetAngle(rotation);
	}

	void DiceSimRigidBody::setTransform(TransformComponent transform) {
		b2Body_SetAwake(bodyId_, false);
		b2Body_SetTransform(bodyId_, { transform.translation.x, -transform.translation.y }, b2MakeRot(transform.rotation.z));
		b2Body_SetLinearVelocity(bodyId_, { 0.0f, 0.0f });
		b2Body_SetAngularVelocity(bodyId_, 0.0f);
		b2Body_SetAwake(bodyId_, true);

		update();
	}

}