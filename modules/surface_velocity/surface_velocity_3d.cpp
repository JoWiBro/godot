/*************************************************************************/
/*  surface_velocity_3d.cpp                                              */
/*************************************************************************/
/* Copyright (c) 2007-2022 Joshua Brodie.                                */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "surface_velocity_3d.h"

void SurfaceVelocity3D::set_surface_speed(double p_speed) { surface_speed = p_speed; }

double SurfaceVelocity3D::get_surface_speed() const { return surface_speed; }

void SurfaceVelocity3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_surface_speed", "speed"), &SurfaceVelocity3D::set_surface_speed);
	ClassDB::bind_method(D_METHOD("get_surface_speed"), &SurfaceVelocity3D::get_surface_speed);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "surface_speed"), "set_surface_speed", "get_surface_speed");
}

PackedStringArray SurfaceVelocity3D::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (physics_body == nullptr) {
		warnings.push_back(RTR("Parent node has no physics body, so the surface velocity can not be applied to it.\nMake sure that the parent node inherits from the PhysicsBody3D node."));
	}

	return warnings;
}

SurfaceVelocity3D::SurfaceVelocity3D() : Node() {}

SurfaceVelocity3D::~SurfaceVelocity3D() {}



void SurfaceVelocityConveyor3D::_set_callable_enebled(bool enabled) {

    if(enabled == is_callable_enabled) return;
    is_callable_enabled = enabled;

    if(physics_body != nullptr) {
        if(is_callable_enabled) {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), callable_mp(this, &SurfaceVelocityConveyor3D::_compute_linear_surface_velocity_conveyor));
        } else {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), Callable());
        }
    }
}

void SurfaceVelocityConveyor3D::_notification(int p_notification) {
    switch(p_notification) {
        case NOTIFICATION_ENTER_TREE: {
            physics_body = Object::cast_to<PhysicsBody3D>(get_parent());
            _set_callable_enebled(is_enabled());

            break;
        }
        case NOTIFICATION_EXIT_TREE: {
            _set_callable_enebled(false);

            break;
        }
        case NOTIFICATION_ENABLED: {
            _set_callable_enebled(true);

            break;
        }
        case NOTIFICATION_DISABLED: {
            _set_callable_enebled(false);

            break;
        }
        default:
            break;
    };

    SurfaceVelocity3D::_notification(p_notification);
}

void SurfaceVelocityConveyor3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_conveyor_axis", "axis"), &SurfaceVelocityConveyor3D::set_conveyor_axis);
	ClassDB::bind_method(D_METHOD("get_conveyor_axis"), &SurfaceVelocityConveyor3D::get_conveyor_axis);
    ClassDB::bind_method(D_METHOD("set_conveyor_axis_relative", "relative"), &SurfaceVelocityConveyor3D::set_conveyor_axis_relative);
	ClassDB::bind_method(D_METHOD("get_conveyor_axis_relative"), &SurfaceVelocityConveyor3D::get_conveyor_axis_relative);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "conveyor_axis"), "set_conveyor_axis", "get_conveyor_axis");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "conveyor_axis_relative"), "set_conveyor_axis_relative", "get_conveyor_axis_relative");
}

void SurfaceVelocityConveyor3D::set_conveyor_axis(const Vector3 &p_axis) {
    conveyor_axis = p_axis;
}

Vector3 SurfaceVelocityConveyor3D::get_conveyor_axis() const {
    return conveyor_axis;
}

void SurfaceVelocityConveyor3D::set_conveyor_axis_relative(bool p_relative) {
    conveyor_axis_relative = p_relative;
}

bool SurfaceVelocityConveyor3D::get_conveyor_axis_relative() const {
    return conveyor_axis_relative;
}

Vector3 SurfaceVelocityConveyor3D::_compute_linear_surface_velocity_conveyor(const Vector3 &p_pos, const Vector3 &p_normal) {

    Vector3 axis = conveyor_axis;

    if(conveyor_axis_relative && physics_body != nullptr) {
        axis = physics_body->get_basis().xform(axis);
    }

	return axis.cross(p_normal).normalized() * get_surface_speed();
}

SurfaceVelocityConveyor3D::SurfaceVelocityConveyor3D() : SurfaceVelocity3D() {}

SurfaceVelocityConveyor3D::~SurfaceVelocityConveyor3D() {
    _set_callable_enebled(false);
}



