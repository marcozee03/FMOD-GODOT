.. _doc_compiling_from_source:

Compiling from Sources
===============================

Before starting, you will need to add the FMOD headers and libraries to the appropriate
location. To simplest way to compile this addon to your project is to run the export.py
script example below. Libraries should be placed in libs/<platform>/<architecture>

.. code-block::
   python3 export.py -p <platform> -o /path/to/godot_project/addons

**Flags**
-o  Output
    flag specifies where to copy the outputs of the command to notice how it should
    be the path to the addons directory of your Godot project.
-p  Platform
    flag is currently one of 3 options "windows" "linux" and "all"
-f  force
    skips the prompt asking for your permission to override files in the output path.
-sc Skip
    skips the compilation steps and copies whatever is currently in the bin
    directory to output path.

Alternatively refer to `Link Introduction to the buildsystem <https://docs.godotengine.org/en/stable/contributing/development/compiling/introduction_to_the_buildsystem.html>
