.. _doc_compiling_from_source:

Compiling from Sources
===============================

Setup
-----

FMOD is a commercial library so you will need to download the FMOD headers and libraries yourself. Libraries should be placed in 'libs/<platform>/<architecture>'.Headers should be placed in 'headers/' The fmod_installer.py script is also able to automatically install the headers for some platforms

.. code-block::
   # use --help for more options
   python3 fmod_installer.py setup <Fmod Version Number>

Build
-----

Use export.py to build the extension and add it to a project. Which manages copying over the necessary fmod dependencies

.. code-block::
   # use --help for more options
   python3 export.py build -p <platform>
   python3 export.py export <path/to/project>/addons

Alternatively refer to `introduction to the buildsystem`_

.. _introduction to the buildsystem: https://docs.godotengine.org/en/stable/engine_details/development/compiling/introduction_to_the_buildsystem.html
