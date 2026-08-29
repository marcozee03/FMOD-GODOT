#pragma once

#include "binding/conversions.h"
#include "classes/global_constants.hpp"
#include "core/property_info.hpp"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/char_string.hpp"
#include "variant/packed_string_array.hpp"
#include "variant/variant.hpp"
#include "variant/vector2i.hpp"
#include <fmod_studio.h>
#include <godot_cpp/templates/vector.hpp>
#include <variant/string.hpp>
#include <variant/vector4i.hpp>

using namespace godot;
namespace FmodGodot
{

struct ParameterCache
{
    String full_path;
    String name;
    Vector4i guid;
    GD_PARAMETER_ID id;
    float min_value;
    float default_value;
    float max_value;
    FMOD_STUDIO_PARAMETER_FLAGS flags;
    FMOD_STUDIO_PARAMETER_TYPE type;
    PackedStringArray labels;
    constexpr bool is_readonly()
    {
        return flags & FMOD_STUDIO_PARAMETER_READONLY;
    }
    constexpr bool is_automatic()
    {
        return flags & FMOD_STUDIO_PARAMETER_AUTOMATIC;
    }
    constexpr bool is_global()
    {
        return flags & FMOD_STUDIO_PARAMETER_GLOBAL;
    }
    constexpr bool is_discrete()
    {
        return flags & FMOD_STUDIO_PARAMETER_DISCRETE;
    }
    constexpr bool is_labeled()
    {
        return flags & (FMOD_STUDIO_PARAMETER_LABELED | FMOD_STUDIO_PARAMETER_DISCRETE);
    }
    ParameterCache()
    {
    }
    ParameterCache(const FMOD_STUDIO_PARAMETER_DESCRIPTION &p_description)
    {
        name = p_description.name;
        full_path = "parameter:/" + name;
        guid = cast_to_vector4i(p_description.guid);
        default_value = p_description.defaultvalue;
        flags = p_description.flags;
        min_value = p_description.minimum;
        max_value = p_description.maximum;
        id = cast_to_gd_parameter_id(p_description.id);
        type = p_description.type;
    }
    static PropertyInfo propInfo(const FMOD_STUDIO_PARAMETER_DESCRIPTION &p_parameter,
                                 FMOD_STUDIO_EVENTDESCRIPTION *p_event)
    {
        PropertyInfo info;
        info.name = "parameters/" + String(p_parameter.name);
        info.usage = PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;
        if (p_parameter.flags & (FMOD_STUDIO_PARAMETER_LABELED | FMOD_STUDIO_PARAMETER_DISCRETE))
        {
            info.hint = PROPERTY_HINT_ENUM;
            info.type = Variant::INT;
            PackedStringArray labels;
            labels.resize(p_parameter.maximum + 1);
            for (int label_index = 0; label_index <= p_parameter.maximum; label_index++)
            {
                int retrieved = 0;
                FMOD_Studio_EventDescription_GetParameterLabelByID(p_event, p_parameter.id, label_index, nullptr, 0,
                                                                   &retrieved);

                CharString label;
                label.resize_uninitialized(retrieved + 1);
                if (retrieved > 0)
                {
                    int size = retrieved;
                    FMOD_Studio_EventDescription_GetParameterLabelByID(p_event, p_parameter.id, label_index,
                                                                       label.ptrw(), size, &retrieved);
                    label[size - 1] = ':';
                    label[size] = '\0';
                }
                labels.set(label_index, String(label.ptr()) + itos(label_index));
            }
            info.hint_string = String(",").join(labels);
        }
        else
        {
            info.hint = PROPERTY_HINT_RANGE;
            info.type = Variant::FLOAT;
            if (p_parameter.flags & FMOD_STUDIO_PARAMETER_DISCRETE)
            {
                info.hint_string = vformat("%f,%f,1", p_parameter.minimum, p_parameter.maximum);
            }
            else
            {
                info.hint_string = vformat("%f,%f", p_parameter.minimum, p_parameter.maximum);
            }
        }
        return info;
    }
    void load_labels(FMOD_STUDIO_EVENTDESCRIPTION *p_description)
    {
        if (is_labeled())
        {
            labels.resize(max_value + 1);
            for (int i = 0; i <= max_value; i++)
            {
                int retrieved = 0;
                FMOD_Studio_EventDescription_GetParameterLabelByID(p_description, cast_to_parameter_id(id), i, nullptr,
                                                                   0, &retrieved);
                CharString label;
                label.resize_uninitialized(retrieved);
                if (retrieved > 0)
                {
                    int size = retrieved;

                    FMOD_Studio_EventDescription_GetParameterLabelByID(p_description, cast_to_parameter_id(id), i,
                                                                       label.ptrw(), size, &retrieved);
                }
                labels[i] = label.ptr();
            }
        }
    }
    explicit operator PropertyInfo()
    {
        PropertyInfo info;
        info.name = "parameters/" + name;
        if (is_readonly())
        {
            info.usage = PropertyUsageFlags::PROPERTY_USAGE_READ_ONLY;
        }
        else
        {
            info.usage = PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;
        }

        // if (param.type != FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
        // {
        //     continue;
        // }
        if (is_labeled())
        {
            info.hint = PROPERTY_HINT_ENUM;
            info.type = Variant::INT;
            PackedStringArray labels2;
            labels2.resize(labels.size());
            for (int i = 0; i < labels.size(); i++)
            {
                labels2[i] = vformat("%s:%d", labels[i], i);
            }
            info.hint_string = String(",").join(labels2);
        }
        else
        {
            info.hint = PROPERTY_HINT_RANGE;
            info.type = Variant::FLOAT;
            if (is_discrete())
            {
                info.hint_string = vformat("%s,%s,1", min_value, max_value);
            }
            else
            {
                info.hint_string = vformat("%s,%s", min_value, max_value);
            }
        }
        return info;
    };
};
} // namespace FmodGodot
