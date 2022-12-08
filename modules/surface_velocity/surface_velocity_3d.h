/*************************************************************************/
/*  surface_velocity_3d.h                                                */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
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

#ifndef SURFACE_VELOCITY_3D_H
#define SURFACE_VELOCITY_3D_H

#include "scene/3d/physics_body_3d.h"
#include "scene/main/node.h"

class SurfaceVelocity3D : public Node {
	GDCLASS(SurfaceVelocity3D, Node);

	double surface_speed = 1.0;

protected:

	PhysicsBody3D *physics_body = nullptr;

	static void _bind_methods();

public:

	void set_surface_speed(double p_speed);
	double get_surface_speed() const;

	PackedStringArray get_configuration_warnings() const override;

	SurfaceVelocity3D();
	~SurfaceVelocity3D();
};

class SurfaceVelocityConveyor3D: public SurfaceVelocity3D {
	GDCLASS(SurfaceVelocityConveyor3D, SurfaceVelocity3D);

	Vector3 conveyor_axis = Vector3(0.0, 0.0, 1.0);
	bool conveyor_axis_relative = false;

	bool is_callable_enabled = false;
	void _set_callable_enebled(bool enabled);

protected:
	void _notification(int p_notification);
	static void _bind_methods();

	Vector3 _compute_linear_surface_velocity_conveyor(const Vector3 &p_pos, const Vector3 &p_normal);

public:

	void set_conveyor_axis(const Vector3 &p_axis);
	Vector3 get_conveyor_axis() const;

	void set_conveyor_axis_relative(bool p_relative);
	bool get_conveyor_axis_relative() const;

	SurfaceVelocityConveyor3D();
	~SurfaceVelocityConveyor3D();
};

class SurfaceVelocityBurrow3D: public SurfaceVelocity3D {
	GDCLASS(SurfaceVelocityBurrow3D, SurfaceVelocity3D);

	Vector3 burrow_direction = Vector3(1.0, 0.0, 0.0);
	bool burrow_direction_relative = false;
	
	bool is_callable_enabled = false;
	void _set_callable_enebled(bool enabled);

protected:
	void _notification(int p_notification);
	static void _bind_methods();

	Vector3 _compute_linear_surface_velocity_burrow(const Vector3 &p_pos, const Vector3 &p_normal);

public:

	void set_burrow_direction(const Vector3 &p_dir);
	Vector3 get_burrow_direction() const;

	void set_burrow_direction_relative(bool p_relative);
	bool get_burrow_direction_relative() const;

	SurfaceVelocityBurrow3D();
	~SurfaceVelocityBurrow3D();
};


class SurfaceVelocityWalk3D: public SurfaceVelocity3D {
	GDCLASS(SurfaceVelocityWalk3D, SurfaceVelocity3D);

	Vector3 walk_direction = Vector3(1.0, 0.0, 0.0);
	bool walk_direction_relative = false;
	
	Vector3 walk_up = Vector3(0.0, 1.0, 0.0);
	bool walk_up_relative = false;

	real_t walk_turn_speed = 0.0;
	
	bool is_callable_enabled = false;
	void _set_callable_enebled(bool enabled);

protected:
	void _notification(int p_notification);
	static void _bind_methods();

	Vector3 _compute_linear_surface_velocity_walk(const Vector3 &p_pos, const Vector3 &p_normal);
	//Vector3 _compute_angular_surface_velocity_walk(const Vector3 &p_pos, const Vector3 &p_normal);

public:

	void set_walk_direction(const Vector3 &p_dir);
	Vector3 get_walk_direction() const;

	void set_walk_direction_relative(bool p_relative);
	bool get_walk_direction_relative() const;

	void set_walk_up(const Vector3 &p_up);
	Vector3 get_walk_up() const;

	void set_walk_up_relative(bool p_relative);
	bool get_walk_up_relative() const;

	void set_walk_turn_speed(real_t p_turn);
	real_t get_walk_turn_speed() const;

	SurfaceVelocityWalk3D();
	~SurfaceVelocityWalk3D();
};

#endif // SURFACE_VELOCITY_3D_H