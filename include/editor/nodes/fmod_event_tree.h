#pragma once
#ifdef TOOLS_ENABLED
#include <classes/tree.hpp>
using namespace godot;

namespace FmodGodot
{
class EventTree : public Tree
{
    GDCLASS(EventTree, Tree);

  public:
    enum DisplayFlags
    {
        BANKS = 1,
        EVENTS = 2,
        VCAS = 4,
        GLOBAL_PARAMETERS = 8
    };

  private:
    DisplayFlags display_flags;
    void on_item_activated();
    void on_item_selected();

  protected:
    static void _bind_methods();

  public:
    EventTree();
    ~EventTree();
    void set_display_flags(int p_flags);
    int get_display_flags() const;
    void LoadEvents();
    String get_item_path(TreeItem *item);
    Variant _get_drag_data(const Vector2 &p_vec2) override;
};

} // namespace FmodGodot
VARIANT_BITFIELD_CAST(FmodGodot::EventTree::DisplayFlags)
#endif
