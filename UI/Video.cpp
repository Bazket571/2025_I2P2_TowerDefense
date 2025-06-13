#include "Video.hpp"

Video::Video(std::string filename, Engine::Point pos, Engine::Point size):
	IObject(pos.x, pos.y, size.x, size.y)
{
	video = std::shared_ptr<ALLEGRO_VIDEO>(al_open_video(filename.c_str()), al_close_video);
	event_queue = std::shared_ptr<ALLEGRO_EVENT_QUEUE>(al_create_event_queue(), al_destroy_event_queue);
	al_register_event_source(event_queue.get(), al_get_video_event_source(video.get()));
	al_start_video(video.get(), al_get_default_mixer());
	Play();
}



void Video::Update(float delta)
{
	ALLEGRO_EVENT event;
	if (al_get_next_event(event_queue.get(), &event)) {
		if (event.type == ALLEGRO_EVENT_VIDEO_FRAME_SHOW) {
			shouldDraw = true;
		}
	}
}

void Video::Draw() const
{
	if (shouldDraw) {
		curFrame = al_get_video_frame(video.get());
	}
	int width = al_get_bitmap_width(curFrame);
	int height = al_get_bitmap_height(curFrame);
	al_draw_scaled_bitmap(curFrame, 0, 0, width, height, Position.x, Position.y, Size.x, Size.y, 0);
	shouldDraw = false;
}

void Video::Play()
{
	al_set_video_playing(video.get(), true);
}

void Video::Stop()
{
	al_set_video_playing(video.get(), false);
}
