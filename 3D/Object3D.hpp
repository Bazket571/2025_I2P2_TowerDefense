#pragma once
#define ALLEGRO_UNSTABLE

#include "Engine/IObject.hpp"

#include <memory>
#include <string>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

class Object3D : public Engine::IObject {
	
public:
	struct Vertex {
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		ALLEGRO_COLOR color;
	};

	std::shared_ptr<ALLEGRO_VERTEX_BUFFER> vertexBuffer;
	std::shared_ptr<ALLEGRO_INDEX_BUFFER> indicesBuffer;
	std::shared_ptr<ALLEGRO_BITMAP> texture;

	Object3D(std::string gltfFile, Engine::Point pos, Engine::Point scale, int anchorX = 0, int anchorY = 0);
	void Draw() const;
	virtual void Update(float delta);
};