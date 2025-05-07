#include "Engine/Settings.hpp"

namespace Engine{
    void Settings::ToggleBoundingBox(){
        this->DrawBoundingBox = !this->DrawBoundingBox;
    }
}