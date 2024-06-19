#pragma once

#include <glm/vec2.hpp>

#include "../../Physics/PhysicsEngine.h"

/**
* @file IGameObject.h
* the file contains general data about game objects
*/

/**
* @class IGameObject
* @brief Virtual class
* 
* The class renders, updates, sets the size and collision of game objects
*/
class IGameObject
{
public:

	/**
    * An enum.
    * Contains information about the type of game objects
    */
	enum class EObjectType
	{
		BrickRoad, ///< enum value BrickRoad
		Border, ///< enum value Border
		Hero, ///< enum value Hero
		House, ///< enum value House
		LeftHouseOne, ///< enum value LeftHouseOne
		LeftHouseTwo, ///< enum value LeftHouseTwo
		RightHouseOne, ///< enum value RightHouseOne
		RightHouseTwo, ///< enum value RightHouseTwo
		Water, ///< enum value Water
		Arrow,///< enum value Arrow

		Unknown ///< enum value BrickRoad
	};

	/**
	* A constructor
	* @param[in] objectType EObjectType objectType
	* @param[in] position glm::vec2& position
	* @param[in] size glm::vec2& size
	* @param[in] rotation float rotation
	* @param[in] layer float layer
	*/
	IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	/**
	* A virtual disconstructor
	*/
	virtual ~IGameObject();

	/**
	* A full virtual member
	*/
	virtual void render() const = 0;

	/**
	* A virtual member. Responsible for updating in the current frame
	* @param[in] delta Double update time.
	*/
	virtual void update(const double delta) {};

	/**
	* A virtual member.
	* @return object position
	*/
	virtual glm::vec2& getCurrentPosition() { return m_position; }

	/**
	* A virtual member.
	* @return object direction
	*/
	virtual glm::vec2& getCurrentDirection() { return m_direction; }

	/**
	* A virtual member.
	* @return object velocity
	*/
	virtual double getCurrentVelocity() { return m_velocity; }

	/**
	* A virtual member.
	* @param[in] velocity double velocity
	*/
	virtual void setVelocity(const double velocity);

	/**
	* A const member.
	* @return object size
	*/
	const glm::vec2& getSize() { return m_size; }

	/**
	* A const member.
	* @return object Collider
	*/
	const std::vector<Physics::Collider>& getColliders() { return m_colliders; }

	/**
	* A enum member.
	* @return object type
	*/
	EObjectType getObjectType() const { return m_objectType; }

	/**
	* A virtual member.
	* @return collision confirmation
	*/
	virtual bool collides(const EObjectType objectType) { return true; }

	/**
	* A virtual member.
	*/
	virtual void onCollision() {}

	/**
	* A virtual member.
	* @return is it possible to interact with an object
	*/
	virtual bool isActive() const { return m_isActive; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
	EObjectType m_objectType;

	glm::vec2 m_direction;
	double m_velocity;
	std::vector<Physics::Collider> m_colliders;

	bool m_isActive;
};