#include "hud.h"

using namespace godot;

Hud::Hud() { reset = false; }
Hud::~Hud() {}

void Hud::_bind_methods() {
  ADD_SIGNAL(MethodInfo("start_game",
                        PropertyInfo(Variant::ARRAY, "mnchars_to_include")));
}

TypedDictionary<String, String> Hud::get_mnchar_id_color_name_dict() {
  return mnchar_id_color_name_dict;
}

String Hud::get_instructions()

{
  return instructions;
}

void Hud::set_winner_text(const String winner_arg) { winner_text = winner_arg; }

void Hud::set_instructions_text(const String instructions_arg) {
  instructions_text = instructions_arg;
}

void Hud::set_entrants_text(const String entrants_arg) {
  entrants_text = entrants_arg;
}

// void Hud::set_overall_hits_text(String overall_hits_arg) {
//   overall_wins_text = overall_hits_arg;
// }

// void Hud::set_overall_wins_text(String overall_wins_arg) {
//   overall_wins_text = overall_wins_arg;
// }

void Hud::update_between_game_message() {
  String between_game_message = winner_text + instructions_text + entrants_text;
  // UtilityFunctions::print("between_game_message is: ", between_game_message);
  auto between_game_label = get_node<Label>("BetweenGameLabel");
  between_game_label->set_text(between_game_message);
}

void Hud::clear_mnchars_to_include() { mnchars_to_include.clear(); }

// void Hud::update_within_game_message() {
//   String within_game_message = overall_hits_text + overall_wins_text;
// }

void Hud::set_can_launch_new_game(bool can_launch_new_game_arg) {
  can_launch_new_game = can_launch_new_game_arg;
}

void Hud::_process(double delta) {

  auto input = Input::get_singleton();

  for (int i = 0; i < 8; i++)

  {

    String strint = String::num_int64(i);
    if ((input->is_action_just_pressed("fire_" + strint)) &&
        (mnchars_to_include.has(strint) == false))

    {
      UtilityFunctions::print("Adding player " + strint + " to the game.");
      mnchars_to_include.append(strint);

      entrants_text += "Added Player " + strint + " (" +
                       String(mnchar_id_color_name_dict[strint]) +
                       ") to the game.\n";
      update_between_game_message();
    }

    if ((input->is_action_pressed("fire_" + strint)) &&
        (input->is_action_pressed("reset_" + strint)) &&
        (can_launch_new_game == true)) {
      UtilityFunctions::print("New game requested. Players:",
                              mnchars_to_include);

      instructions_text = "";
      winner_text = "";
      entrants_text = "";
      update_between_game_message();
      can_launch_new_game = false;

      emit_signal("start_game", mnchars_to_include);
    }
  }
}