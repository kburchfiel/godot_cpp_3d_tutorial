## Godot CPP 3D Tutorial [Work in progress]

By Ken Burchfiel

Released under the MIT license

This step-by-step guide will demonstrate how to create a 3D multiplayer game in Godot using C++. It is based on my [Cube Combat demo](https://github.com/kburchfiel/godot_cpp_3d_demo), but (unlike that project) guides you more explicitly through the steps involved, both within your code editor and Godot, to put this kind of game together.

*Note: This resource is being created without the use of generative-AI tools.*


## Part 1: Getting started

(Note: Many of these steps will resemble those in the excellent 'Getting started' section of the official GDExtension documentation at https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html . Certain code blocks within this section derive from that document as well.)

1. Create a new folder that will store your Godot project and its corresponding code. I'll call mine `godot_cpp_3d_tutorial`, but the name you choose is of course up to you.

1. First, you'll want to download the latest stable version of godot-cpp from https://github.com/godotengine/godot-cpp . (As of 2026-04-17, a stable version of version 10.x hasn't yet been released, so I went ahead and downloaded the beta version with a commit ID of 4862a9d (https://github.com/godotengine/godot-cpp/tree/4862a9dcf1471c9ea19680b9faadb5b6a9432092 .) Whether you download and unzip or simply clone it, make sure that exists within your project folder within a folder named 'godot-cpp'.

1. Next, open up this godot-cpp folder within your terminal and run `scons platform=linux` (replacing `linux` with your own OS if needed). This will compile all of the source code needed to apply this library. (It will *also* generate additional code files that aren't visible in an uncompiled version of the repository, such as the one on GitHub).

1. Go ahead and create a 'src' folder within your root project folder. This folder will store your source C++ code and its compiled variants.

1. Create a 'project' folder within this root folder also. Next, open up Godot, which you can download from https://godotengine.org/ if you haven't already. (I'm using Godot 4.6 for this project, but this tutorial should be applicable for newer releases also for a decent while.) Within the loading screen, hit the Create button at the top left. I chose 'Cpp 3D Tutorial' as my project name and the 'project' folder I just created as my path. Once you've filled in these items, hit Create on the bottom right.

    ![](/tutorial_screenshots/new_game_project.png)


1. Close back out of the editor for now. Before we create a scene, we should first create a GDExtension class within C++ that can be used as the basis for that scene. (This is a different approach than what you might be used to with GDScript.)

1. Before we start writing our own C++ code, let's get a few crucial setup tasks out of the way. First, within your project folder, which will now have a number of Godot-generated items (including a project.godot) file, go ahead and create a new folder called 'bin.' Within this folder, create a new file called gdexample.gdextension, then paste the following material into it:

    ```
    [configuration]

    entry_symbol = "example_library_init"
    compatibility_minimum = "4.1"
    reloadable = true

    [libraries]

    macos.debug = "./libgdexample.macos.template_debug.dylib"
    macos.release = "./libgdexample.macos.template_release.dylib"
    windows.debug.x86_32 = "./gdexample.windows.template_debug.x86_32.dll"
    windows.release.x86_32 = "./gdexample.windows.template_release.x86_32.dll"
    windows.debug.x86_64 = "./gdexample.windows.template_debug.x86_64.dll"
    windows.release.x86_64 = "./gdexample.windows.template_release.x86_64.dll"
    linux.debug.x86_64 = "./libgdexample.linux.template_debug.x86_64.so"
    linux.release.x86_64 = "./libgdexample.linux.template_release.x86_64.so"
    linux.debug.arm64 = "./libgdexample.linux.template_debug.arm64.so"
    linux.release.arm64 = "./libgdexample.linux.template_release.arm64.so"
    linux.debug.rv64 = "./libgdexample.linux.template_debug.rv64.so"
    linux.release.rv64 = "./libgdexample.linux.template_release.rv64.so"

    ```

    (This is an exact copy of the corresponding code within Godot's official GDExtension documentation (Reference 1)).

    (I could have changed 'gdexample' and 'example_libary_init' to more meaningful entries, but for boilerplate like this, it's often safest to stick with the existing version.)

1. Next, within your src folder, create a new file called 'register_types.h.' Copy and paste the following code into that file:

    ```
    #pragma once

    #include <godot_cpp/core/class_db.hpp>

    using namespace godot;

    void initialize_example_module(ModuleInitializationLevel p_level);
    void uninitialize_example_module(ModuleInitializationLevel p_level);
    ```

    (Source: Reference 1)

1. These two files won't need to be modified further. The same is not the case for the following code, which we'll update over time to include the classes that we'll create within this tutorial. Go ahead and paste it into a new 'register_types.cpp' file within your src folder:

    ```
    #include "register_types.h"

    #include <gdextension_interface.h>
    #include <godot_cpp/core/defs.hpp>
    #include <godot_cpp/godot.hpp>

    using namespace godot;

    void initialize_example_module(ModuleInitializationLevel p_level) {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
        }

    }

    void uninitialize_example_module(ModuleInitializationLevel p_level) {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
        }
    }

    extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_example_module);
        init_obj.register_terminator(uninitialize_example_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
    }
    ```

    (Source: Reference 1, with a few lines removed so that we can add in our own versions later)

1. We'll also need to add a SConstruct file to our root folder in order to successfully compile our GDExtension classes. Visit https://docs.godotengine.org/en/4.6/_downloads/8df537a8866633825684515bce40faa6/SConstruct , which should prompt you to save an SConstruct file to your computer. Go ahead and save it into your root folder as 'SConstruct' (without any extension). (You can also access this download by visiting https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html , then searching for a link with the text "the SConstruct file we prepared.")

    Here's what your root folder should look like at this point (excluding certain Godot-created project files):

    ```
    godot_cpp_3d_tutorial/ [my root folder name; your name may vary]

    --godot-cpp/ [contains your compiled godot-cpp repository]

    --project/

    ----bin/

    ------gdexample.gdextension

    --src/

    ----register_types.cpp/

    ----register_types.h

    --SConstruct
    ```


1. Although we haven't created any classes yet, this will be a good time to compile the code we've added in so far. Navigate to your root folder and then run `scons platform=[your_os]` (e.g. `scons platform=linux` in my case). This is the same command you used to compile godot-cpp--just in your project folder rather than the godot-cpp folder. You should see `scons: done building targets.` appear in the terminal after all files have been compiled and linked.

## Part 2: Adding in a class

1. Now that we've gotten those setup tasks out of the way, we can begin programming our own GDExtension classes. Let's start with the Main class, which will govern the game area and some fundamental gameplay logic.

    Within your src folder, create a new file called 'main.h'. Copy the following code into this file:


    ```
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
    ```
    (Source: Reference 8)

    Here, we're declaring a GDExtension class called 'Main' that will inherit from Node. We're also adding some fundamental functions (e.g. constructors, destructors, _bind_methods(), and _ready()). Lots more will be added to this file later on, but I wanted to focus on the items we'll need at this moment.


1. Next, create a file within src/ called 'main.cpp' and paste the following code into it:

    ```

    #include "main.h"

    void Main::_bind_methods() {}

    Main::Main() {}

    Main::~Main() {}

    void Main::_ready() {

        UtilityFunctions::print("Main::_ready() just got called.");
    }
    ```

    (Source: Reference 8)

    This is all pretty barebones so far, but we'll expand this file quite a bit later in the tutorial.

    We don't need to add anything just yet to `_bind_methods()`, but since that function is a core part of many GDExtension classes, I figured I would add it here.

1. Go back to register_types.cpp. Directly under `#include "register_types.h"`, add:

    ```
    #include "main.h"

    ```

1. Next, within the `initialize_example_module()` function, add the following text right before the final closing bracket:

    ```
    GDREGISTER_RUNTIME_CLASS(Main);
    ```

    These two updates allow Godot to learn about our Main GDExtension class. We'll repeat these simple updates for all other classes that we define. 
    
    Note: I'm using GDREGISTER_RUNTIME_CLASS here rather than GDREGISTER_CLASS so that this code will only run when I'm actually running my project. With GDREGISTER_CLASS, code can also run (or at least attempt to run) in the editor, which can cause some irritating crashes. (For instance, suppose your code for a class references a Pivot object that you haven't yet created. If you then attempt to launch the editor after compiling this code, the editor will attempt to find this object, fail, and potentially crash. To avoid the need to comment out that code, add the object into your editor, and then recompile it, you can simply make that class a *runtime* class.)


    (Sources: References 8, 9, and 10)



1. Go ahead and run `scons platform=[your_os]` again. (If you haven't closed your terminal since the last time you ran this command, you may be able to access this line by pressing your Up Arrow key.) You should again see `scons: done building targets.` once the compilation process completes.

## Part 3: Setting up main.tscn

1. Now that we have a class (albeit a very simple one), we can create a scene based on it. Reopen Godot and open your project. Next, within the 'Create Root Node:' menu, select 'Other Node' and enter 'Main' in the search bar. Hopefully, you will see your newly-created Main node within the list of available nodes:

    ![](/tutorial_screenshots/main_node.png)

    If you *don't* see this node (a situation I've faced quite a few times), this likely means that you forgot one of the earlier steps (such as adding references to this node to your register_types.cpp file). 

1. Select this node, then save your scene as main.tscn. Next, click the play button near the top right of the editor in order to get the debug message we defined within `Main::_ready()` to display. This will bring up a scene-selection dialog box; you can hit 'Select Current', since we'll indeed want this to be our main scene.


1. A gameplay window with a gray box will open. Nothing will appear inside it, which is to be expected. However, you *should* see `Main::_ready() just got called.` appear within your Output tab in the lower half of the main editor window:

    ![](/tutorial_screenshots/main_ready.png)

1. While we're inside the editor, let's go ahead and create a game area. (Many of the following steps were based on the 'Setting up the game area' section of the Your First 3D Game Tutorial (Reference 3).) Right click on Main and select Add Child Node, then search for (and select) StaticBody3D. Rename it Ground within your scene tree on the left side of the editor. Next, add a CollisionShape3D and a MeshInstance3D as children of Ground.

1. Select the MeshInstance3D item within the scene tree if you haven't already. Click the 'empty' box to the right of Mesh in the Inspector and select a new BoxMesh. Next, click on CollisionShape3D within the scene tree; within the Shape row of the Inspector, choose a BoxShape3D.

1. Next, click on the Ground object within the scene tree, then click on Transform within the Inspector. Change the y entry within the Position (not Scale!) menu to -0.5 so that the top of the Ground, which is 1 meter thick, has a y position of 0 rather than 0.5.

1. Click the CollisionShape resource, then select the BoxShape. Within the Size menu that appears, change the x and z values to 60; keep the y value untouched at 1. Next, click on the MeshInstance, then click on the chain to the right of 'Scale' within the Transform section such that it appears broken. (This way, changes to one dimension won't affect the others.) As you did with the BoxShape, change the x and z scales to 60.0 and leave the y scale untouched at 1.0. Your Ground object should now look like a thin square.

1. Let's change the drab, white color of the Ground to something more interesting. Select your MeshInstance3D in the scene tree, then click the downwards-facing arrow to the right of the Mesh (and its corresponding gray cube) in the Inspector and select 'Edit.' Within the Material section, create a new StandardMaterial3D, then click the downwards arrow to the right of the white sphere that appears and select Edit. Click on the Albedo section, then select a color of your choice. 

Here's what the Ground should look like at this point:

![](/tutorial_screenshots/ground.png)

1. Before we can get to the 'action' part of this scene, we'll need lights and a camera. Add a DirectionalLight3D as a child of Main, then set its y transform to 20.0. (The x and z transforms can stay at 0.0.) Change its x rotation to -90, or whatever allows the light to point directly down at the ground. (You'll know it's working when you see the ground brighten up.) Check the Shadow box within the Light3D section of the Inspector as well.

1. Next, add a Marker3D as a child of Main and set its y and z transforms to 25.0 and -40.0, respectively. Finally, add a Camera3D as a child of the Marker3D. Set its x and y rotations (accessible within the Transform section of the Inspector) to -45 and 180, respectively. (You can scroll up to the top of the Inspector to get a preview of what the camera will display.) 

1. Try running the scene again. You should now see your game area within the window that appears:

![](/tutorial_screenshots/game_area.png)


1. Now that we have a game scene in place, this will be a good time to begin work on our Mnchar (main character) class. There's plenty more C++ code that will get added to main.cpp and main.h, but those additions will be easier to implement and debug once we have actual characters and projectiles to manage.

## Part 4: Laying the foundations for our Mnchar class

1. Our Mnchar class, which players will be able to control via game controllers, will fire projectiles and (potentially) get hit by other projectiles. We'll eventually configure our game such that anywhere from 2-8 Mnchars can get added to the game scene at the start of each game; however, that configuration will involve a Hud class that we won't be setting up for a little while.

1. To begin the setup process, create both a 'mnchar.h' and a 'mnchar.cpp' file within your src folder. Enter the following code into mnchar.h:

    ```
    #pragma once

    #include <godot_cpp/classes/character_body3d.hpp>
    #include <godot_cpp/variant/utility_functions.hpp>

    using namespace godot;

    class Mnchar : public CharacterBody3D {
    GDCLASS(Mnchar, CharacterBody3D)

    private:
    double movement_speed = 14;
    double rotation_speed = 0.15;
    
    protected:
    static void _bind_methods();

    public:
    Mnchar();
    ~Mnchar();

    void set_movement_speed(const double movement_speed);
    double get_movement_speed() const;

    void set_rotation_speed(const double rotation_speed);
    double get_rotation_speed() const;


    };
    ```

    (Source: References 1, 2, and 4)

    This code is very similar to that found within main.h. Two new additions of note are `movement_speed` and `rotation_speed` along with their corresponding setter and getter functions. We'll make it possible to update both of these values within the editor. 

    Also note that, because Mnchar will extend CharacterBody3D, we need to include its header file within this source file. (I chose to use CharacterBody3D as my player's class because the Your First 3D Game tutorial uses this same class; see Reference 5. Code for the CharacterBody3D class itself can be found in References 6 and 7.

1. Next, within mnchar.cpp, enter the following:

    ```
    #include "mnchar.h"
    #include <godot_cpp/core/class_db.hpp>

    using namespace godot;

    void Mnchar::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_movement_speed"),
                        &Mnchar::get_movement_speed);
    ClassDB::bind_method(D_METHOD("set_movement_speed", "p_movement_speed"),
                        &Mnchar::set_movement_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"),
                "set_movement_speed", "get_movement_speed");

    ClassDB::bind_method(D_METHOD("get_rotation_speed"),
                        &Mnchar::get_rotation_speed);
    ClassDB::bind_method(D_METHOD("set_rotation_speed", "p_rotation_speed"),
                        &Mnchar::set_rotation_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed"),
                "set_rotation_speed", "get_rotation_speed");

    }

    Mnchar::Mnchar() {}

    Mnchar::~Mnchar() {}



    void Mnchar::set_movement_speed(const double p_movement_speed) {
    movement_speed = p_movement_speed;
    }

    double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::set_rotation_speed(const double p_rotation_speed) {
  rotation_speed = p_rotation_speed;
}

double Mnchar::get_rotation_speed() const { return rotation_speed; }


    ```

    (Source: References 1 and 2)

    This code, like that in main.h, is quite barebones; our priority is simply to add in a class that the Godot editor will recognize. Once we create and configure a Mnchar scene within the editor, we'll then come back and extend this code. 

    However, I did also add in code that will allow us to modify the movement and rotation speeds within the editor. This isn't a crucial part of this particular game, but it *will* allow you to test out different movement and rotation values without having to recompile the code--not that that's a huge hurdle.

    The speed-adjustment code consists of two functions (`set_movement_speed()` and `get_movement_speed()`) that let us specify and retrieve, respectively, the player's speed. Both of these functions (including the former's `p_movement_speed` argument) have corresponding bind_method() calls within `_bind_methods()`. In addition, we have an ADD_PROPERTY() call that tells the editor about the `movement_speed` variable along with its setter and getter functions. (These are all based on the `amplitude` property within the GDExample class in Reference 1.) 
    
    The rotation-adjustment code is very similar. I simply copied and pasted my relevant movement-adjustment code, then replaced all cases of 'movement' with 'rotation.' 

    (It is *not* necessary to add in `bind_method()` and `ADD_PROPERTY()` calls like this for every attribute of a class. However, they're very important for certain items, such as signals--which we'll get to later.)


1. This new code should compile at this point; however, if we tried to do so, we most likely wouldn't be able to locate a Mnchar class within our editor. That's because we also need to update register_types.cpp with information about this class. Fortunately, this is easy to do so. Right under `#include "main.h"`, add `#include "mnchar.h"`. Next, right under `GDREGISTER_RUNTIME_CLASS(Main);`, add `GDREGISTER_RUNTIME_CLASS(Mnchar);`. 

1. *Now* run `scons platform=[your_os]` to compile this new Mnchar-related code. In my case, the editor still didn't show the Mnchar class within the 'Create New Node' menu after this step, but it did present it once I closed and relaunched my editor. Thus, I'd recommend that you do the same at this point.

## Part 5: Setting up mnchar.tscn

1. Back in the editor, create a new scene. Click the 'Other Node' button under the 'Create Root Node:' prompt; search for 'Mnchar'; then double-click it. Next, save this scene as mnchar.tscn.

    You *should* see the custom Movement Speed property that we configured near the top of the Inspector menu, along with our default value (14). Changing this value in the editor will also change the Mnchar's behavior within our game.

    ![](/tutorial_screenshots/early_mnchar_scene.png)

    (I have found, however, that this property will sometimes disappear from the editor after compiling my code. This might be caused by an issue with my current setup, but it might also be a glitch within the editor itself. Closing, then relaunching the editor always seems to resolve this issue, thankfully.)

1. Add a Node3D as a child of Mnchar and rename it 'Pivot'. In many cases, we'll apply movement and rotation actions to this node rather than to Mnchar itself. Next, add a MeshInstance as a child of Pivot; name it 'Body'; click the 'empty' text within its Mesh section in the Inspector; and select a BoxMesh. Next, click the downwards-pointing arrow to the right of the gray cube in the Inspector and select Edit. (Reference 5)

1. Within the Size section of the edit menu, change the x, y, and z values from 1.0 to 2.0. Next, go down to the Material section and select a new StandardMaterial 3D. Unlike with the game area, we won't choose a color for this material just yet--as we'll actually use C++ to update this color instead. (This will make it easier to assign different colors to different Mnchar instances.) 

1. We'll also want to create a turret for our player. Create a new MeshInstance3D child of Pivot; name it 'Turret'; assign it a new BoxMesh; go into that mesh's edit menu; change the x, y, and z sizes to 0.5, 0.5, and 0.5; and give it a new StandardMaterial3D. 

1. Next, navigate back to the Turret's main Inspector menu. (You can do so a few different ways; one of which is to select the Body, then the Turret again.) Within the Node3D section, set the z transform to 1.25 meters. That way, the turret will be adjacent to the forward face of the Pivot.

1. Add a CollisionShape3D as a child of *Mnchar* (not Pivot). Click on the 'empty' text within the Shape section of the Inspector; add in a BoxShape3D; click the downwards arrow to the right of 'BoxShape3D'; and select 'Edit.' Change the x, y, and z Size values to 2.0 in order to make it the exact same size as the Body component. 

    (You could also update these Size values, along with the transform of the CollisionShape, to allow it to fit over both the Body and Turret objects; alternatively, you could create a separate CollisionShape3D for the Turret. That would prevent the Turret from being able to enter into other objects. But this simpler approach will suffice for this tutorial.) (Reference 5)

    Once you're finished with these updates, go ahead and save the scene.

1. In order to move the Mnchar with a controller (or, for development purposes, a keyboard), we'll need to add input actions to our game's Input Map. Navigate to this map by clicking Project in the top left of the Godot editor window, then selecting Project Settings; the Input Map should be the second tab from the left. (Reference 5)

1. For now, we'll just add in keyboard entries; once we're further in the development process, we'll add in controller entries also. Click on the 'Add New Action' text within the Input Map menu; type move_left_0; and hit the '+ Add' button to the right of this window. Next, click the + sign to the right of the new 'move_left_0' entry that has appeared; and hit your J key (or, if you're using a different layout like I am, where the J key would be on a QWERTY keyboard).

    (You're welcome to use a key other than J, such as Left Arrow, if you'd like. The use of 'J' will make more sense in the context of all the keys we'll be adding in.)

    ![](/tutorial_screenshots/first_input_map_entry.png)

1. Perform the same steps for the following action names and keys:

    1. move_right_0 (L key)
    1. move_forward_0 (I key)
    1. move_back_0 (K key)
    1. rotate_left_0 (S key)
    1. rotate_right_0 (F key)
    1. fire_0 (Space Bar)
    1. reset_0 (O key)

1. Once you've finished this process, your input map should look like the following:

    ![](/tutorial_screenshots/player_0_keyboard_input_map.png)

    By the way, the reason for adding '_0' to the end of these actions is to allow different sets of controls to be distinguished for different players later on.

    Close out of the input map and save your mnchar.tscn file.

## Part 6: Adding a Mnchar to the game area

1. We're almost ready to add in code that will let us move our Mnchar around the game area. First, though, we need to *add* the Mnchar to the game area.

1. One option would be to instantiate a Mnchar as a child scene of main.tscn (Reference 4). However, since we're creating a multiplayer game whose player count might change from round to round, it will be more ideal to add Mnchars to this scene via code. (That way, a specific number of Mnchars can be placed within the game area depending on how many players choose to enter a given game.) The following steps will allow us to add a Mnchar to the scene using C++.

1. Within main.h, add the following code right above `protected:`

    ```
    private:
        Ref<PackedScene> mnchar_scene;
    ```

    Next, add the following right below `~Main();`

    ```
    Ref<PackedScene> get_mnchar_scene();
    void set_mnchar_scene(Ref<PackedScene>);
    ```

    (Reference 8)

    Finally, after `#include <godot_cpp/variant/utility_functions.hpp>` , add:

    ```
    #include <godot_cpp/classes/packed_scene.hpp>
    #include "mnchar.h"
    ```

1. Next, within main.cpp, add the following code within `Main::_bind_methods():`

    ```
      ClassDB::bind_method(D_METHOD("get_mnchar_scene"), &Main::get_mnchar_scene);
    ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_scene", "get_mnchar_scene");
    ```

    (Reference 8)

1. In addition, add the following code below `Main::~Main() {}`:

    ```
    Ref<PackedScene> Main::get_mnchar_scene() { return mnchar_scene; }

    void Main::set_mnchar_scene(Ref<PackedScene> packed_scene) {
    mnchar_scene = packed_scene;
    }
    ```

    (Reference 8)


    We're defining a scene (`mnchar_scene`) that we'll be able to access within main.cpp. Adding it as a property (like we did with movement_speed) will allow us to specify, within the editor, the exact scene (in this case, mnchar.tscn) that we want Godot to interpret as `mnchar_scene`.

1. Next, add the following to the bottom of `Main::_ready()`:

    ```
    auto new_mnchar =
        reinterpret_cast<Mnchar *>(get_mnchar_scene()->instantiate());
    
    add_child(new_mnchar);
    ```

    (Reference 8)

    This code retrieves our mnchar_scene; reinterprets it as a Mnchar object; and then adds it to our scene tree.


1. Note: When I was putting this code together, I initially forgot to define `get_mnchar_scene()` and `set_mnchar_scene()` within main.cpp. Although my code compiled successfully, these omissions caused Godot to fail to locate both my Main and Mnchar classes within the editor:

    ![](/tutorial_screenshots/red_xs_due_to_missing_functions.png)

    (I knew this was the cause because I've made similar mistakes more often than I'd like to admit!)

    Adding these functions in, then closing and relaunching the editor resolved this issue.

    I actually managed to make a similar mistake later on for `get_rotation_speed()` and `set_rotation_speed()`. Error messages within the console helped me figure out the issue. (Note the `get_rotation_speed` reference within the third-to-last error.)

    ![](/tutorial_screenshots/scary_but_helpful_error_messages.png)

1. Go ahead and compile your code, then relaunch the editor. After you open main.tscn and click on the Main node within your scene tree, you should now see a new Packed Scene entry right below Main in the inspector. Click the 'empty' text; select 'Load'; and then choose your mnchar.tscn scene. 

1. When you try playing your project, you should now see a little gray box (your Mnchar) in the middle of your game area:

    ![](/tutorial_screenshots/mnchar_added_via_code.png)


1. You'll notice, though, that the main character is partially sunk in the ground. We could fix this by changing its transform within Mnchar.tscn; however, because we'll need to be able to move Mnchars around later on when setting up multiplayer games, we may as well add some initial movement code now.

    Declare a new `start()` function for the Mnchar class by adding the following code right before the final closing bracket of mnchar.h:

    ```
    void start(Vector3 mnchar_translate_arg);
    ```

    Next, add the following code to the bottom of mnchar.cpp:

    ```
    void Mnchar::start(Vector3 mnchar_translate_arg)
    {translate(mnchar_translate_arg);}
    ```

    This function will ultimately allow us to configure each player's starting color, location, rotation, and ID. For now, though, we'll just use it to move our player to a better starting location.

    (Resource 8)

1. Within Main::_ready(), add the following code right above `add_child(new_mnchar)`:

    ```
    new_mnchar -> start(Vector3(15, 1, -20));
    ```

    This will move the Mnchar 15 meters to the left, one meter up (to make its bottom level with the ground), and 20 meters closer to the camera. We'll add in code later to give other Mnchars different starting locations, but all of them will also get moved up one meter.

    (Resource 8)

    Note: I had originally tried this code:

    `get_node<Node3D>("Pivot") -> translate(translate_val);`

    However, this caused issues when multiple characters were added to the scene--possibly because the transforms of the actual Mnchar class weren't getting changed and were thus overlapping with one another. (This caused them to shoot up in the sky, which was both frustrating and hilarious!)

1. Compile your code, then rerun your main scene. You should now see the full Mnchar closer to the bottom left of the window:

    ![](/tutorial_screenshots/repositioned_mnchar.png)

## Part 7: Moving the Mnchar

1. Because we're creating a multiplayer game, we'll want to set up our movement code in a way that allows each player to move his or her own Mnchar (and no one else's). The approach we'll take for this task will be as follows:

    1. We'll first create a new String variable (`mnchar_id`) that will store a unique ID for each Mnchar. (These IDs will range from "0" to "7" in order to support up to eight different players.) This variable will also be useful for setting character-specific locations, rotations, and colors.
    
    1. We'll also add each of these same IDs to the end of all action names within one particular group of movement commands that we'll create. Thus, one group's action names will end in "0", others will end in "1", and so forth. (This is why we added "_0" to our first set of movement names.) We'll also specify that each set of commands will only work for one particular device. (The device IDs recognized by Godot range from 0 through 7, thus matching our own list of possible IDs.)

    1. In our movement code, we'll check for movements that match the given Mnchar's ID. For instance, here's what our left/right movement code will look like:

        ```
        x_direction = input->get_axis("move_left_"+mnchar_id, 
        "move_right_"+mnchar_id);
        ```
    1. In summary, by having Mnchar IDs match movement-name suffixes, and by having all movements for a particular suffix match only one particular device, we can create a functioning multiplayer control setup without too much extra work. (This approach was based on references 11 through 15.) 

1. The first step here will be to add a `mnchar_id` value to our Mnchar class. Under `double movement_speed = 14;` within the `private:` section of Mnchar.h, add:

    ```
    String mnchar_id = "";
    ```

    Next, in the `public:` section, add the following setter and getter function declarations under your `get_movement_speed()` function declaration:

    ```
    void set_mnchar_id(const String mnchar_id);
    String get_mnchar_id() const;
    ```

    Finally, add a `mnchar_id_arg` argument before your `mnchar_translate_arg` within `start()` so that the declaration matches the following line:

    ```
    void start(String mnchar_id_arg, Vector3 mnchar_translate_arg);
    ```

1. Within mnchar.cpp, add the following below your `get_movement_speed()` function definition:

    ```
    void Mnchar::set_mnchar_id(const String p_mnchar_id) {
    mnchar_id = p_mnchar_id;
    }

    String Mnchar::get_mnchar_id() const { return mnchar_id;}
    ```

    Next, add `String mnchar_id_arg` before `Vector3 mnchar_translate_arg` within the arguments in this file's `Mnchar::start()` function definition. In addition, right before `translate(mnchar_translate_arg);` in the function body, add: `set_mnchar_id(mnchar_id_arg);`.

1. Finally, within main.cpp, update your `new_mnchar -> start()` command within `Main::_ready()` so that it reads as follows:

    ```
    new_mnchar -> start("0", Vector3(15, 1, -20));
    ```

    (This will assign `new_mnchar` an ID of 0, thus allowing all movement commands ending in "0" to get registered by this Mnchar.)

    If you want to confirm that this change has taken effect, you can also add the following code following `set_mnchar_id(mnchar_id_arg)` within `Mnchar::start()`:

    ```
    UtilityFunctions::print("Mnchar's ID is ", get_mnchar_id(), ".");
    ```

    (Printing out values is incredibly helpful for debugging work.)

    We could also have added `bind_method()` and `ADD_PROPERTY()` calls for our `mnchar_id` value and its corresponding setter and getter functions (as we did with the `movement_speed` variable). However, that won't be necessary in this case, as we won't need to access or modify those values directly within the editor.

1. Now that we have code in place for assigning `mnchar_id`s, we can utilize that ID within our input code. First, add the following code after your `start()` function declaration within mnchar.h:

    ```
    void _physics_process(double delta) override;
    ```

    (See Reference 4 for the use of `_physics_process` here rather than `_process`.)

    In addition, add the following three include statements after `#include <godot_cpp/variant/utility_functions.hpp>`:

    ```
    #include <godot_cpp/classes/input.hpp>
    #include <godot_cpp/classes/input_event.hpp>
    #include <godot_cpp/classes/input_map.hpp>
    ```

1. Next, add the following code to the end of mnchar.cpp. We'll start each `_physics_process` call by retrieving input data that we can then parse. (The lack of a closing bracket is intentional, since we'll be filling out the rest of this function below.)

    ```
    void Mnchar::_physics_process(double delta) {

    auto input = Input::get_singleton();

    ```

    (Reference 4)

1.  Extend this function by adding the following code, which uses our left, right, forward, and back movements to determine the player's movement along the x (left/right) and z (forward/back) axes. This approach will allow different movement speeds depending on exactly how far a controller joystick is moved down, but it also works fine for keyboard input.

    (Note: I've found that I sometimes need to put 'move_right' before 'move_left', and 'move_forward' before 'move_back', in order to get my movement code to work correctly.)

    ```
    float x_direction =
        input->get_axis("move_left_" + mnchar_id, "move_right_" + mnchar_id);
    float z_direction =
        input->get_axis("move_forward_" + mnchar_id, "move_back_" + mnchar_id);
    ```
    Also note the inclusion of mnchar_id, which I discussed at length earlier.

    (References 16 and 17)

1. Next, we'll rotate the player in response to any rotation commands sent by the player's controller (or, if `mnchar_id` is 0, the keyboard). Add the following code to the end of the function:

    ```
    get_node<Node3D>("Pivot")->rotate_object_local(
        Vector3(0, 1, 0),
        rotation_speed * input->get_axis("rotate_right_" + mnchar_id,
                                        "rotate_left_" + mnchar_id));
    ```

    (References 16, 18, and 19)

1. We'll now retrieve information about the Mnchar's basis that we'll use to update its velocity. Add the following code to the end of `_physics_process()`:

    Note: I'm not sure why, but multiplying the x and z components
  of the x and z bases, respectively, by -1 was critical for getting
  the movement code to work. This may be due to an issue with my setup. I imagine that there's a way to update my code such that at least one of these multiplication commands won't be necessary.

    ```
    auto player_transform_basis_z =
        get_node<Node3D>("Pivot")->get_transform().get_basis()[2];

    auto player_transform_basis_x =
        get_node<Node3D>("Pivot")->get_transform().get_basis()[0];

    player_transform_basis_x.x *= -1;

    player_transform_basis_z.z *= -1;
    ```

1. Extend the function by adding the following code, which initializes, then updates, the player's target velocity.

    ```
    Vector3 target_velocity = Vector3(0, 0, 0);

    float abs_z_direction = std::abs(z_direction);
    float abs_x_direction = std::abs(x_direction);

    if (abs_z_direction >= abs_x_direction) {
        target_velocity +=
            -1 * player_transform_basis_z * z_direction * movement_speed;
    } else {
        target_velocity +=
            -1 * player_transform_basis_x * x_direction * movement_speed;
    }
    ```

    I would like Mnchar to be able to move *only* forward, back, left, or right relative to its current position (i.e. not diagonally). Therefore, the code above finds the absolute value of the x and z directions, then moves the player along the axis with the greatest absolute value. This isn't strictly necessary, but I find it makes the Mnchar's movement somewhat more intuitive.


    (Note that I'm using the standard library's abs() function rather than Godot's, as the latter appeared to truncate values down to the nearest int.)


    (References 4, 17, 20, and 21)

1.  Finally, close out this function with the following code:
    ```
    set_velocity(target_velocity);

    move_and_slide();
    }
    ```

    (References 4 and 21)


1. Reopen your editor, launch the scene, and test out the controls. Make sure that no directions are the inverse of what you'd expect--and that the player cannot move diagonally.

    By the way: if the game crashes right when you launch it, make sure that your mnchar.tscn scene is still present within Main's Packed Scene attribute. (It sometimes disappears on my end, but thankfully, it's easy to add back in.)


# Here with editing:

Next, add in projectile-launch code (which will first involve creating a projectile), followed by code that allows the player to respond to being hit by a projectile (which will involve adding in a projectile detector).


## References

Notes: 

* In some cases, a reference within this list was itself based heavily (or even entirely) on another reference. However, in order to keep this list simple and manageable, I won't include such details. You can find more information about the sources used for these references within their own repositories.

* In addition, references that begin with '/godot-cpp' refer to a file within a *compiled* godot-cpp repository. (See Part 1 for more details on (1) how to access and compile this repository and (2) the exact version I'm using.)

* Not all references are listed within every paragraph. For instance, if multiple paragraphs in a row use the same reference, I might only cite that reference within some of those paragraphs.

* Reference 1: https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html

* Reference 2: https://github.com/kburchfiel/godot_cpp_3d_demo/

    * Since this repository is essentially a step-by-step tutorial to creating the code in Reference 2, both source files correspond very closely to one another. For example, the mnchar.h code within this repository was based mostly on https://github.com/kburchfiel/godot_cpp_3d_demo/blob/main/src/mnchar.h .

* Reference 3: https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/01.game_setup.html

* Reference 4: https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html

* Reference 5: https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/02.player_input.html

* Reference 6: /godot-cpp/gen/src/classes/character_body3d.cpp

* Reference 7: /godot-cpp/gen/include/godot_cpp/classes/character_body3d.hpp

* Reference 8: https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6
    
    * The vast majority of the code in this repository came from https://github.com/j-dax/gd-cpp , which was released under the BSD-3-Clause license by Matthew Piazza. My repository simply converted that code into a step-by-step guide (similar to this one one).

    * In order to simplify this reference list, I won't link to all of the individual source files that I consulted. However, I recommend checking player.cpp and player.h for the Mnchar class's code; main.cpp and main.h for the Main class's code; and hud.cpp and hud.h for the Hud class's code. (These source files are available within https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/tree/main/src .)

* Reference 9: https://godotengine.org/releases/4.3/#gdextension-runtime-class-registration

* Reference 10: https://forum.godotengine.org/t/gdextension-register-runtime-class/77868/4?u=kburchfiel

* Reference 11: https://www.reddit.com/r/godot/comments/13ikz4u/best_way_to_handle_controller_input_for_local/

* Reference 12: https://github.com/remram44/godot-multiplayer-example

* Reference 13: https://www.gdquest.com/library/split_screen_coop/

* Reference 14: https://godotassetlibrary.com/asset/QdddqG/multiplayer-input

* Reference 15: https://kidscancode.org/godot_recipes/3.x/2d/splitscreen_demo/index.html

* Reference 16: https://docs.godotengine.org/en/stable/tutorials/2d/2d_movement.html

* Reference 17: https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd

* Reference 18: https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html

* Reference 19: /godot-cpp/gen/src/classes/node3d.cpp

* Reference 20: /godot-cpp/src/variant/vector3.cpp

* Reference 21: /godot-cpp/include/godot_cpp/variant/vector3.hpp
