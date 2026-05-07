// See README for documentation and references

#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Hud : public CanvasLayer {
  GDCLASS(Hud, CanvasLayer)

private:
  bool reset;
  Size2 screenSize;
  static void _bind_methods();

  const String instructions = "To join this game, press Fire on \
your controller. To launch a game, press both Fire and Reset \
simultaneously. To reset overall stats, hold down Reset for \
two seconds.\n\n";

  String winner_text{""};
  String instructions_text{""};
  String entrants_text{""};

  String overall_hits_text{""};
  String overall_wins_text{""};

  Array mnchars_to_include{};

  bool can_launch_new_game = true;

  const TypedDictionary<String, String> mnchar_id_color_name_dict{
      {String("0"), "Blue"},  {String("1"), "Green"},   {String("2"), "Cyan"},
      {String("3"), "Red"},   {String("4"), "Magenta"}, {String("5"), "Yellow"},
      {String("6"), "White"}, {String("7"), "Black"}};

  TypedDictionary<String, double> id_reset_time_dict{
      {"0", 0.0}, {"1", 0.0}, {"2", 0.0}, {"3", 0.0},
      {"4", 0.0}, {"5", 0.0}, {"6", 0.0}, {"7", 0.0}};

public:
  Hud();
  ~Hud();

  TypedDictionary<String, String> get_mnchar_id_color_name_dict() const;

  String get_instructions() const;

  void set_winner_text(const String winner_arg);
  void set_instructions_text(const String instructions_arg);
  void set_entrants_text(const String entrants_arg);

  void set_overall_hits_text(const String overall_hits_arg);
  void set_overall_wins_text(const String overall_wins_arg);

  void clear_mnchars_to_include();

  void update_between_game_message();
  void update_constant_message();

  void set_can_launch_new_game(bool can_launch_new_game_arg);

  void _process(double delta) override;
};