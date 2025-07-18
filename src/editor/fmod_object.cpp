#ifdef TOOLS_ENABLED
#include "fmod_object.h"
#include <iostream>
using namespace std;
namespace FmodGodot
{

    std::ostream &operator<<(std::ostream &stream, const Bank object)
    {
        stream << '[';
        for (const String &str : object.events)
        {
            stream << str.utf8() << ',';
        }
        stream << ']';
        return stream;
    }
    std::ostream &operator<<(std::ostream &stream, const Event object)
    {
        stream << "Is3D:" << object.is3d << " Doppler:" << object.doppler_enabled << " Length:" << object.lengthMS << " one_shot:" << object.one_shot << " min:" << object.min << " max:" << object.max;
        for (auto param : object.parameters)
        {
            stream << "\t[" << param.full_path.utf8().ptr() << param << ']';
        }
        return stream;
    }
    std::ostream &operator<<(std::ostream &stream, const Parameter object)
    {
        stream << '[' << object.min_value << ".." << object.default_value << ".." << object.max_value << "] discrete:" << object.discrete;
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const VCA object)
    {
        return stream;
    }
}
#endif