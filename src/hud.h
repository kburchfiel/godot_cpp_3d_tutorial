#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Hud : public CanvasLayer {
  GDCLASS(Hud, CanvasLayer)

private:
  bool reset;
  Size2 screenSize;

  static void _bind_methods();

  String instructions = "To join this game, press Fire on \
    your controller.\nTo launch a game, press both Fire and\nReset \
    simultaneously.\nTo reset overall stats, hold down Reset\nfor \
    two seconds.\n\n";

  String winner_text{""};
  String instructions_text{""};
  String entrants_text{""};

  String overall_hits_text{""};
  String overall_wins_text{""};

public:
  Dictionary players_to_include;

  Hud();
  ~Hud();

  String get_instructions();

  void set_winner_text(String winner_arg);

  void set_instructions_text(String instructions_arg);

  void set_entrants_text(String entrants_arg);

  void set_overall_hits_text(String overall_hits_arg);

  void set_overall_wins_text(String overall_wins_arg);

  void update_between_game_message();
  void update_within_game_message();

  void _process(double delta) override;
};