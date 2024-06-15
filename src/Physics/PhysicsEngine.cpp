#include "PhysicsEngine.h"

#include "../Game/GameObject/IGameObject.h"
#include "../Game/GameState/Level.h"

namespace Physics
{
    std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dinamicObjects;
    std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

    void PhysicsEngine::init()
    {

    }

    void PhysicsEngine::terminate()
    {
        m_dinamicObjects.clear();
        m_pCurrentLevel.reset();
    }

    void PhysicsEngine::update(const double delta)
    {
        for (auto& currentDinamicObject : m_dinamicObjects)
        {
            if (currentDinamicObject->getCurrentVelocity() > 0)
            {
                const auto newPosition = currentDinamicObject->getCurrentPosition() + currentDinamicObject->getCurrentDirection() * static_cast<float>(currentDinamicObject->getCurrentVelocity() * delta);
                std::vector<std::shared_ptr<IGameObject>> ObjectToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, newPosition + currentDinamicObject->getSize());

                const auto& colliders = currentDinamicObject->getColliders();
                bool hasCollision = false;

                ECollisionDirection dinamicObjectCollisionDirection = ECollisionDirection::Right;
                if (currentDinamicObject->getCurrentDirection().x < 0) dinamicObjectCollisionDirection = ECollisionDirection::Left;
                else if (currentDinamicObject->getCurrentDirection().y > 0) dinamicObjectCollisionDirection = ECollisionDirection::Top;
                else if (currentDinamicObject->getCurrentDirection().y < 0) dinamicObjectCollisionDirection = ECollisionDirection::Bottom;

                ECollisionDirection objectCollisionDirection = ECollisionDirection::Left;
                if (currentDinamicObject->getCurrentDirection().x < 0) objectCollisionDirection = ECollisionDirection::Right;
                else if (currentDinamicObject->getCurrentDirection().y > 0) objectCollisionDirection = ECollisionDirection::Bottom;
                else if (currentDinamicObject->getCurrentDirection().y < 0) objectCollisionDirection = ECollisionDirection::Top;

                for (const auto& currentDinamicObjectCollider : colliders)
                {
                    for (const auto& currentObjectToCheck : ObjectToCheck)
                    {
                        const auto& collidersToCheck = currentObjectToCheck->getColliders();
                        if (currentObjectToCheck->collides(currentDinamicObject->getObjectType()) && !collidersToCheck.empty())
                        {
                            for (const auto& currentObjectCollider : currentObjectToCheck->getColliders())
                            {
                                if (currentObjectCollider.isActive && hasIntersection(currentDinamicObjectCollider, newPosition, currentObjectCollider, currentObjectToCheck->getCurrentPosition()))
                                {
                                    hasCollision = true;
                                    if (currentObjectCollider.onCollisionCallback)
                                    {
                                        currentObjectCollider.onCollisionCallback(*currentDinamicObject, objectCollisionDirection);
                                    }
                                    if (currentDinamicObjectCollider.onCollisionCallback)
                                    {
                                        currentDinamicObjectCollider.onCollisionCallback(*currentObjectToCheck, dinamicObjectCollisionDirection);
                                    }
                                }
                            }
                        }
                    }
                }

                if (!hasCollision)
                {
                    currentDinamicObject->getCurrentPosition() = newPosition;
                }
                else
                {
                    currentDinamicObject->onCollision();
                }
            }
        }
    }

    void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
    {
        m_dinamicObjects.insert(std::move(pGameObject));
    }

    void PhysicsEngine::setCurrentLevel(std::shared_ptr<Level> pLevel)
    {
        m_pCurrentLevel.swap(pLevel);
    }

    bool PhysicsEngine::hasIntersection(const Collider& colliders1, const glm::vec2 position1, 
                                        const Collider& colliders2, const glm::vec2 position2)
    {
        const glm::vec2 currentCollider1_bottomLeft_world = colliders1.boundingBox.bottomLeft + position1;
        const glm::vec2 currentCollider1_topRight_world = colliders1.boundingBox.topRight + position1;

        const glm::vec2 currentCollider2_bottomLeft_world = colliders2.boundingBox.bottomLeft + position2;
        const glm::vec2 currentCollider2_topRight_world = colliders2.boundingBox.topRight + position2;

        if (currentCollider1_bottomLeft_world.x >= currentCollider2_topRight_world.x)
        {
            return false;
        }
        if (currentCollider1_topRight_world.x <= currentCollider2_bottomLeft_world.x)
        {
            return false;
        }
        if (currentCollider1_bottomLeft_world.y >= currentCollider2_topRight_world.y)
        {
            return false;
        }
        if (currentCollider1_topRight_world.y <= currentCollider2_bottomLeft_world.y)
        {
            return false;
        }
        return true;
    }
}