using FMOD;
using Godot;
using System;
namespace FmodGodot
{
    public static class FmodUtils
    {
        public static GUID ToGUID(this Variant variant)
        {
            if (Variant.Type.Vector4I == variant.VariantType)
            {
                var v4 = variant.As<Vector4I>();
                return new GUID
                {
                    Data1 = v4.X,
                    Data2 = v4.Y,
                    Data3 = v4.Z,
                    Data4 = v4.W,
                };
            }
            throw new InvalidCastException($"Cannot cast from {variant.VariantType} to GUID");
        }

        public static Variant ToVariant(this GUID id)
        {
            return new Vector4I(id.Data1, id.Data2, id.Data3, id.Data4);
        }
    }
}