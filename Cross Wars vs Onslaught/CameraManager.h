#pragma once
#include "Camera.h"

class CameraManager
{
public:
	static Camera* GetCamera() { return pMainCamera; }
	static void SetCamera(Camera* _pCamera) { pMainCamera = _pCamera; }

private:
	static Camera* pMainCamera;
};

