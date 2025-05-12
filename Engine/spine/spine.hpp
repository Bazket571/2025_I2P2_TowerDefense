#pragma once
#include <vector>
#include <list>
#include <string>
#include <memory>

#include <spine/spine.h>
#include <Engine/IObject.hpp>
#include <Engine/IControl.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

namespace Engine {
    //Rough sketch class from documentation
    class SpineSprite : public Engine::IObject {
        static const std::string spinePathPrefix;
        mutable spine::SkeletonClipping clipper;
        mutable bool usePremultipliedAlpha;
        mutable bool ownsAnimationStateData;
        mutable spine::Vector<float> worldVertices;
        mutable spine::Vector<spine::Color> tempColors;
        mutable spine::Vector<uint16_t> quadIndices;

    public:
        spine::Skeleton* skeleton;
        spine::AnimationState* state;
        spine::VertexEffect* vertexEffect;
        mutable std::vector<ALLEGRO_VERTEX> vertexArray;
        float timeScale;

        SpineSprite(std::string skeletonFile, std::string atlasFile,
            float x, float y, float scale = 1, float anchorX = 0, float anchorY = 0,
            std::string pathPrefix = spinePathPrefix);
        ~SpineSprite();

        virtual void Update(float deltaTime);
        virtual void Draw() const;

        void setUsePremultipliedAlpha(bool usePMA) { usePremultipliedAlpha = usePMA; };
        bool getUsePremultipliedAlpha() const { return usePremultipliedAlpha; };
    };

    class SpineTextureLoader : public spine::TextureLoader {
        public:
            spine::String filePath;
            virtual void load(spine::AtlasPage& page, const spine::String& path);
            virtual void unload(void* texture);
    };
};

