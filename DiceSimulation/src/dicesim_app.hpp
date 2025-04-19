#pragma once

#include "dicesim_device.hpp"
#include "dicesim_game_object.hpp"
#include "dicesim_renderer.hpp"
#include "dicesim_window.hpp"
#include "dicesim_physics_world.hpp"

#include "box2d.h"

// std
#include <memory>
#include <vector>

namespace dice {
	class DiceSimApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		DiceSimApp();
		~DiceSimApp();

		DiceSimApp(const DiceSimApp&) = delete;
		DiceSimApp& operator=(const DiceSimApp&) = delete;

		void run();

	private:
		// Drops dice again from initial position, rotated by 10 degrees counter-clockwise
		void dropDice();

		void loadGameObjects();
		void loadDice();
		void loadGround();

		DiceSimWindow diceSimWindow{ WIDTH, HEIGHT, "Dice Sim" };
		DiceSimDevice diceSimDevice{ diceSimWindow };
		DiceSimRenderer diceSimRenderer{ diceSimWindow, diceSimDevice };
		DiceSimPhysicsWorld diceSimPhysicsWorld{};

		std::weak_ptr<DiceSimRigidBody> diceRigidBody;

		std::vector<std::shared_ptr<DiceSimGameObject>> gameObjects;
	};
}  // namespace dice