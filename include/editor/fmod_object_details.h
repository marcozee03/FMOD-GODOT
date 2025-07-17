#include <godot_cpp/classes/tree.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodObjectDetails : public Tree
    {
    private:
        /* data */
    public:
        FmodObjectDetails(/* args */);
        ~FmodObjectDetails();

        void DisplayFmodObject();
    };

    FmodObjectDetails::FmodObjectDetails(/* args */)
    {
    }

    FmodObjectDetails::~FmodObjectDetails()
    {
    }

}