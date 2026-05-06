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

void Projectile::set_projectile_color(const Color projectile_color_arg) {
  Ref<BaseMaterial3D> projectilebody_mesh_material_3d =
      (get_node<Node3D>("Pivot")
           ->get_node<MeshInstance3D>("Body")
           ->get_mesh()
           ->surface_get_material(0));

  projectilebody_mesh_material_3d->set_albedo(projectile_color_arg);

  get_node<Node3D>("Pivot")
      ->get_node<MeshInstance3D>("Body")
      ->get_mesh()
      ->surface_set_material(0, Ref<Material>(projectilebody_mesh_material_3d));
}


void Projectile::start(const Transform3D transform, 
const String firing_mnchar_id, const Color projectile_color_arg) {

  set_firing_mnchar_id(firing_mnchar_id);

  set_transform(transform);

  set_projectile_color(projectile_color_arg);

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


