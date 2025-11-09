:allow_comments: False

Fmod Godot Docs – *master* branch
============================

Welcome to the official documentation of `Fmod Godot <https://github.com/marcozee03/FMOD-GODOT/tree/master>`__,

Fmod-Godot is a GDExtension library that integrates the Fmod API with the Godot Engine. The library provides a way for C# to interact directly with the same FMOD Studio/Core system instances the FmodAudioServer uses. So, it is not limited to what the integration handles explicitly.

Unlike `Link FMOD GDExtension<https://github.com/utopia-rise/fmod-gdextension> this The extension doesn't expose the low-level FMOD and FMOD Studio APIs to GDScript. The initial design Goal/Motivation of this was to add a high-level integration with the Godot Engine while allowing C# to use the official FMOD C# wrapper.

The table of contents in the sidebar should let you easily access the documentation
for your topic of interest. You can also use the search function in the top-left corner.

.. Below is the main table-of-content tree of the documentation website.
   It is hidden on the page itself, but it makes up the sidebar for navigation.

.. toctree::
   :hidden:
   :maxdepth: 1
   :caption: About
   :name: sec-general

   about/introduction

.. toctree::
   :hidden:
   :maxdepth: 1
   :caption: Getting started
   :name: sec-learn

   getting_started/introduction/index

.. Sections below are split into two groups. First come meta sections, covering
   general matters. Below that different areas of the engine are listed.
   These sections are sorted alphabetically. Please keep them that way.
.. toctree::
   :hidden:
   :maxdepth: 1
   :caption: Manual
   :name: sec-tutorials

   tutorials/placeholder/index


.. toctree::
   :hidden:
   :maxdepth: 1
   :caption: Class reference
   :name: sec-class-ref

   classes/index
