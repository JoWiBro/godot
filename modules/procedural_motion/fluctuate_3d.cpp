/*************************************************************************/
/*  fluctuate_3d.cpp                                                     */
/*************************************************************************/
/* Copyright (c) 2007-2022 Joshua Brodie.                                */
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

#include "fluctuate_3d.h"
#include "modules/game_frame/math_funcs.h"

real_t Fluctuate3D::_compute_axis_length(const Vector3 &p_axis, const Vector3 &p_vec) const {

    if(p_vec.length_squared() == 0) {
        return 70710678118;
    }

    // compute smallest angle between the vectors
    real_t ang = p_vec.angle_to(p_vec);
    if (ang < Math_PI * 0.5) {
        ang = Math_PI - ang;
    }

    /**********************************

    Solve for c with the Law of Sines

    a/Sin(A) = b/Sin(B) = c/Sin(C)

    A__c__B
     |   /
    b|  /a
     | /
     |/
     C

    /**********************************/

    real_t A = ang;
    real_t a = 1.0;
    real_t b = p_vec.length();
    real_t B = Math::asin((b * Math::sin(A)) / a);
    real_t C = A - B;
    real_t c = (a * Math::sin(C)) / Math::sin(A);

    return MIN(c, RATIO_SQUARE_DIAGONAL_TO_SIDE);
}

void Fluctuate3D::_notification(int p_notification) {
    switch(p_notification) {
        case NOTIFICATION_ENTER_TREE: {
            if(translation_enabled) {
                _translation_setup();
            }
            break;
        }
        case NOTIFICATION_EXIT_TREE: {
            _translation_clear();
            break;
        }
        case NOTIFICATION_ENABLED: {
            if(translation_enabled) {
                _translation_setup();
            }
            break;
        }
        case NOTIFICATION_DISABLED: {
            _translation_clear();
            break;
        }
        case NOTIFICATION_READY: {
            set_process(true);
            set_offset_transform(get_transform());
            _translation_setup();
            break;
        }
        case NOTIFICATION_PROCESS: {
            if(translation_enabled) {
                _translation_process(get_process_delta_time());
            }
            break;
        }
        default:
            break;
    };

    Node3D::_notification(p_notification);
}

void Fluctuate3D::_bind_methods() {
    // Fluctuation
    ClassDB::bind_method(D_METHOD("set_fluctuation_iterations", "iterations"), &Fluctuate3D::set_fluctuate_iterations);
	ClassDB::bind_method(D_METHOD("get_fluctuation_iterations"), &Fluctuate3D::get_fluctuate_iterations);
    ClassDB::bind_method(D_METHOD("set_fluctuation_frequency", "frequency"), &Fluctuate3D::set_fluctuate_frequency);
	ClassDB::bind_method(D_METHOD("get_fluctuation_frequency"), &Fluctuate3D::get_fluctuate_frequency);

    // Translation
    ClassDB::bind_method(D_METHOD("set_translation_enabled", "enabled"), &Fluctuate3D::set_translation_enabled);
	ClassDB::bind_method(D_METHOD("get_translation_enabled"), &Fluctuate3D::get_translation_enabled);
    ClassDB::bind_method(D_METHOD("set_translation_magnitude", "magnitude"), &Fluctuate3D::set_translation_magnitude);
	ClassDB::bind_method(D_METHOD("get_translation_magnitude"), &Fluctuate3D::get_translation_magnitude);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "fluctuation_iterations", PROPERTY_HINT_RANGE, "0,10,1"), "set_fluctuation_iterations", "get_fluctuation_iterations");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fluctuation_frequency", PROPERTY_HINT_NONE, "suffix:Hz"), "set_fluctuation_frequency", "get_fluctuation_frequency");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "translation_enabled"), "set_translation_enabled", "get_translation_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "translation_magnitude", PROPERTY_HINT_NONE, ""), "set_translation_magnitude", "get_translation_magnitude");
}

void Fluctuate3D::Fluctuation::reset(double p_time, const Vector3 &p_axis) {
    time = p_time;
    time_offset = Math::randf();
    axis = p_axis.normalized();
}

