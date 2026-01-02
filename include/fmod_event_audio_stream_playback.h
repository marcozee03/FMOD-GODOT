#pragma once
#include "classes/audio_stream_playback.hpp"
#include "classes/wrapped.hpp"
#include "variant/string_name.hpp"
#include <fmod_studio.h>
using namespace godot;
class FmodEventAudioStreamPlayback : public AudioStreamPlayback
{
    GDCLASS(FmodEventAudioStreamPlayback, AudioStreamPlayback)
  private:
    FMOD_STUDIO_EVENTINSTANCE *event;
    protected:
    static void _bind_methods();
  public:
    FmodEventAudioStreamPlayback(FMOD_STUDIO_EVENTDESCRIPTION* description);
    FmodEventAudioStreamPlayback() = default;
    ~FmodEventAudioStreamPlayback();
    int _get_loop_count() const override;
    Variant _get_parameter(const StringName &p_name) const override;
    double _get_playback_position() const override;
    bool _is_playing() const override;
    int _mix(AudioFrame *buffer, float rate_scale, int frames) override;
    void _seek(float position);
    void _set_parameter(const StringName &p_name, const Variant &p_value) override;
    void _start(double from_pos) override;
    void _stop() override;
    // void _tag_used_streams() override;
};
