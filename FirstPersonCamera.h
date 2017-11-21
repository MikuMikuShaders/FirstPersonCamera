#define MMDPLUGIN_D3DX9_HOOK
#include "mmdplugin/mmd_plugin.h"

class FirstPersonCamera : public MMDPluginDLL4
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	virtual void KeyBoardProc(WPARAM wParam, LPARAM lParam);
	virtual void MouseProc(WPARAM wParam, const MOUSEHOOKSTRUCT* param);

	void lockMouse(bool lock);

	void moveCamera(float speed) noexcept;
	void yawCamera(float speed) noexcept;
	void rotateCamera(int x, int y);

private:
	const char* getPluginTitle() const { return "FirstPersonCamera"; }

private:
	bool _isMouseLocked;

	int _lockMouseX;
	int _lockMouseY;

	mmp::MMDMainData* _mmd;
};