Vector3 Fluctuate3D::_compute_axis_vector(const Fluctuation &p_flux) const {

    real_t len = RATIO_SQUARE_DIAGONAL_TO_SIDE;
    Vector3 axis = GFMath::random_unit_vector();
    Vector3 axis_total(0.0, 0.0, 0.0);

    for (Vector<Fluctuation>::ConstIterator itr = _translation_array.begin(); itr != _translation_array.end(); ++itr) {
       const Fluctuation &flux = *itr;

        if (&flux == &p_flux) {
            continue;
        }

        axis_total += flux.axis;
    }

    len = MIN(len, _compute_axis_length(axis, axis_total));

    return axis.normalized() * len;
}

void Fluctuate3D::_translation_setup() {
#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
#endif

    if(_fluctuate_axis_count <= 0) {
        _translation_array.clear();
        return;
    }

    _translation_array.resize(_fluctuate_axis_count + 1);

    _fluctuate_axis_lifetime = (1.0 / (double)fluctuate_frequency) * Math::pow(fluctuate_iteration_ratio, (real_t)MAX(fluctuate_iterations, 0.0)) ;

    real_t life_offset = _fluctuate_axis_lifetime / _translation_array.size();
	int n = 0;
	for (Vector<Fluctuation>::Iterator itr = _translation_array.begin(); itr != _translation_array.end(); ++itr) {
        Fluctuation &flux = *itr;
        flux.reset(life_offset * n++, _compute_axis_vector(flux));
    }
}

void Fluctuate3D::_translation_process(double p_delta) {
#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
#endif

    Vector3 mag_vec = Vector3(0.0, 0.0, 0.0);
    real_t mag_weight = 0.0;

	for (Vector<Fluctuation>::Iterator E = _translation_array.begin(); E != _translation_array.end(); ++E) {
        Fluctuation &flux = *E;

        flux.time += p_delta;

        real_t mag = 0.0;

        for(int j = 0;j < fluctuate_iterations; ++j) {
            real_t ang = (flux.time_offset + flux.time) * fluctuate_frequency / Math::pow(fluctuate_iteration_ratio,(real_t)j) * Math_TAU;
            mag += Math::sin(ang);
        }
        if(fluctuate_iterations > 0) {
            mag /= fluctuate_iterations;
        }

        real_t weight = 0.0;
        real_t life = flux.time / _fluctuate_axis_lifetime;
        if(life >= 1.0) {
            flux.reset(0.0, _compute_axis_vector(flux));
            weight = 0.0;
        } else {
            weight = Math::sin(life * Math_PI);
        }

        mag_vec += flux.axis * mag * weight;
        mag_weight += weight;
    }

    if(mag_weight > 0.0) {
        mag_vec *= translation_magnitude / mag_weight;
    }

	//mag_vec /= (real_t)_fluctuate_axis_count;

    set_position(offset_transform.xform(mag_vec));
}

void Fluctuate3D::_translation_clear() {
    _translation_array.clear();
}

void Fluctuate3D::set_fluctuate_iterations(short p_iterations) { fluctuate_iterations = p_iterations; }
short Fluctuate3D::get_fluctuate_iterations() const { return fluctuate_iterations; }

void Fluctuate3D::set_fluctuate_iteration_ratio(real_t p_ratio) { fluctuate_iteration_ratio = p_ratio; }
real_t Fluctuate3D::get_fluctuate_iteration_ratio() const { return fluctuate_iteration_ratio; }

void Fluctuate3D::set_fluctuate_frequency(real_t p_frequency) { fluctuate_frequency = p_frequency; }
real_t Fluctuate3D::get_fluctuate_frequency() const { return fluctuate_frequency; }

void Fluctuate3D::set_translation_enabled(bool p_enabled) { translation_enabled = p_enabled; }
bool Fluctuate3D::get_translation_enabled() const { return translation_enabled; }

void Fluctuate3D::set_translation_magnitude(real_t p_magnitude) { translation_magnitude = p_magnitude; }
real_t Fluctuate3D::get_translation_magnitude() const { return translation_magnitude; }

void Fluctuate3D::set_offset_transform(Transform3D &p_offset) { offset_transform = p_offset; }
Transform3D Fluctuate3D::get_offset_transform() const { return offset_transform; }

PackedStringArray Fluctuate3D::get_configuration_warnings() const {
	PackedStringArray warnings = Node3D::get_configuration_warnings();

	return warnings;
}

Fluctuate3D::Fluctuate3D() : Node3D() {}

Fluctuate3D::~Fluctuate3D() {}
