#pragma once
#define ALLEGRO_UNSTABLE
#include "Engine/Group.hpp"
#include <memory>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
class Group3D : public Engine::Group {
	bool renderShadow;

	static const std::string shadowVert, shadowFrag, mainVert, mainFrag;

	static std::shared_ptr<ALLEGRO_SHADER> shadowShader;
	static std::shared_ptr<ALLEGRO_SHADER> mainShader;

	std::shared_ptr<ALLEGRO_BITMAP> depthbuffer;
	std::shared_ptr<ALLEGRO_BITMAP> render;

	bool attachAndBuildShader(ALLEGRO_SHADER* shader, std::string vertSource, std::string fragSource);
	
public:
	static ALLEGRO_TRANSFORM perspective_transform(float width, float height);
	static ALLEGRO_TRANSFORM orthographic_transform(float width, float height);
	static ALLEGRO_TRANSFORM light_view();
	static ALLEGRO_TRANSFORM camera_view();
	Group3D();
	Group3D(bool renderShadow);
	void Draw() const override;
};