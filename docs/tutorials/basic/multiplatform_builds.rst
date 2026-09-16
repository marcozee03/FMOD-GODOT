.. _doc_multiplatform_builds:

Multiplatform Builds
====================
.. note::

    Multiplatform builds are experimental and subject to change. a general purpose plugin like `Resource Remaps`_ might be more useful for your project.

Setup
=====

In project settings set *fmod/banks/import/source_type* to 'Multiple Platform Build'

Multiple platform builds apply several restrictions to your project structure. The structure can be configured in project settings.

* fmod/banks/import/bank_directory: Set the directory to look for banks

Banks for different platforms will be stored in subdirectories of the configured bank_directory. They
should be named after feature tags. For example, "pc", "linux", "mac". You can even combine feature tags.
For example, "linux.double". For your project you will likely want bank_directory/pc to be your main
directory which will then get remapped for other platforms.

.. _Resource Remaps: https://store.godotengine.org/asset/allenwp/resource-remaps/
