# 2D Physics Dice Simulation in Vulkan

This is a project I've been working on in relation to a separate project I did in highschool. The original project was aimed at exploring the relationship between the shape of an unfairly shaped d6 (6 sided dice), and the probability
of it landing on different numbers.

In highschool, I initially created a Unity program that simulates some arbitrary dice shape for a large amount of rolls, and calculates the probability of each number experimentally. Unfortunately, due to Unity's Physics system
being non-deterministic (and me not knowing this at the time), the results were not accurate.

I recently decided to re-explore this, but this time with a much lower level implementation. I'm currently interested in learning graphics programming with low level graphics APIs, so I decided to use Vulkan and run my own custom physics simulations.
Currently, the simulations use the [Box2D](https://box2d.org/) physics library for 2D physics. This library is not deterministic, but it is accurate enough for an individual dice roll. I aim to expand on this by writing my own 2D physics engine and integrating it into this program, but for now Box2D is used.

A lot of the application code was done by following [this youtube tutorial series](https://www.youtube.com/watch?v=Y9U9IE0gVHA&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR), up until the implementation of camera view transforms.
I modified the code so that it uses orthographic projection (necessary for 2D), and implemented additional [RigidBody](https://github.com/nathanrun1/2d-dice-sim/blob/main/DiceSimulation/src/dicesim_rigidbody.hpp) and [PhysicsWorld](https://github.com/nathanrun1/2d-dice-sim/blob/main/DiceSimulation/src/dicesim_physics_world.hpp) classes to integrate the 2D physics.

## Current Results

The program currently renders a simple 2D scene, with a 4-sided "dice" and a surface to fall on. The user can press 'R' to reset the dice's position with a new rotation, and drop it again. 
To be able to properly analyse how the unfair shape of the dice relates to the probability of the sides, I allow the vertices defining the dice to be customized, which gives the ability to create any arbitrary quadrilateral.
The dice vertices are currently modifiable in the dicesim_app.cpp file under the loadDice() function.

The scene also uses a [coefficient of restitution](https://en.wikipedia.org/wiki/Coefficient_of_restitution) of 0 (i.e. no bouncing), as this is an property of the dice rolls in my initial project that I assume for simplicity.

https://github.com/user-attachments/assets/14298dc7-b85b-4701-ab0b-37e325dc87b9

## Future Plans

- Implement automatic dice rolls at various starting rotations to quickly calculate the probability of each dice roll result for quick analysis.
- Implement 3D physics with the [Jolt Physics](https://github.com/jrouwe/JoltPhysics) library to be able to use this with the originally intended 6-sided die (and then other arbitrary shapes)
- Implement custom deterministic 3D physics calculations as an additional challenge to improve my knowledge in creating physics engines and Vulkan in general.
