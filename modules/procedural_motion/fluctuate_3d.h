/*************************************************************************/
/*  fluctuate_3d.h                                                		 */
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

#ifndef FLUCTUATE_3D_H
#define FLUCTUATE_3D_H

#include "scene/3d/node_3d.h"
#include "modules/game_frame/math_defs.h"

class Fluctuate3D : public Node3D {
	GDCLASS(Fluctuate3D, Node3D);

	struct Fluctuation {
		Vector3 axis;
		double time = 0.0;
		double time_offset = 0.0;

		void reset(double p_time, const Vector3 &p_axis);
	};

	const short _fluctuate_axis_count = 3;
	double _fluctuate_axis_lifetime = 1.0;
	short fluctuate_iterations = 3;
	real_t fluctuate_iteration_ratio = GOLDEN_RATIO;
	real_t fluctuate_frequency = 0.5;

	bool translation_enabled = true;
	real_t translation_magnitude = 1.0;
	Vector<Fluctuation> _translation_array;

	Transform3D offset_transform;

	real_t _compute_axis_length(const Vector3 &p_axis, const Vector3 &p_vec) const;

protected:
	void _notification(int p_notification);
	static void _bind_methods();

	Vector3 _compute_axis_vector(const Fluctuation &p_flux) const;

	void _translation_setup();
	void _translation_process(double p_delta);
	void _translation_clear();

public:

	void set_fluctuate_iterations(short p_iterations);
	short get_fluctuate_iterations() const;

	void set_fluctuate_iteration_ratio(real_t p_ratio);
	real_t get_fluctuate_iteration_ratio() const;

	void set_fluctuate_frequency(real_t p_frequency);
	real_t get_fluctuate_frequency() const;

	void set_translation_enabled(bool p_enabled);
	bool get_translation_enabled() const;

	void set_translation_magnitude(real_t p_magnitude);
	real_t get_translation_magnitude() const;

	void set_offset_transform(Transform3D &p_offset);
	Transform3D get_offset_transform() const;

	PackedStringArray get_configuration_warnings() const override;

	Fluctuate3D();
	~Fluctuate3D();
};

#endif // FLUCTUATE_3D_H
