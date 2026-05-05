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

  int current_hit_value = hits_achieved[firing_mnchar_id_arg];
  current_hit_value += 1;
  hits_achieved[firing_mnchar_id_arg] = current_hit_value;

  int current_overall_hit_value = overall_hits_achieved[firing_mnchar_id_arg];
  current_overall_hit_value += 1;
  overall_hits_achieved[firing_mnchar_id_arg] = current_overall_hit_value;

  active_mnchars.erase(hit_mnchar_id_arg);

  generate_overall_hits_text();

  UtilityFunctions::print("Current size of active_mnchars: ",
                          active_mnchars.size());



  if (active_mnchars.size() == 1) {
    String winning_mnchar = *active_mnchars.begin();

    int current_overall_win_value = overall_wins[winning_mnchar];
    current_overall_win_value += 1;
    overall_wins[winning_mnchar] = current_overall_win_value;

    generate_overall_wins_text();
    end_game(winning_mnchar);
  }

  else if (active_mnchars.size() == 0) {
    end_game("Nobody");
  }
}

void Main::_on_hud_start_game(Array mnchars_to_include) {

  get_node<Hud>("Hud")->set_process_mode(PROCESS_MODE_DISABLED);

  active_mnchars.clear();

  hits_achieved = TypedDictionary<String, int>{};

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

    hits_achieved[mnchar_id_arg] = 0;

    if (overall_hits_achieved.has(mnchar_id_arg) == false)

    {
      overall_hits_achieved[mnchar_id_arg] = 0;
    }

    if (overall_wins.has(mnchar_id_arg) == false) {
      overall_wins[mnchar_id_arg] = 0;
    }
  }

  UtilityFunctions::print("Printing out all active players in set:");
  for (auto active_mnchars_iterator = active_mnchars.begin();
       active_mnchars_iterator != active_mnchars.end();
       ++active_mnchars_iterator) {
    UtilityFunctions::print(*active_mnchars_iterator);
  }

  generate_overall_hits_text();
  generate_overall_wins_text();

}

void Main::end_game(String winning_mnchar_id) {
  get_tree()->call_group("mnchars", "queue_free");

  String new_winner_message = "The winning player \
is: " + winning_mnchar_id;

  if (winning_mnchar_id != "Nobody") {
    new_winner_message +=
        " (" + String(mnchar_id_color_name_dict[winning_mnchar_id]) + ")";
  }

  new_winner_message += "\n\n";

  Array hits_achieved_keys = hits_achieved.keys();

  for (int key_index = 0; key_index < hits_achieved_keys.size(); key_index++)

  {
    String mnchar_id_arg = String(hits_achieved_keys[key_index]);
    String current_id_hits_achieved =
        String::num_int64(hits_achieved[mnchar_id_arg]);

    new_winner_message += "Player " + mnchar_id_arg + " (" +
                          String(mnchar_id_color_name_dict[mnchar_id_arg]) +
                          ")" + " scored " + current_id_hits_achieved +
                          " hits.\n";
  }

  new_winner_message += "\n";

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

  mnchar_id_color_name_dict =
      get_node<Hud>("Hud")->get_mnchar_id_color_name_dict();

  get_node<Timer>("HudProcessTimer")
      ->connect("timeout", Callable(this, "_on_hud_process_timer_timeout"));

  get_node<Hud>("Hud")->connect("start_game",
                                Callable(this, "_on_hud_start_game"));

  get_node<Hud>("Hud")->set_winner_text("Welcome to Cube Combat!\n\n");

  get_node<Hud>("Hud")->set_instructions_text(
      get_node<Hud>("Hud")->get_instructions());

  get_node<Hud>("Hud")->update_between_game_message();
}

void Main::generate_overall_hits_text() {
  String overall_hits_text = "Overall hits:\n";

  Array overall_hits_achieved_keys = overall_hits_achieved.keys();

  for (int key_index = 0; key_index < overall_hits_achieved_keys.size();
       key_index++) {
    overall_hits_text +=
        "Player " + String(overall_hits_achieved_keys[key_index]) + " (" +
        String(mnchar_id_color_name_dict[String(
            overall_hits_achieved_keys[key_index])]) +
        "): " +
        String::num_int64(
            overall_hits_achieved[overall_hits_achieved_keys[key_index]]) +
        "\n";
  }

  get_node<Hud>("Hud")->set_overall_hits_text(overall_hits_text);
  get_node<Hud>("Hud")->update_constant_message();
}

void Main::generate_overall_wins_text() {
  Array overall_wins_keys = overall_wins.keys();

  String overall_wins_text = "\nOverall wins:\n";

  for (int key_index = 0; key_index < overall_wins_keys.size(); key_index++)

  {
    overall_wins_text +=
        "Player " + String(overall_wins_keys[key_index]) + " (" +
        String(mnchar_id_color_name_dict[String(
            overall_wins_keys[key_index])]) +
        "): " + String::num_int64(overall_wins[overall_wins_keys[key_index]]) +
        "\n";
  }

  get_node<Hud>("Hud")->set_overall_wins_text(overall_wins_text);
  get_node<Hud>("Hud")->update_constant_message();
}
