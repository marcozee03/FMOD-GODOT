#pragma once
#include <classes/tree.hpp>
using namespace godot;

namespace FmodGodot
{
class FmodObjectTree : public Tree
{
    GDCLASS(FmodObjectTree, Tree);

  public:
    enum DisplayFlags : uint
    {
        FMOD_DISPLAY_BANKS = 1,
        FMOD_DISPLAY_EVENTS = 2,
        FMOD_DISPLAY_VCAS = 4,
        FMOD_DISPLAY_GLOBAL_PARAMETERS = 8,
        FMOD_DISPLAY_ALL = 0xffffffff
    };

  private:
    DisplayFlags display_flags;
    void on_item_activated();
    void on_item_selected();

  protected:
    static void _bind_methods();

  public:
    FmodObjectTree();
    ~FmodObjectTree();
    void set_display_flags(int p_flags);
    int get_display_flags() const;
    void LoadEvents();
    String get_item_path(TreeItem *p_item);
    Variant _get_drag_data(const Vector2 &p_vec2) override;
};

} // namespace FmodGodot
VARIANT_BITFIELD_CAST(FmodGodot::FmodObjectTree::DisplayFlags)
