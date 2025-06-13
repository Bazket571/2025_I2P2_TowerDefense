#pragma once
#include "Engine/Group.hpp"

#include "allegro5/allegro.h"
#include <memory>
class Billboard : public Engine::Group{
    void GetModelMatrix(ALLEGRO_TRANSFORM* trans, Engine::Point Position) const;
    static void inverse(ALLEGRO_TRANSFORM* trans);
public:
    Billboard();
    void AddNewObject(IObject* obj);
    void AddNewControlObject(IObject* obj);
	void Draw() const override;
    static void ScreenToWorld(float & x, float & y, float &z);
    //Check ray p0-p1 intersection point with plane of height z
    static Engine::Point RayPlane(Engine::Point p0, Engine::Point p1, float z);

    static Engine::Point MousePlane(Engine::Point mouse, float z);

    //Do Transformations to the mouse position before passing them to the thing.
    /// <summary>
    /// Delegate the mouse key down event to all controls.
    /// </summary>
    /// <param name="button">The button pressed.</param>
    /// <param name="mx">Mouse x coordinate in window space.</param>
    /// <param name="my">Mouse y coordinate in window space.</param>
    void OnMouseDown(int button, int mx, int my) override;
    /// <summary>
    /// Delegate the mouse key up event to all controls.
    /// </summary>
    /// <param name="button">The button released.</param>
    /// <param name="mx">Mouse x coordinate in window space.</param>
    /// <param name="my">Mouse y coordinate in window space.</param>
    void OnMouseUp(int button, int mx, int my) override;
    /// <summary>
    /// Delegate the mouse move event to all controls.
    /// </summary>
    /// <param name="mx">Mouse x coordinate in window space.</param>
    /// <param name="my">Mouse y coordinate in window space.</param>
    void OnMouseMove(int mx, int my) override;
};