.. _doc_c_sharp_installation_guide:

C# installation guide
===============================
To use the Fmod C# wrappers it is necessary to replace FMOD.VERSION.dll and
FMOD.Studio.STUDIO_VERSION.dll constants to use the proper fmod library version.
snippets below

.. code-block::

    public partial class VERSION
    {
        public const int number = 0x00020309;

    #if DEBUG || TOOLS
        public const string logging = "L";
    #else
        public const string logging = "";
    #endif
    #if GODOT_LINUXBSD
        public const string dll = $"fmod{logging}.so.14";
    #else
        public const string dll = "fmod{logging}";
    #endif
    }

.. code-block::

    public partial class STUDIO_VERSION
    {
    #if DEBUG || TOOLS
        public const string logging = "L";
    #else
        public const string logging = "";
    #endif
    #if GODOT_LINUXBSD
        public const string dll = $"fmodstudio{logging}.so.14";
    #else
        public const string dll = $"fmodstudio{logging};
    #endif
    }

.. seealso:: For info on how to compile from source see
   :ref:`Compiling from Source <doc_compiling_from_source>`.
