# Plugins

For ComCheck, a plugin is a dynamic library which can analyze lines of specific files and says that this line is rather a comment or a code line.

The algorithm that determine this classification could be directly integrated in ComCheck. But if we do like this, you won't be able to analyze two differents languages with the same program. That's why, in ComCheck, one plugin describe on language.

The user can use the default ComCheck which contains the C++ plugin. But he can also download and compile other plugins for other languages, and even create his own plugin for his specific use.

## How it works ?

A plugin requires at least two pieces of information and three at most:

    a list of extension to be able to filter every file (mandatory)
    a language which play the role of title for the plugin (mandatory)
    a rank in order to display the list of plugins in a specific order

The interface of plugins could be find in ComCheck sources in iplugin.hpp

## How could you create your own plugin ?

In the folder, you can find a folder called `template` which describes a useless plugin. You can just copy it, paste it in another folder and work with the copy by opening the .pro in QtCreator (Qt4 or Qt5).

To create your own plugin, you need to modify some data from this template project :

In the .pro, the TARGET represents the name of your plugin so be sure that there is no other plugin called `libTARGET.a`,  `libTARGET.so` or `TARGET.dll`. I recommend you to change the class name. I don't think that it will create a problem but
> to be prepared is half the victory
.

You can change every important values in the constructor of your class. And you can add any method you need. The only you need to know is that ComCheck will call your plugin by the `get_type` method for each line of each file with the right extension. You can implement as you wish but be sure that you return a `CC_Flag`. If you use Qt4, you need to check if the first parameter in the `Q_EXPORT_PLUGIN2` macro is exactly the same string as `TARGET`.

At the end, you just need to build this project (Ctrl+B or bottom right button) to create the dynamic library in the plugins folder.

## This is how I have create the C++ plugin:

    1: copy 'template' folder and paste it by changing the name into 'CPP'
    2: enter in 'CPP' and rename every file in 'cpp.*' instead of 'template.*'
    3: open QtCreator and open the cpp.pro in it, click on 'Configure' if it asks
    4: rename every 'template.*' in 'cpp.*' and 'plugin_Template' into 'plugin_CPP' and save
    5: double click on 'cpp.pro' in the file list at the left to include cpp.cpp and cpp.hpp
    6: open the .cpp and rename the header include in the first line in #include "cpp.hpp"
    7: change the extension and the language in the constructor
    8: open the .hpp, make Ctrl+Shift+F:
                      put 'Scope' on 'CurrentProject'
                      write 'TemplatePlugin' in 'Search for'
                      check 'Case sensitive'
                      click on 'Search & Replace'
                      write 'CPP_Plugin' in 'Replace with'
                      click on 'Replace'
    9: in the .cpp, in the Q_EXPORT_PLUGIN2 macro, replace 'plugin_Template' by 'plugin_CPP'

Now I just need to implement the CPP algorithm in the `get_type` method, and build the project, to create the perfect plugin for ComCheck.

