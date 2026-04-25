// See README for documentation and references

#include "main.h"

void Main::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_mnchar_scene"), &Main::get_mnchar_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_scene", "get_mnchar_scene");
}

Main::Main() {}

Main::~Main() {}

Ref<PackedScene> Main::get_mnchar_scene() { return mnchar_scene; }

void Main::set_mnchar_scene(Ref<PackedScene> packed_scene) {
mnchar_scene = packed_scene;
}

void Main::_ready() {


    Array players_to_include {"0", "1"};

    for (int index = 0; index < players_to_include.size(); index++)
    {

    String mnchar_id_arg = players_to_include[index];
    // UtilityFunctions::print("Main::_ready() Checkpoint 3.");
    Color mnchar_color_arg = mnchar_id_color_dict[mnchar_id_arg];
    // UtilityFunctions::print("Main::_ready() Checkpoint 4.");
    Vector3 mnchar_translate_arg = mnchar_id_location_dict[mnchar_id_arg];
    // UtilityFunctions::print("Main::_ready() Checkpoint 5.");
    double mnchar_rotation_arg = mnchar_id_rotation_dict[mnchar_id_arg];

    auto new_mnchar = reinterpret_cast<Mnchar *>(
    get_mnchar_scene()->instantiate());

    new_mnchar -> start(mnchar_id_arg, mnchar_translate_arg,
    mnchar_rotation_arg, mnchar_color_arg);

    add_child(new_mnchar);
    }

}
