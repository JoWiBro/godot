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
}

void Fluctuate3D::Fluctuation::reset(double p_time, const Vector3 &p_axis) {
    time = p_time;
    time_offset = Math::randf();
    axis = p_axis.normalized();
}

Vector3 Fluctuate3D::_compute_random_axis_vector() const {

    real_t phi = Math::random(0.0, Math_TAU);
    real_t cos_theta = Math::random(-1.0f, 1.0f);

    real_t theta = Math::acos(cos_theta);

    return Vector3(
        Math::sin(theta) * Math::cos(phi),
        Math::sin(theta) * Math::sin(phi),
        Math::cos(theta)
    );
}

void Fluctuate3D::_translation_setup() {
    if(fluctuate_axis_count <= 0) {
        translation_array.clear();
        return;
    }

    translation_array.resize(fluctuate_axis_count + 1);

    real_t life_offset = fluctuate_axis_lifetime / translation_array.size();
	int n = 0;
	for (Vector<Fluctuation>::Iterator E = translation_array.begin(); E != translation_array.end(); ++E) {
        Fluctuation &flux = *E;
        flux.reset(life_offset * n++, _compute_random_axis_vector());
    }
}

void Fluctuate3D::_translation_process(double p_delta) {
    Vector3 mag_vec = Vector3(0.0, 0.0, 0.0);
    real_t mag_weight = 0.0;

	for (Vector<Fluctuation>::Iterator E = translation_array.begin(); E != translation_array.end(); ++E) {
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
        real_t life = flux.time / fluctuate_axis_lifetime;
        if(life >= 1.0) {
            flux.reset(0.0, _compute_random_axis_vector());
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

	mag_vec /= (real_t)fluctuate_axis_count;

    set_position(offset.xform(mag_vec));
}

void Fluctuate3D::_translation_clear() {
    translation_array.clear();
}

void Fluctuate3D::set_fluctuate_axis_count(short p_count) { fluctuate_axis_count = p_count; }
short Fluctuate3D::get_fluctuate_axis_count() const { return fluctuate_axis_count; }

void Fluctuate3D::set_fluctuate_axis_lifetime(double p_lifetime) { fluctuate_axis_lifetime = p_lifetime; }
double Fluctuate3D::get_fluctuate_axis_lifetime() const { return fluctuate_axis_lifetime; }

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

PackedStringArray Fluctuate3D::get_configuration_warnings() const {
	PackedStringArray warnings = Node3D::get_configuration_warnings();

	return warnings;
}

Fluctuate3D::Fluctuate3D() : Node3D() {
    offset = get_basis();
}

Fluctuate3D::~Fluctuate3D() {}
