// See README for documentation and references

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

  ClassDB::bind_method(D_METHOD("get_rotation_speed"),
                       &Mnchar::get_rotation_speed);
  ClassDB::bind_method(D_METHOD("set_rotation_speed", "p_rotation_speed"),
                       &Mnchar::set_rotation_speed);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed"),
               "set_rotation_speed", "get_rotation_speed");
}

Mnchar::Mnchar() {}

Mnchar::~Mnchar() {}

void Mnchar::set_movement_speed(const double p_movement_speed) {
  movement_speed = p_movement_speed;
}

double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::set_rotation_speed(const double p_rotation_speed) {
  rotation_speed = p_rotation_speed;
}

double Mnchar::get_rotation_speed() const { return rotation_speed; }

void Mnchar::set_mnchar_id(const String p_mnchar_id) {
  mnchar_id = p_mnchar_id;
}

String Mnchar::get_mnchar_id() const { return mnchar_id; }

void Mnchar::start(String mnchar_id_arg, Vector3 mnchar_translate_arg)

{
  set_mnchar_id(mnchar_id_arg);
  UtilityFunctions::print("Mnchar's ID is ", get_mnchar_id(), ".");
  translate(mnchar_translate_arg);
}

void Mnchar::_physics_process(double delta) {

  auto input = Input::get_singleton();

  float x_direction =
      input->get_axis("move_right_" + mnchar_id, "move_left_" + mnchar_id);
  float z_direction =
      input->get_axis("move_back_" + mnchar_id, "move_forward_" + mnchar_id);

  get_node<Node3D>("Pivot")->rotate_object_local(
      Vector3(0, 1, 0),
      rotation_speed * input->get_axis("rotate_right_" + mnchar_id,
                                       "rotate_left_" + mnchar_id));

  auto player_transform_basis_z =
      get_node<Node3D>("Pivot")->get_transform().get_basis()[2];

  auto player_transform_basis_x =
      get_node<Node3D>("Pivot")->get_transform().get_basis()[0];

  player_transform_basis_x.x *= -1;

  player_transform_basis_z.z *= -1;

  Vector3 target_velocity = Vector3(0, 0, 0);

  float abs_z_direction = std::abs(z_direction);
  float abs_x_direction = std::abs(x_direction);

  if (abs_z_direction >= abs_x_direction) {
    target_velocity +=
        -1 * player_transform_basis_z * z_direction * movement_speed;
  } else {
    target_velocity +=
        -1 * player_transform_basis_x * x_direction * movement_speed;
  }

  set_velocity(target_velocity);

  move_and_slide();
}