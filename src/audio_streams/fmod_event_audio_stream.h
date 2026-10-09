#pragma once

#include "classes/audio_stream.hpp"
#include "classes/wrapped.hpp"
#include "fmod_studio_common.h"
#include "variant/dictionary.hpp"
#include "variant/typed_array.hpp"
#include "variant/vector4i.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodEventAudioStream : public AudioStream
{
    friend class FmodAudioStreamPlayback;
    GDCLASS(FmodEventAudioStream, AudioStream)
  private:
    Vector4i event_guid;
    FMOD_STUDIO_EVENTDESCRIPTION *_get_event_description() const;

  protected:
    static void _bind_methods();

  public:
    void set_event_guid(Vector4i p_event_guid);
    Vector4i get_event_guid() const;

    virtual Ref<AudioStreamPlayback> _instantiate_playback() const override;

    virtual double _get_length() const override;
    // virtual String _get_stream_name() const override;
    virtual bool _is_monophonic() const override
    {
        return true;
    }
    // virtual double _get_bpm() const override;
    // virtual int32_t _get_beat_count() const override;
    // virtual Dictionary _get_tags() const override;
    // virtual bool _has_loop() const override;
    // virtual int32_t _get_bar_beats() const override;
    virtual TypedArray<Dictionary> _get_parameter_list() const override;
};
} // namespace FmodGodot
