#pragma once
#include <string>
#include <memory>

#include <allegro5/allegro_video.h>
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"

class Video : public Engine::IObject {
	std::shared_ptr<ALLEGRO_VIDEO> video;
	std::shared_ptr<ALLEGRO_EVENT_QUEUE> event_queue;
	mutable ALLEGRO_BITMAP* curFrame;
	mutable bool shouldDraw = false;
public:
	Video(std::string filename, Engine::Point pos, Engine::Point size);

	virtual void Update(float delta);
	virtual void Draw() const;
	void Play();
	void Stop();
};