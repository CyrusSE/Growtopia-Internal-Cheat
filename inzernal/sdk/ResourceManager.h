#pragma once
#include <string>
#include <sdk/Surface.h>

GTClass ResourceManager {
   public:
    SurfaceAnim* GetSurfaceAnim(const std::string& texture_file) {
        static auto func = types::GetSurfaceAnim(sigs::get(sig::getsurfaceanim));
        return func(this, texture_file, false);
    }
};
