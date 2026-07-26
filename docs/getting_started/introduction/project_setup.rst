.. _doc_project_setup:

Project Setup
=============

Project Settings
----------------

Goto **Project > Project Settings > Fmod** If you do not see FMOD toggle advanced settings.

.. General
.. ^^^^^^^
..
.. **Sample Rate** - The audio sample rate that FMOD's mixer should use. Lower sample rates reduce mixer CPU usage but also reduce audio quality.
..
.. **Live Update** - One of Disabled/Enabled/Development Build Only. Determines when Live Update is enabled. Combine with feature tags for additional customization.
..
.. **Live Update Port** - The network port that FMOD will use to accept connections from FMOD Studio.
..
.. **3D Settings**:
..
.. * Distance Factor - A factor for converting game distance units to FMOD distance units.
.. * Doppler Scale - A scaling factor for doppler shift.
.. * Rolloff Scale - A scaling factor for distance attenuation. When a sound uses a roll-off mode other than FMOD_3D_CUSTOMROLLOFF and the distance is greater than the sound's minimum distance, the distance is scaled by the roll-off scale.
..
.. **Channels**:
..
.. * Real Count - The number of channels that will be audible. Lowering this count will reduce the FMOD mixer's CPU usage. See the virtual voice system document for more information.
.. * Virtual Count - The number of channels that can be played. Once the Real Channel Count is exceeded, the quietest channels will be virtualised, meaning they are not audible. See the virtual voice system document for more information.
..
.. **DSP**:
..
.. The DSP buffer configuration that the FMOD mixer should use.
..
.. * Buffer Length - The length of each DSP buffer.
.. * Buffer Count - The number of DSP buffers to use.
..
.. Banks
.. ^^^^^
..
.. **Source Type**:
.. Specifies the method FMOD Godot uses to access your source .bank files.
..
.. * Single Platform Build - You have a directory containing .bank files for a single platform. You specify the location of the directory in the Build Path setting.
..
.. * **WORK IN PROGRESS** Multiple Platform Build - You have a directory containing .bank files for multiple platforms, with each platform in its own subdirectory. You specify the location of the top-level directory in the Build Path setting
..
.. * FMOD
