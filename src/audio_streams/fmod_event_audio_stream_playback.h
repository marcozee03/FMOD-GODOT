#pragma once
#include "classes/audio_stream_playback.hpp"
#include "classes/wrapped.hpp"
#include "fmod_studio_common.h"
using namespace godot;
namespace FmodGodot
{
class FmodEventAudioStreamPlayback : public AudioStreamPlayback
{
    GDCLASS(FmodEventAudioStreamPlayback, AudioStreamPlayback);

  private:
    FMOD_STUDIO_EVENTINSTANCE *instance;

  protected:
    static void _bind_methods();

  public:
    FmodEventAudioStreamPlayback();
    FmodEventAudioStreamPlayback(Vector4i p_guid);
    ~FmodEventAudioStreamPlayback();
    virtual void _start(double p_from_pos) override;
    virtual void _stop() override;
    virtual bool _is_playing() const override;
    // virtual int32_t _get_loop_count() const override;
    virtual double _get_playback_position() const override;
    virtual void _seek(double p_position) override;
    // virtual int32_t _mix(AudioFrame *p_buffer, float p_rate_scale, int32_t p_frames);
    // virtual void _tag_used_streams() override;
    virtual void _set_parameter(const StringName &p_name, const Variant &p_value) override;
    virtual Variant _get_parameter(const StringName &p_name) const override;
};
} // namespace FmodGodot
