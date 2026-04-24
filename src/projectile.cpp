// See README for documentation and references

#include "projectile.h"

using namespace godot;

void Projectile::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_projectile_speed"),
                       &Projectile::get_projectile_speed);
  ClassDB::bind_method(D_METHOD("set_projectile_speed", "p_projectile_speed"),
                       &Projectile::set_projectile_speed);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_speed"),
               "set_projectile_speed", "get_projectile_speed");
}

Projectile::Projectile() {}

Projectile::~Projectile() {}

void Projectile::set_projectile_speed(const double p_projectile_speed) {
  projectile_speed = p_projectile_speed;
}

double Projectile::get_projectile_speed() const { return projectile_speed; }

void Projectile::set_firing_mnchar_id(const String firing_mnchar_id_arg) {
  firing_mnchar_id = firing_mnchar_id_arg;
}

String Projectile::get_firing_mnchar_id() const { return firing_mnchar_id; }

void Projectile::start(Transform3D transform, String firing_mnchar_id) {

  set_firing_mnchar_id(firing_mnchar_id);

  set_transform(transform);

  auto projectile_basis_z = Projectile::get_transform().get_basis()[2];

  projectile_basis_z.z *= -1;

  projectile_velocity = -1 * projectile_basis_z * projectile_speed;
}

void Projectile::_physics_process(double delta) {

  auto collision = move_and_collide(projectile_velocity * delta);

  if (active_time >= 2) {
    queue_free();
  }

  active_time += delta;
}