void SurfaceVelocityBurrow3D::_set_callable_enebled(bool enabled) {

    if(enabled == is_callable_enabled) return;
    is_callable_enabled = enabled;

    if(physics_body != nullptr) {
        if(is_callable_enabled) {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), callable_mp(this, &SurfaceVelocityBurrow3D::_compute_linear_surface_velocity_burrow));
        } else {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), Callable());
        }
    }
}

void SurfaceVelocityBurrow3D::_notification(int p_notification) {
    switch(p_notification) {
        case NOTIFICATION_ENTER_TREE: {
            physics_body = Object::cast_to<PhysicsBody3D>(get_parent());
            _set_callable_enebled(is_enabled());

            break;
        }
        case NOTIFICATION_EXIT_TREE: {
            _set_callable_enebled(false);

            break;
        }
        case NOTIFICATION_ENABLED: {
            _set_callable_enebled(true);

            break;
        }
        case NOTIFICATION_DISABLED: {
            _set_callable_enebled(false);

            break;
        }
        default:
            break;
    };

    SurfaceVelocity3D::_notification(p_notification);
}

void SurfaceVelocityBurrow3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_burrow_direction", "direction"), &SurfaceVelocityBurrow3D::set_burrow_direction);
	ClassDB::bind_method(D_METHOD("get_burrow_direction"), &SurfaceVelocityBurrow3D::get_burrow_direction);
    ClassDB::bind_method(D_METHOD("set_burrow_direction_relative", "relative"), &SurfaceVelocityBurrow3D::set_burrow_direction_relative);
	ClassDB::bind_method(D_METHOD("get_burrow_direction_relative"), &SurfaceVelocityBurrow3D::get_burrow_direction_relative);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "burrow_direction"), "set_burrow_direction", "get_burrow_direction");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "burrow_direction_relative"), "set_burrow_direction_relative", "get_burrow_direction_relative");
}

void SurfaceVelocityBurrow3D::set_burrow_direction(const Vector3 &p_dir) {
    burrow_direction = p_dir;
}

Vector3 SurfaceVelocityBurrow3D::get_burrow_direction() const {
    return burrow_direction;
}

void SurfaceVelocityBurrow3D::set_burrow_direction_relative(bool p_relative) {
    burrow_direction_relative = p_relative;
}

bool SurfaceVelocityBurrow3D::get_burrow_direction_relative() const {
    return burrow_direction_relative;
}

Vector3 SurfaceVelocityBurrow3D::_compute_linear_surface_velocity_burrow(const Vector3 &p_pos, const Vector3 &p_normal) {

    Vector3 dir = burrow_direction;

    if(burrow_direction_relative && physics_body == nullptr) {
        dir = physics_body->get_basis().xform(dir);
    }

	dir.normalize();
	dir -= p_normal * p_normal.dot(dir);

	return dir * get_surface_speed();
}

SurfaceVelocityBurrow3D::SurfaceVelocityBurrow3D() : SurfaceVelocity3D() {}

SurfaceVelocityBurrow3D::~SurfaceVelocityBurrow3D() {
    _set_callable_enebled(false);
}



void SurfaceVelocityWalk3D::_set_callable_enebled(bool enabled) {

    if(enabled == is_callable_enabled) return;
    is_callable_enabled = enabled;

    if(physics_body != nullptr) {
        if(is_callable_enabled) {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), callable_mp(this, &SurfaceVelocityWalk3D::_compute_linear_surface_velocity_walk));
            //PhysicsServer3D::get_singleton()->body_set_compute_angular_surface_velocity(physics_body->get_rid(), callable_mp(this, &SurfaceVelocityWalk3D::_compute_angular_surface_velocity_walk));

        } else {
            PhysicsServer3D::get_singleton()->body_set_compute_linear_surface_velocity(physics_body->get_rid(), Callable());
            //PhysicsServer3D::get_singleton()->body_set_compute_angular_surface_velocity(physics_body->get_rid(), Callable());
        }
    }
}

void SurfaceVelocityWalk3D::_notification(int p_notification) {
    switch(p_notification) {
        case NOTIFICATION_ENTER_TREE: {
            physics_body = Object::cast_to<PhysicsBody3D>(get_parent());
            _set_callable_enebled(is_enabled());

            break;
        }
        case NOTIFICATION_EXIT_TREE: {
            _set_callable_enebled(false);

            break;
        }
        case NOTIFICATION_ENABLED: {
            _set_callable_enebled(true);

            break;
        }
        case NOTIFICATION_DISABLED: {
            _set_callable_enebled(false);

            break;
        }
        default:
            break;
    };

    Node::_notification(p_notification);
}

