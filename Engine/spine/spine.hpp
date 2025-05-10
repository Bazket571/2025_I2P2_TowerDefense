#pragma once
#include <spine/spine.h>
#include <Engine/IObject.hpp>

namespace spine {
    class SkeletonDrawable : public Engine::IObject{
        bool ownsAnimationStateData;
		mutable bool usePremultipliedAlpha;
    public:
        Skeleton *skeleton;
        AnimationState *state;
        float timeScale;

        SkeletonDrawable(SkeletonData *skeleton, AnimationStateData *stateData = 0);
        ~SkeletonDrawable();

        void update(float deltaTime, Physics physics = Physics_Update);
        virtual void Draw() const;

		void setUsePremultipliedAlpha(bool usePMA) { usePremultipliedAlpha = usePMA; };
		bool getUsePremultipliedAlpha() { return usePremultipliedAlpha; };
    };
}

namespace Engine{
    class TextureLoader : public spine::TextureLoader{
        public:
		virtual void load(spine::AtlasPage &page, const spine::String &path);
		virtual void unload(void *texture);
		spine::String toString() const;
    };
}