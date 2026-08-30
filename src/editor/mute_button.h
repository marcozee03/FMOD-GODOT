#pragma once
#include "classes/button.hpp"
#include "classes/wrapped.hpp"
using namespace godot;
namespace FmodGodot
{
class MuteButton : public Button
{
    GDCLASS(MuteButton, Button);

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    MuteButton();

    void _toggled(bool p_toggled_on) override;
};
} // namespace FmodGodot
