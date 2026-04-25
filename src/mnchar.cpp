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

  ClassDB::bind_method(D_METHOD("get_projectile_scene"),
                       &Mnchar::get_projectile_scene);
  ClassDB::bind_method(D_METHOD("set_projectile_scene", "projectile_scene"),
                       &Mnchar::set_projectile_scene);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_projectile_scene", "get_projectile_scene");

  ADD_SIGNAL(MethodInfo("mnchar_hit",
                        PropertyInfo(Variant::STRING, "mnchar_id"),
                        PropertyInfo(Variant::STRING, "firing_mnchar_id")));

  ClassDB::bind_method(D_METHOD("_on_projectile_detector_body_entered", "node"),
                       &Mnchar::_on_projectile_detector_body_entered);
}

Mnchar::Mnchar() {
}

Mnchar::~Mnchar() {}

Ref<PackedScene> Mnchar::get_projectile_scene() { return projectile_scene; }

void Mnchar::set_projectile_scene(Ref<PackedScene> packed_scene) {
  projectile_scene = packed_scene;
}

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

void Mnchar::set_mnchar_color(const Color mnchar_color_arg) {
  Ref<BaseMaterial3D> mncharbody_mesh_material_3d =
      (get_node<Node3D>("Pivot")
           ->get_node<MeshInstance3D>("Body")
           ->get_mesh()
           ->surface_get_material(0));

  mncharbody_mesh_material_3d->set_albedo(mnchar_color_arg);

  get_node<Node3D>("Pivot")
      ->get_node<MeshInstance3D>("Body")
      ->get_mesh()
      ->surface_set_material(0, Ref<Material>(mncharbody_mesh_material_3d));
}

void Mnchar::start(String mnchar_id_arg, Vector3 mnchar_translate_arg,
                   double mnchar_rotation_arg, Color mnchar_color_arg) {
  set_mnchar_id(mnchar_id_arg);
  UtilityFunctions::print("Mnchar's ID is ", get_mnchar_id(), ".");
  translate(mnchar_translate_arg);
  set_mnchar_color(mnchar_color_arg);
  get_node<Node3D>("Pivot")->rotate_object_local(Vector3(0, 1, 0),
                                                 mnchar_rotation_arg);
get_node<Area3D>("Projectile_Detector")->connect(
"body_entered", Callable(this, "_on_projectile_detector_body_entered"));                                                 
}

void Mnchar::shoot_projectile()

{
  auto projectile =
      reinterpret_cast<Projectile *>(projectile_scene->instantiate());

  Transform3D projectile_transform =
      get_node<Node3D>("Pivot")->get_global_transform();

  projectile_transform =
      projectile_transform.translated_local(Vector3(0, 0, 3));
  projectile->start(projectile_transform, mnchar_id);
  get_parent()->add_child(projectile);
}

void Mnchar::_on_projectile_detector_body_entered(Node3D *node) {
  UtilityFunctions::print(
      "on_body_entered() just got called within mnchar.cpp.");
  Projectile *node_as_projectile = Object::cast_to<Projectile>(node);
  String firing_mnchar_id = node_as_projectile->get_firing_mnchar_id();
  emit_signal("mnchar_hit", mnchar_id, firing_mnchar_id);
  queue_free();
}

void Mnchar::_physics_process(double delta) {

  auto input = Input::get_singleton();

  if ((input->is_action_just_pressed("fire_" + mnchar_id)) &&
      (input->is_action_pressed("reset_" + mnchar_id) == false)) {

    shoot_projectile();
  }

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