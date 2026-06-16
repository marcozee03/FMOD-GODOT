#include "classes/h_box_container.hpp"
#include "classes/label.hpp"
#include "classes/texture_rect.hpp"
#include "classes/wrapped.hpp"
using namespace godot;
namespace FmodGodot
{
#define get_editor_theme_icon(icon) get_theme_icon(icon, "EditorIcons")
class LiveUpdateIndicator : public HBoxContainer
{
    GDCLASS(LiveUpdateIndicator, HBoxContainer)
  private:
    TextureRect *icon;
    Label *label;
    void _update_theme();
    void _connected();
    void _disconnected();
    struct
    {
        Color disconnected_color;
        Color connected_color;
        Ref<Texture> icon;
    } theme_cache;

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    LiveUpdateIndicator();
};
} // namespace FmodGodot
