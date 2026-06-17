#pragma once
#include "variant/packed_string_array.hpp"
#ifdef TOOLS_ENABLED
#include "classes/text_edit.hpp"
#include "classes/wrapped.hpp"
#include "templates/local_vector.hpp"
#include <classes/input_event.hpp>
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
    void _enter_tree() override;
    void _gui_input(const Ref<InputEvent> &event) override;
    PackedStringArray get_history() const;
    void load_history(const PackedStringArray &arr);
};
} // namespace FmodGodot
#endif
