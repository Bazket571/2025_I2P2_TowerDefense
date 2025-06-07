#define ALLEGRO_UNSTABLE

#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Engine/IObject.hpp"

#include <vector>
#include <memory>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_opengl.h>

#include "tiny_gltf.h"

class Object3D : public Engine::IObject {
	template <typename T>
	std::vector<T> getFromAccessor(int accessorID) const;
public:
	tinygltf::Model model;
	std::vector<ALLEGRO_VERTEX> vertices;
	std::vector<int> indices;
	std::shared_ptr<ALLEGRO_BITMAP> texture;
	std::shared_ptr<ALLEGRO_BITMAP> depthbuffer;
	std::shared_ptr<ALLEGRO_BITMAP> render;
	std::shared_ptr<ALLEGRO_SHADER> shadowShader;

	Object3D(std::string gltfFile, int x, int y, float scaleX, float scaleY, int anchorX = 0, int anchorY = 0);
	void Draw() const;
	virtual void Update(float delta);
};