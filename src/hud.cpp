#include "hud.h"

using namespace godot;

Hud::Hud() { reset = false; }
Hud::~Hud() {}

void Hud::_bind_methods() {}

String Hud::get_instructions()

{
  return instructions;
}

void Hud::set_winner_text(String winner_arg) { winner_text = winner_arg; }

void Hud::set_instructions_text(String instructions_arg) {
  instructions_text = instructions_arg;
}

void Hud::set_entrants_text(String entrants_arg) {
  entrants_text = entrants_arg;
}

void Hud::set_overall_hits_text(String overall_hits_arg) {
  overall_wins_text = overall_hits_arg;
}

void Hud::set_overall_wins_text(String overall_wins_arg) {
  overall_wins_text = overall_wins_arg;
}

void Hud::update_between_game_message() {
  String between_game_message =
      winner_text + instructions_text + entrants_text;
}

void Hud::update_within_game_message() {
  String within_game_message = overall_hits_text + overall_wins_text;
}

void Hud::_process(double delta) {}