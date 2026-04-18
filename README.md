## Godot CPP 3D Tutorial [Work in progress]

By Ken Burchfiel

Released under the MIT license

This step-by-step guide will demonstrate how to create a 3D multiplayer game in Godot using C++. It is based on my [Cube Combat demo](https://github.com/kburchfiel/godot_cpp_3d_demo), but (unlike that project) guides you more explicitly through the steps involved, both within your code editor and Godot, to put this kind of game together.

*Note: This resource is being created without the use of generative-AI tools.*


## Part 1: Getting started

(Note: Many of these steps will resemble those in the excellent 'Getting started' section of the official GDExtension documentation at https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html . Certain code blocks within this section derive from that document as well.)

1. Go ahead and create a new folder that will store your Godot project and its corresponding code. I'll call mine `godot_cpp_3d_tutorial`, but the name you choose is of course up to you.

1. First, you'll want to download the latest stable version of godot-cpp from https://github.com/godotengine/godot-cpp . (As of 2026-04-17, a stable version of version 10.x hasn't yet been released, so I went ahead and downloaded the beta version with a commit ID of 4862a9d (https://github.com/godotengine/godot-cpp/tree/4862a9dcf1471c9ea19680b9faadb5b6a9432092 .) Whether you download and unzip or simply clone it, make sure that exists within your project folder within a folder named 'godot-cpp'.

1. Next, open up this godot-cpp folder within your terminal and run `scons platform=linux` (replacing `linux` with your own OS if needed). This will compile all of the source code needed to apply this library. (It will *also* generate additional code files that aren't visible in an uncompiled version of the repository, such as the one on GitHub).

1. Go ahead and create a 'src' folder within your root project folder. This folder will store your source C++ code and its compiled variants.

1. Create a 'project' folder within this root folder also. Next, open up Godot, which you can download from https://godotengine.org/ if you haven't already. (I'm using Godot 4.6 for this project, but this tutorial should be applicable for newer releases also for a decent while.) Within the loading screen, hit the Create button at the top left. I chose 'Cpp 3D Tutorial' as my project name and the 'project' folder I just created as my path. Once you've filled in these items, hit Create on the bottom right.

    ![](/tutorial_screenshots/new_game_project.png)


1. Next, go ahead and close out of the editor. Before we create a scene, we should first create a GDExtension class within C++ that can be used as the basis for that scene. (This is a different approach than what you might be used to with GDScript.)

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
    (Source: Reference 2)

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

    (Source: Reference 2)

    This is all pretty barebones so far, but we'll expand this file quite a bit later in the tutorial.

    We don't need to add anything just yet to `_bind_methods()`, but since that function is a core part of many GDExtension classes, I figured I would add it here.

1. Go back to register_types.cpp. Directly under `#include "register_types.h"`, add:

    ```
    #include "main.h"

    ```

1. Next, within the `initialize_example_module()` function, add the following text right before the final closing bracket:

    ```
    GDREGISTER_CLASS(Main);
    ```

    These two updates allow Godot to learn about our Main GDExtension class. We'll repeat these simple updates for all other classes that we define. (Source: Reference 2)

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



## References

Note: In some cases, a reference within this list was itself based heavily (or even entirely) on another reference. However, in order to keep this list simple and manageable, I won't include such details. You can find more information about the sources used for these references within their own repositories.

Reference 1: https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html

Reference 2: https://github.com/kburchfiel/godot_cpp_3d_demo/blob/main/src/main.h

Reference 3: https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/01.game_setup.html