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

  if (active_mnchars.size() == 1)
  {
    String winning_mnchar = *active_mnchars.begin();
    end_game(winning_mnchar);
  }

  else if (active_mnchars.size() == 0) {
    end_game("Nobody");
  }
}

void Main::end_game(String winning_mnchar_id) {
  get_tree()->call_group("mnchars", "queue_free");

  String new_winner_message = "The winning player \
is: " + winning_mnchar_id;

  UtilityFunctions::print(new_winner_message);
}

void Main::_ready() {

  active_mnchars.clear();

  Array mnchars_to_include{"0", "1"};

  for (int index = 0; index < mnchars_to_include.size(); index++) {

    String mnchar_id_arg = mnchars_to_include[index];
    // UtilityFunctions::print("Main::_ready() Checkpoint 3.");
    Color mnchar_color_arg = mnchar_id_color_dict[mnchar_id_arg];
    // UtilityFunctions::print("Main::_ready() Checkpoint 4.");
    Vector3 mnchar_translate_arg = mnchar_id_location_dict[mnchar_id_arg];
    // UtilityFunctions::print("Main::_ready() Checkpoint 5.");
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
