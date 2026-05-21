/*
 *	Name: CollisionShapeType
 *	Author: Pawel Mrochen
 */

#pragma once

namespace physics {

enum class CollisionShapeType
{
	NONE,
	PLANE,
	AXIS_ALIGNED_BOX,
	ORIENTED_BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	CONE,
	TORUS,
	CONVEX_HULL,
	TRIANGLE_MESH,
	MESH,
	HEIGHTFIELD
};

} // namespace physics
