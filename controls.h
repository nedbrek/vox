#ifndef CONTROLS_H
#define CONTROLS_H

class Camera;

/// handle user interaction with camera
class Controls
{
public:
	Controls();

	/// call at the beginning of frame to translate user commands to camera ori
	void beginFrame(Camera *cp);

	double fps() const { return fps_; }

protected:
	double lastTime_, lastInterval_;
	int windowWidth_, windowHeight_;
	unsigned framesSinceLastInterval_;
	double fps_;
};

#endif

