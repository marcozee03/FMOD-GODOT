#pragma once
#include "classes/wrapped.hpp"
#include "variant/typed_array.hpp"
#include "variant/vector4i.hpp"
#include <classes/audio_stream.hpp>
#include <fmod_studio.h>
using namespace godot;
class FmodEventAudioStream : public AudioStream
{
    GDCLASS(FmodEventAudioStream, AudioStream)
  private:
    friend class FmodEventAudioStreamPlayback;
    Vector4i event_id;
    FMOD_STUDIO_EVENTDESCRIPTION *description;

  protected:
    static void _bind_methods();

  public:
    FmodEventAudioStream();
    ~FmodEventAudioStream();
    int _get_bar_beats() const override;
    int _get_beat_count() const override;
    double _get_bpm() const override;
    double _get_length() const override;
    TypedArray<Dictionary> _get_parameter_list() const override;
    String _get_stream_name() const override;
    bool _has_loop() const override;
    Ref<AudioStreamPlayback> _instantiate_playback() const override;
    bool _is_monophonic() const override;
    Vector4i get_event_id() const;
    void set_event_id(Vector4i p_id);
};
