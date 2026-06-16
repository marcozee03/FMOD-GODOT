#pragma once
#ifdef TOOLS_ENABLED
#include "classes/text_edit.hpp"
#include "classes/wrapped.hpp"
#include <classes/input_event.hpp>
#include "templates/local_vector.hpp"
namespace FmodGodot
{
using namespace godot;
class CommandInput : public TextEdit
{
    GDCLASS(CommandInput, TextEdit)
    LocalVector<String> history;

  private:
    int history_index = -1;
    static const int history_length = 1000;

  protected:
    static void _bind_methods();

  public:
    CommandInput();
    void history_push(const String &command);
    void update_height();
    void _gui_input(const Ref<InputEvent> &event) override;
};
} // namespace FmodGodot
#endif
