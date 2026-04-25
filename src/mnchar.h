// See README for documentation and references

#pragma once

#include "projectile.h"
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class Mnchar : public CharacterBody3D {
  GDCLASS(Mnchar, CharacterBody3D)

private:
  double movement_speed = 14;
  double rotation_speed = 0.15;
  String mnchar_id = "";
  Ref<PackedScene> projectile_scene;

protected:
  static void _bind_methods();

public:
  Mnchar();
  ~Mnchar();

  void set_movement_speed(const double movement_speed);
  double get_movement_speed() const;

  void set_rotation_speed(const double rotation_speed);
  double get_rotation_speed() const;

  void set_mnchar_id(const String mnchar_id);
  String get_mnchar_id() const;

  void start(String mnchar_id_arg, Vector3 mnchar_translate_arg,
             double mnchar_rotation_arg, Color mnchar_color_arg);

  Ref<PackedScene> get_projectile_scene();
  void set_projectile_scene(Ref<PackedScene>);

  void set_mnchar_color(const Color mnchar_color);

  void shoot_projectile();

  void _physics_process(double delta) override;
};