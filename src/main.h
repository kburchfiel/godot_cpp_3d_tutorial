// See README for documentation and references

#pragma once
#include "mnchar.h"
#include "hud.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/core/math_defs.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/timer.hpp>

using namespace godot;

class Main : public Node {
  GDCLASS(Main, Node)

private:
  Ref<PackedScene> mnchar_scene;

  TypedDictionary<String, Vector3> mnchar_id_location_dict{
      {String("0"), Vector3(15, 0, -20)},  {String("1"), Vector3(-15, 0, 20)},
      {String("2"), Vector3(-20, 0, -15)}, {String("3"), Vector3(20, 0, 15)},
      {String("4"), Vector3(-5, 0, -20)},  {String("5"), Vector3(5, 0, 20)},
      {String("6"), Vector3(-20, 0, 5)},   {String("7"), Vector3(20, 0, -5)}};

  TypedDictionary<String, double> mnchar_id_rotation_dict{
      {String("0"), 0},           {String("1"), Math_PI},
      {String("2"), Math_PI / 2}, {String("3"), Math_PI / -2},
      {String("4"), 0},           {String("5"), Math_PI},
      {String("6"), Math_PI / 2}, {String("7"), Math_PI / -2}};

  TypedDictionary<String, Color> mnchar_id_color_dict{
      {String("0"), Color(0, 0, 1, 1)}, {String("1"), Color(0, 1, 0, 1)},
      {String("2"), Color(0, 1, 1, 1)}, {String("3"), Color(1, 0, 0, 1)},
      {String("4"), Color(1, 0, 1, 1)}, {String("5"), Color(1, 1, 0, 1)},
      {String("6"), Color(1, 1, 1, 1)}, {String("7"), Color(0, 0, 0, 1)}};

  HashSet<String> active_mnchars{};

protected:
  static void _bind_methods();

public:
  Main();
  ~Main();

  Ref<PackedScene> get_mnchar_scene();
  void set_mnchar_scene(Ref<PackedScene>);

  void _on_mnchar_mnchar_hit(String hit_mnchar_id_arg,
                             String firing_mnchar_id_arg);

  void _on_hud_start_game(Array mnchars_to_include);

  void end_game(String winning_mnchar_id);

void _on_hud_process_timer_timeout();

  void _ready();
};
