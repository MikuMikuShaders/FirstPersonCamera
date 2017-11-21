#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera()
	: _isMouseLocked(false)
{
	_mmd = mmp::getMMDMainData();
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void
FirstPersonCamera::lockMouse(bool lock)
{
	if (_isMouseLocked != lock)
	{
		if (!lock)
			::ShowCursor(TRUE);
		else
		{
			POINT pt;
			::GetCursorPos(&pt);

			_lockMouseX = pt.x;
			_lockMouseY = pt.y;

			::ShowCursor(FALSE);
		}

		_isMouseLocked = lock;
	}
}

void
FirstPersonCamera::moveCamera(float speed) noexcept
{
	D3DXMATRIX rotate;
	D3DXMatrixRotationYawPitchRoll(&rotate, -_mmd->rxyz.y, -_mmd->rxyz.x, _mmd->rxyz.z);

	D3DXVECTOR4 forward;
	D3DXVec3Transform(&forward, &D3DXVECTOR3(0, 0, -1), &rotate);

	_mmd->xyz.x += forward.x * speed;
	_mmd->xyz.y += forward.y * speed;
	_mmd->xyz.z += forward.z * speed;
}

void
FirstPersonCamera::yawCamera(float speed) noexcept
{
	D3DXMATRIX rotate;
	D3DXMatrixRotationYawPitchRoll(&rotate, -_mmd->rxyz.y, -_mmd->rxyz.x, _mmd->rxyz.z);

	D3DXVECTOR4 right;
	D3DXVec3Transform(&right, &D3DXVECTOR3(1, 0, 0), &rotate);

	_mmd->xyz.x += right.x * speed;
	_mmd->xyz.y += right.y * speed;
	_mmd->xyz.z += right.z * speed;
}

void
FirstPersonCamera::rotateCamera(int x, int y)
{
	if (!_isMouseLocked)
		return;

	float angleX = (x - _lockMouseX) / 500.0f;
	float angleY = (y - _lockMouseY) / 500.0f;

	_lockMouseX = x;
	_lockMouseY = y;

	D3DXMATRIX rotate;
	D3DXMatrixRotationYawPitchRoll(&rotate, -_mmd->rxyz.y, -_mmd->rxyz.x, _mmd->rxyz.z);

	D3DXVECTOR4 foward;
	D3DXVec3Transform(&foward, &D3DXVECTOR3(0, 0, 1), &rotate);

	_mmd->rxyz.y -= angleX;
	_mmd->rxyz.x -= angleY;

	_mmd->xyz.x += foward.x * _mmd->length;
	_mmd->xyz.y += foward.y * _mmd->length;
	_mmd->xyz.z += foward.z * _mmd->length;

	_mmd->length = 0;
}

void
FirstPersonCamera::KeyBoardProc(WPARAM wParam, LPARAM lParam)
{
	if (_mmd->is_camera_mode)
	{
		float speed = GetKeyState(VK_SHIFT) > 0 ? 1.0f : 4.0f;

		if (wParam != VK_PROCESSKEY)
		{
			if (wParam == 'W')
				moveCamera(-speed);

			if (wParam == 'S')
				moveCamera(speed);

			if (wParam == 'A')
				yawCamera(-speed);

			if (wParam == 'D')
				yawCamera(speed);
		}
	}
}

void
FirstPersonCamera::MouseProc(WPARAM wParam, const MOUSEHOOKSTRUCT* lParam)
{
	if (_mmd->is_camera_mode)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		{
			this->lockMouse(true);
			break;
		}
		case WM_LBUTTONUP:
		{
			this->lockMouse(false);
			break;
		}
		case WM_MOUSEMOVE:
		{
			PMOUSEHOOKSTRUCT mouse = (PMOUSEHOOKSTRUCT)lParam;
			this->rotateCamera(mouse->pt.x, mouse->pt.y);
			break;
		}
		}
	}
}

int
version()
{
	return 4;
}

MMDPluginDLL4*
create4(IDirect3DDevice9* device)
{
	return new FirstPersonCamera();
}

void
destroy4(MMDPluginDLL4* p)
{
	delete p;
}