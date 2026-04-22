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

    UtilityFunctions::print("Main::_ready() just got called.");

    auto new_mnchar = reinterpret_cast<Mnchar *>(
        get_mnchar_scene()->instantiate());
    
    new_mnchar -> start(Vector3(15, 1, -20));

    add_child(new_mnchar);

}