void SurfaceVelocityWalk3D::_bind_methods() {
    // Walk direction
    ClassDB::bind_method(D_METHOD("set_walk_direction", "direction"), &SurfaceVelocityWalk3D::set_walk_direction);
	ClassDB::bind_method(D_METHOD("get_walk_direction"), &SurfaceVelocityWalk3D::get_walk_direction);
    ClassDB::bind_method(D_METHOD("set_walk_direction_relative", "relative"), &SurfaceVelocityWalk3D::set_walk_direction_relative);
	ClassDB::bind_method(D_METHOD("get_walk_direction_relative"), &SurfaceVelocityWalk3D::get_walk_direction_relative);

    //walk up
    ClassDB::bind_method(D_METHOD("set_walk_up", "up"), &SurfaceVelocityWalk3D::set_walk_up);
	ClassDB::bind_method(D_METHOD("get_walk_up"), &SurfaceVelocityWalk3D::get_walk_up);
    ClassDB::bind_method(D_METHOD("set_walk_up_relative", "relative"), &SurfaceVelocityWalk3D::set_walk_up_relative);
	ClassDB::bind_method(D_METHOD("get_walk_up_relative"), &SurfaceVelocityWalk3D::get_walk_up_relative);

    ClassDB::bind_method(D_METHOD("set_walk_turn_speed", "turn"), &SurfaceVelocityWalk3D::set_walk_turn_speed);
	ClassDB::bind_method(D_METHOD("get_walk_turn_speed"), &SurfaceVelocityWalk3D::get_walk_turn_speed);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "walk_direction"), "set_walk_direction", "get_walk_direction");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "walk_direction_relative"), "set_walk_direction_relative", "get_walk_direction_relative");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "walk_up"), "set_walk_up", "get_walk_up");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "walk_up_relative"), "set_walk_up_relative", "get_walk_up_relative");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_turn_speed"), "set_walk_turn_speed", "get_walk_turn_speed");
}

void SurfaceVelocityWalk3D::set_walk_direction(const Vector3 &p_dir) {
    walk_direction = p_dir;
}

Vector3 SurfaceVelocityWalk3D::get_walk_direction() const {
    return walk_direction;
}

void SurfaceVelocityWalk3D::set_walk_direction_relative(bool p_relative) {
    walk_direction_relative = p_relative;
}

bool SurfaceVelocityWalk3D::get_walk_direction_relative() const {
    return walk_direction_relative;
}

void SurfaceVelocityWalk3D::set_walk_up(const Vector3 &p_up) {
    walk_up = p_up;
}

Vector3 SurfaceVelocityWalk3D::get_walk_up() const {
    return walk_up;
}

void SurfaceVelocityWalk3D::set_walk_up_relative(bool p_relative) {
    walk_up_relative = p_relative;
}

bool SurfaceVelocityWalk3D::get_walk_up_relative() const {
    return walk_up_relative;
}

void SurfaceVelocityWalk3D::set_walk_turn_speed(real_t p_turn) {
    walk_turn_speed = p_turn;
}

real_t SurfaceVelocityWalk3D::get_walk_turn_speed() const {
    return walk_turn_speed;
}

Vector3 SurfaceVelocityWalk3D::_compute_linear_surface_velocity_walk(const Vector3 &p_pos, const Vector3 &p_normal) {

    Vector3 dir = walk_direction;
    Vector3 up = walk_up;

    if(physics_body != nullptr) {
        if(walk_direction_relative) {
            dir = physics_body->get_basis().xform(dir);
        }
        if(walk_up_relative) {
            up = physics_body->get_basis().xform(up);
        }
    }

    up.normalize();
    real_t up_norm_dot = MAX(0.0, -up.dot(p_normal));
    dir -= up * up.dot(dir);

	dir.normalize();
	dir -= p_normal * p_normal.dot(dir);

	return dir * get_surface_speed() * up_norm_dot;
}
/*
Vector3 SurfaceVelocityWalk3D::_compute_angular_surface_velocity_walk(const Vector3 &p_pos, const Vector3 &p_normal) {

    Vector3 up = walk_up;

    if(physics_body != nullptr && walk_up_relative) {
        up = physics_body->get_basis().xform(up);
    }

    up.normalize();
    real_t up_norm_dot = MAX(0.0, -up.dot(p_normal));

    return up * up_norm_dot * get_walk_turn_speed();
}
*/
SurfaceVelocityWalk3D::SurfaceVelocityWalk3D() : SurfaceVelocity3D() {}

SurfaceVelocityWalk3D::~SurfaceVelocityWalk3D() {
    _set_callable_enebled(false);
}