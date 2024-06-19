#pragma once

#include <unordered_set>
#include <memory>
#include <vector>
#include <functional>

#include <glm/vec2.hpp>

/**
* @file Physics.h
* File responsible for game physics
*/

class IGameObject;
class Level;
class FightScreen;

/**
* @namespace Physics Covers all classes responsible for game physics
*/
namespace Physics
{
    /**
    * An enum.
    * Is responsible for the side of the collision
    */
    enum class ECollisionDirection : uint8_t
    {
        Top, ///< enum value Top.
        Bottom, ///< enum value Bottom.
        Left, ///< enum value Left.
        Right ///< enum value Right.
    };

    /**
    * @struct AABB
    * @brief Axis Aligned Bounding Box
    * 
    * responsible for a small contact area
    */
    struct AABB
    {
        /**
        * A constructor
        * @param[in] _bottomLeft glm::vec2&, coordinates of the lower left point
        * @param[in] _topRight glm::vec2&, coordinates of the upper right point
        */
        AABB(const glm::vec2& _bottomLeft, const glm::vec2& _topRight) : bottomLeft(_bottomLeft), topRight(_topRight) {}
        glm::vec2 bottomLeft; ///< coordinates of the lower left point
        glm::vec2 topRight; ///< coordinates of the upper right point
    };

    /**
    * @struct Collider
    * @brief collider texture
    *
    * checks if the texture has a collision
    */
    struct Collider
    {
        /**
        * A constructor. First overload
        * @param[in] _bottomLeft glm::vec2&, coordinates of the lower left point
        * @param[in] _topRight glm::vec2&, coordinates of the upper right point
        * @param[in] _onCollisionCallback std::function<void(const IGameObject&, const ECollisionDirection)>, receives data about which texture received a collision and from which side
        */
        Collider(const glm::vec2& _bottomLeft, const glm::vec2& _topRight, std::function<void(const IGameObject&, const ECollisionDirection)> _onCollisionCallback = {})
            : boundingBox(_bottomLeft, _topRight)
            , isActive(true)
            , onCollisionCallback(_onCollisionCallback) {}

        /**
        * A constructor. Second overload
        * @param[in] _boundingBox AABB&, struct variable, stores four coordinates, lower left and upper right corner
        * @param[in] _onCollisionCallback std::function<void(const IGameObject&, const ECollisionDirection)>, receives data about which texture received a collision and from which side
        */
        Collider(const AABB& _boundingBox, std::function<void(const IGameObject&, const ECollisionDirection)> _onCollisionCallback = {})
            : boundingBox(_boundingBox)
            , isActive(true)
            , onCollisionCallback(_onCollisionCallback) {}

        AABB boundingBox; ///< struct variable. stores four coordinates, lower left and upper right corner
        bool isActive; ///< indicator whether an object has a collision
        std::function<void(const IGameObject&, const ECollisionDirection)> onCollisionCallback; ///< receives data about which texture received a collision and from which side
    };

    /**
    * @class PhysicsEngine
    * @brief adds physics to game objects
    *
    * Class that loads object physics
    */
    class PhysicsEngine {
    public:
        PhysicsEngine() = delete;
        ~PhysicsEngine() = delete;
        PhysicsEngine(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(PhysicsEngine&&) = delete;
        PhysicsEngine(PhysicsEngine&&) = delete;

        /**
        * A static member. Initializes the physics engine in the game
        */
        static void init();

        /**
        * A static member. Destroys the physics engine in the game
        */
        static void terminate();

        /**
        * A static member.
        * @param[in] delta Double, update time.
        */
        static void update(const double delta);

        /**
        * A static member.
        * @param[in] pGameObject std::shared_ptr<IGameObject>, points to dynamic objects.
        */
        static void addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject);

        /**
        * A static member.
        * @param[in] pLevel std::shared_ptr<Level>, Contains the main map.
        */
        static void	setCurrentLevel(std::shared_ptr<Level> pLevel);

        /**
        * A static member.
        * @param[in] pLevel std::shared_ptr<FightScreen>, Contains the pFight map.
        */
        static void	setCurrentFight(std::shared_ptr<FightScreen> pFight);

    private:
        static std::unordered_set<std::shared_ptr<IGameObject>> m_dinamicObjects;
        static std::shared_ptr<Level> m_pCurrentLevel;
        static std::shared_ptr<FightScreen> m_pCurrentFight;

        /**
        * A static member.
        * @param[in] colliders1 Collider&, contains a link to the collider of the first object
        * @param[in] position1 glm::vec2, position of the first object
        * param[in] colliders2 Collider&, contains a link to the collider of the second object
        * @param[in] position1 glm::vec2, position of the second object
        */
        static bool hasIntersection(const Collider& colliders1, const glm::vec2 position1, 
                                    const Collider& colliders2, const glm::vec2 position2);
    };
}