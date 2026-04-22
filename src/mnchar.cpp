#include "mnchar.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Mnchar::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_movement_speed"),
                       &Mnchar::get_movement_speed);
  ClassDB::bind_method(D_METHOD("set_movement_speed", "p_movement_speed"),
                       &Mnchar::set_movement_speed);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"),
               "set_movement_speed", "get_movement_speed");

}

Mnchar::Mnchar() {}

Mnchar::~Mnchar() {}



void Mnchar::set_movement_speed(const double p_movement_speed) {
  movement_speed = p_movement_speed;
}

double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::start(Vector3 mnchar_translate_arg)

{translate(mnchar_translate_arg);}
