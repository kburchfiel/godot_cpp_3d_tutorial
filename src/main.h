// See README for documentation and references

#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include "mnchar.h"

using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

private:
    Ref<PackedScene> mnchar_scene;

protected:

static void _bind_methods();

public:
Main();
~Main();

Ref<PackedScene> get_mnchar_scene();
void set_mnchar_scene(Ref<PackedScene>);

void _ready();

};
