#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class Mnchar : public CharacterBody3D {
  GDCLASS(Mnchar, CharacterBody3D)

private:
  double movement_speed = 14;
  
  protected:
  static void _bind_methods();

public:
  Mnchar();
  ~Mnchar();

  void set_movement_speed(const double movement_speed);
  double get_movement_speed() const;

void start(Vector3 mnchar_translate_arg);
};