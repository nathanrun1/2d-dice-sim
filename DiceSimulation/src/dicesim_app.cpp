#include "dicesim_app.hpp"

#include "dicesim_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <box2d.h>

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace dice {

    DiceSimApp::DiceSimApp() { loadGameObjects(); }

    DiceSimApp::~DiceSimApp() {}

    void DiceSimApp::run() {
        SimpleRenderSystem simpleRenderSystem{ diceSimDevice, diceSimRenderer.getSwapChainRenderPass() };
        DiceSimCamera camera{};
        camera.setViewDirection(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.0f, 0.f, 1.f));


        while (!diceSimWindow.shouldClose()) {
            glfwPollEvents();

            float aspect = diceSimRenderer.getAspectRatio();
            camera.setOrthographicProjection(-aspect * 20, aspect * 20, -20, 20, -1, 10);
            //camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = diceSimRenderer.beginFrame()) {
                diceSimRenderer.beginSwapChainRenderPass(commandBuffer);

                diceSimPhysicsWorld.step(1.0f / 60.0f, 10); // TEMP: Time step hardcoded temporary (will cause simulation speed changes)

                // Drop dice again when user presses 'R' key
                if (glfwGetKey(diceSimWindow.getGLFWWindow(), GLFW_KEY_R) == GLFW_PRESS) {
                    dropDice();
                    while (glfwGetKey(diceSimWindow.getGLFWWindow(), GLFW_KEY_R) == GLFW_PRESS) {
                        glfwPollEvents();
                    }
                }

                

                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

                diceSimRenderer.endSwapChainRenderPass(commandBuffer);
                diceSimRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(diceSimDevice.device());
    }

    std::unique_ptr<DiceSimModel> createRectangleModel2D(DiceSimDevice& device, glm::vec3 offset, float halfWidth = 0.5f, float halfHeight = 0.5f, glm::vec3 color = {0.1f, 0.1f, 0.8f}) {
        std::vector<DiceSimModel::Vertex> vertices{
            {{-halfWidth, -halfHeight, .0f}, color},
            {{halfWidth, halfHeight, .0f}, color},
            {{-halfWidth, halfHeight, .0f}, color},
            {{-halfWidth, -halfHeight, .0f}, color},
            {{halfWidth, -halfHeight, .0f}, color},
            {{halfWidth, halfHeight, .0f}, color}
        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<DiceSimModel>(device, vertices);
    }

    std::unique_ptr<DiceSimModel> createQuadModel2D(DiceSimDevice& device, glm::vec3 offset, glm::vec2 topRight, glm::vec2 topLeft, glm::vec2 botLeft, glm::vec2 botRight, glm::vec3 color = { 0.1f, 0.1f, 0.8f }) {
        std::vector<DiceSimModel::Vertex> vertices{
            {{topLeft.x, topLeft.y, .0f}, color},
            {{botRight.x, botRight.y, .0f}, color},
            {{topRight.x, topRight.y, .0f}, color},
            {{topLeft.x, topLeft.y, .0f}, color},
            {{botLeft.x, botLeft.y, .0f}, color},
            {{botRight.x, botRight.y, .0f}, color}
        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<DiceSimModel>(device, vertices);
    }

    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<DiceSimModel> createCubeModel(DiceSimDevice& device, glm::vec3 offset) {
        std::vector<DiceSimModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<DiceSimModel>(device, vertices);
    }

    void DiceSimApp::dropDice() {
        static float dropRotationDeg = 0.0f;

        TransformComponent transform{};
        transform.rotation = { 0.0f, 0.0f, glm::radians<float>(dropRotationDeg += 10.0f) };
        transform.translation = { .0f, .0f, .0f };
        transform.scale = { 1.0f, 1.0f, 1.0f };
        
        diceRigidBody.lock()->setTransform(transform);
    }

    void DiceSimApp::loadDice() {
        // Vertices defining dice quadrilateral, ordered top right to bottom right (same as cartesian quadrant order)
        std::vector<glm::vec2> diceQuadVertices = {
            {6.f, -3.f},
            {-2.f, -3.f},
            {-3.f, 3.f},
            {3.f, 2.f}
        };

        // Convert vertices to Box2D polygon
        std::vector<b2Vec2> diceQuadb2Points{};
        for (const glm::vec2& vertex : diceQuadVertices) {
            diceQuadb2Points.push_back(b2Vec2{ -vertex.x, -vertex.y }); // Flip x & y axis due to Vulkan - Box2D coordinate differences
        }
        b2Hull diceHull = b2ComputeHull(diceQuadb2Points.data(), 4);
        b2Polygon dicePolygon = b2MakePolygon(&diceHull, .0f);

        // Create rendered dice gameobject
        std::shared_ptr<DiceSimModel> diceModel = createQuadModel2D(diceSimDevice, { .0f, .0f, .0f },
            diceQuadVertices[0], diceQuadVertices[1], diceQuadVertices[2], diceQuadVertices[3]);

        TransformComponent diceTransform{};
        diceTransform.rotation = { 0.0f, 0.0f, glm::radians(0.f) };
        diceTransform.translation = { .0f, 0.0f, .0f };
        diceTransform.scale = { 1.0f, 1.0f, 1.0f };

        auto diceGameObject = DiceSimGameObject::createGameObject();
        diceGameObject.transform = diceTransform;
        diceGameObject.model = diceModel;
        gameObjects.push_back(std::make_shared<DiceSimGameObject>(std::move(diceGameObject)));

        // Create the dynamic rigidbody
        DiceSimRigidBodyConfigInfo rigidBodyInfo{};
        rigidBodyInfo.pGameObject = gameObjects.back().get();
        rigidBodyInfo.pTransform = &diceTransform;
        rigidBodyInfo.pShape = &dicePolygon;
        rigidBodyInfo.isDynamic = true;

        diceRigidBody = diceSimPhysicsWorld.createRigidBody(rigidBodyInfo);
    }

    void DiceSimApp::loadGround() {
        // Ground is rendered as a large rectangle covering bottom area of the screen
        std::shared_ptr<DiceSimModel> groundModel = createRectangleModel2D(diceSimDevice, { .0f, .0f, .0f }, 50.0f, 10.0f,
            { 0.8f, 0.1f, 0.1f });

        TransformComponent groundTransform{};
        groundTransform.rotation = { 0.0f, 0.0f, 0.0f };
        groundTransform.translation = { .0f, 20.0f, .0f }; // Offset by 20 down
        groundTransform.scale = { 1.0f, 1.0f, 1.0f };

        b2Polygon groundPolygon = b2MakeBox(50.0f, 10.0f);

        auto groundGameObject = DiceSimGameObject::createGameObject();
        groundGameObject.transform = groundTransform;
        groundGameObject.model = groundModel;
        gameObjects.push_back(std::make_shared<DiceSimGameObject>(std::move(groundGameObject)));

        DiceSimRigidBodyConfigInfo rigidBodyInfo{};
        rigidBodyInfo.pGameObject = gameObjects.back().get();
        rigidBodyInfo.pTransform = &groundTransform;
        rigidBodyInfo.pShape = &groundPolygon;
        rigidBodyInfo.isDynamic = false;

        // Create the static rigidbody
        diceSimPhysicsWorld.createRigidBody(rigidBodyInfo);
    }

    void DiceSimApp::loadGameObjects() {
        loadDice();
        loadGround();
    }

}  // namespace diceSim