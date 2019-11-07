#include "pch.h"
#include "Noclip.h"
#include "alt-log.h"

const double PI = 3.141592653589793238463;

Noclip::Noclip()
{
	enabled = false;
	speed = 1.0;
}

Vector3f GetCamDirection()
{
	uint32_t localPed = Native::Invoke<uint32_t, uint32_t>(N::GET_PLAYER_PED, 0);

	float pedHeading = Native::Invoke<float, uint32_t>(N::GET_ENTITY_HEADING, localPed);
	float camHeading = Native::Invoke<float>(N::GET_GAMEPLAY_CAM_RELATIVE_HEADING);
	float camPitch = Native::Invoke<float>(N::GET_GAMEPLAY_CAM_RELATIVE_PITCH);

	float heading = (camHeading + pedHeading);

	float x = -sin(heading * PI / 180.f);
	float y = cos(heading * PI / 180.f);
	float z = sin(camPitch * PI / 180.f);

	float length = sqrt(x * x + y * y + z * z);
	if (length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}

	return { x, y, z };
}

void Noclip::Tick()
{
	if (!enabled)
	{
		return;
	}

	uint32_t localPed = Native::Invoke<uint32_t, uint32_t>(N::GET_PLAYER_PED, 0);

	Vector3f curPosition = Native::Invoke<Vector3f, uint32_t, bool>(N::GET_ENTITY_COORDS, localPed, false);
	Vector3f camDirection = GetCamDirection();

	Native::Invoke<void, uint32_t, float, float, float>(N::SET_ENTITY_VELOCITY, localPed, 0.f, 0.f, 0.f);

	if (Native::Invoke<bool, uint32_t, uint32_t>(N::IS_CONTROL_PRESSED, 0, String::Hash("input_move_up_only")))
	{
		curPosition.x += speed * camDirection.x * (highSpeed ? 3 : 1);
		curPosition.y += speed * camDirection.y * (highSpeed ? 3 : 1);
		curPosition.z += speed * camDirection.z * (highSpeed ? 3 : 1);
	}

	if (Native::Invoke<bool, uint32_t, uint32_t>(N::IS_CONTROL_PRESSED, 0, String::Hash("input_move_down_only")))
	{
		curPosition.x -= speed * camDirection.x * (highSpeed ? 3 : 1);
		curPosition.y -= speed * camDirection.y * (highSpeed ? 3 : 1);
		curPosition.z -= speed * camDirection.z * (highSpeed ? 3 : 1);
	}

	Native::Invoke<void, uint32_t, float, float, float, bool, bool, bool, bool>(N::SET_ENTITY_COORDS_NO_OFFSET, localPed, curPosition.x, curPosition.y, curPosition.z, false, false, false, false);
}

void Noclip::Toggle()
{
	auto flag = !enabled;

	uint32_t localPed = Native::Invoke<uint32_t, uint32_t>(N::GET_PLAYER_PED, 0);
	if (!flag)
	{
		Native::Invoke<void, uint32_t, int>(N::SET_ENTITY_ALPHA, localPed, 255);
		Native::Invoke<void, uint32_t, bool>(N::SET_ENTITY_VISIBLE, localPed, true);
	}
	else
	{
		Native::Invoke<void, uint32_t, int>(N::SET_ENTITY_ALPHA, localPed, 0);
		Native::Invoke<void, uint32_t, bool>(N::SET_ENTITY_VISIBLE, localPed, false);
	}

	Log::Info("Noclip::Toggle ", enabled);

	enabled = flag;
}

void Noclip::SetHighSpeed(bool enable)
{
	highSpeed = enable;
}
