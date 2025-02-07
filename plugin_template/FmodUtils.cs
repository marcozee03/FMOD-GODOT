using FMOD;
using FmodGodot;
using Godot;
using System;
namespace FMOD
{
    public partial struct GUID
    {
        public static implicit operator GUID(Vector4I v4)
        {
            return new GUID
            {
                Data1 = v4.X,
                Data2 = v4.Y,
                Data3 = v4.Z,
                Data4 = v4.W,
            };
        }
        public static implicit operator Vector4I(GUID id)
        {
            return new Vector4I(id.Data1, id.Data2, id.Data3, id.Data4);
        }
    }
}