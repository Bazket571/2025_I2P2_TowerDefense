#pragma once
#define ALLEGRO_UNSTABLE
#include "Engine/Group.hpp"
#include "Billboard.hpp"

#include <memory>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
class Group3D : public Engine::Group {
	bool renderShadow;

	static const std::string shadowVert, shadowFrag, mainVert, mainFrag;
	static ALLEGRO_TRANSFORM identity;


	std::shared_ptr<ALLEGRO_SHADER> shadowShader;
	std::shared_ptr<ALLEGRO_SHADER> mainShader;

	std::shared_ptr<ALLEGRO_BITMAP> depthbuffer;
	std::shared_ptr<ALLEGRO_BITMAP> render;

	Billboard* billboards;
	bool attachAndBuildShader(ALLEGRO_SHADER* shader, std::string vertSource, std::string fragSource);

	
public:
	static ALLEGRO_TRANSFORM perspective_transform(float width, float height);
	static ALLEGRO_TRANSFORM orthographic_transform(float width, float height);
	static ALLEGRO_TRANSFORM light_view();
	static ALLEGRO_TRANSFORM camera_view();
	Group3D();
	Group3D(bool renderShadow);
	void Draw() const override;
	void Update(float delta);
	void AddNewControlBillboard(IObject* obj);
	void AddNewBillboard(IObject* obj);
	Engine::Group* GetBillboards();
};