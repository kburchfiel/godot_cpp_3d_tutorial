// See README for documentation and references

#include "main.h"

void Main::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_mnchar_scene"), &Main::get_mnchar_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_scene", "get_mnchar_scene");

  ClassDB::bind_method(D_METHOD("_on_mnchar_mnchar_hit", "hit_mnchar_id_arg"),
                       &Main::_on_mnchar_mnchar_hit);

  ClassDB::bind_method(D_METHOD("_on_hud_start_game"),
                       &Main::_on_hud_start_game);

  ClassDB::bind_method(D_METHOD("_on_hud_process_timer_timeout"),
                       &Main::_on_hud_process_timer_timeout);
}

Main::Main() {}

Main::~Main() {}

Ref<PackedScene> Main::get_mnchar_scene() { return mnchar_scene; }

void Main::set_mnchar_scene(Ref<PackedScene> packed_scene) {
  mnchar_scene = packed_scene;
}

void Main::_on_mnchar_mnchar_hit(String hit_mnchar_id_arg,
                                 String firing_mnchar_id_arg) {
  UtilityFunctions::print("The Mnchar with an ID of ", hit_mnchar_id_arg,
                          " was just hit by the Mnchar with an ID of ",
                          firing_mnchar_id_arg, ".");
  active_mnchars.erase(hit_mnchar_id_arg);
  // See godot-cpp/include/godot_cpp/templates/hash_set.hpp

  UtilityFunctions::print("Current size of active_mnchars: ",
                          active_mnchars.size());

  if (active_mnchars.size() == 1) {
    String winning_mnchar = *active_mnchars.begin();
    end_game(winning_mnchar);
  }

  else if (active_mnchars.size() == 0) {
    end_game("Nobody");
  }
}

void Main::_on_hud_start_game(Array mnchars_to_include) {

  get_node<Hud>("Hud")->set_process_mode(PROCESS_MODE_DISABLED);

  active_mnchars.clear();

  for (int index = 0; index < mnchars_to_include.size(); index++) {

    String mnchar_id_arg = mnchars_to_include[index];
    Color mnchar_color_arg = mnchar_id_color_dict[mnchar_id_arg];
    Vector3 mnchar_translate_arg = mnchar_id_location_dict[mnchar_id_arg];
    double mnchar_rotation_arg = mnchar_id_rotation_dict[mnchar_id_arg];

    auto new_mnchar =
        reinterpret_cast<Mnchar *>(get_mnchar_scene()->instantiate());

    new_mnchar->connect("mnchar_hit", Callable(this, "_on_mnchar_mnchar_hit"));

    new_mnchar->start(mnchar_id_arg, mnchar_translate_arg, mnchar_rotation_arg,
                      mnchar_color_arg);

    add_child(new_mnchar);

    active_mnchars.insert(mnchar_id_arg);
  }

  UtilityFunctions::print("Printing out all active players in set:");
  for (auto active_mnchars_iterator = active_mnchars.begin();
       active_mnchars_iterator != active_mnchars.end();
       ++active_mnchars_iterator) {
    UtilityFunctions::print(*active_mnchars_iterator);
  }
}

void Main::end_game(String winning_mnchar_id) {
  get_tree()->call_group("mnchars", "queue_free");

  String new_winner_message = "The winning player \
is: " + winning_mnchar_id;

  if (winning_mnchar_id != "Nobody") // Will be true if (1) a game
  // was reset or (2) the final two players were hit at the exact
  // same time
  {
    new_winner_message +=
        " (" +
        String(get_node<Hud>("Hud")
                   ->get_mnchar_id_color_name_dict()[
      winning_mnchar_id]) + ")";
  }

  new_winner_message += "\n\n";

  get_node<Hud>("Hud")->set_winner_text(new_winner_message);
  get_node<Hud>("Hud")->update_between_game_message();

  get_node<Timer>("HudProcessTimer")->start();
}

void Main::_on_hud_process_timer_timeout() {
  get_node<Hud>("Hud")->clear_mnchars_to_include();
  get_node<Hud>("Hud")->set_instructions_text(
      get_node<Hud>("Hud")->get_instructions());
  get_node<Hud>("Hud")->update_between_game_message();
  get_node<Hud>("Hud")->set_can_launch_new_game(true);
  get_node<Hud>("Hud")->set_process_mode(PROCESS_MODE_ALWAYS);
}

void Main::_ready() {

  get_node<Timer>("HudProcessTimer")
      ->connect("timeout", Callable(this, "_on_hud_process_timer_timeout"));

  get_node<Hud>("Hud")->connect("start_game",
                                Callable(this, "_on_hud_start_game"));

  get_node<Hud>("Hud")->set_winner_text("Welcome to Cube Combat!\n\n");

  get_node<Hud>("Hud")->set_instructions_text(
      get_node<Hud>("Hud")->get_instructions());

  get_node<Hud>("Hud")->update_between_game_message();
}
