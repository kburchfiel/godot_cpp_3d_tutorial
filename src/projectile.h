// See README for documentation and references

#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class Projectile : public CharacterBody3D {
  GDCLASS(Projectile, CharacterBody3D)

private:
  double projectile_speed = 64;
  double active_time = 0;
  String firing_mnchar_id = "";
  Vector3 projectile_velocity = Vector3(0, 0, 0);

protected:
  static void _bind_methods();

public:
  Projectile();
  ~Projectile();

  void set_projectile_speed(const double movement_speed);
  double get_projectile_speed() const;

  void set_firing_mnchar_id(const String firing_mnchar_id_arg);
  String get_firing_mnchar_id() const;

  void start(Transform3D transform, String firing_mnchar_id);
  void _physics_process(double delta) override;
};
