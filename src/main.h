#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

protected:

static void _bind_methods();

public:
Main();
~Main();

void _ready();

};