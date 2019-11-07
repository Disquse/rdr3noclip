#pragma once

class Noclip
{
public:
    Noclip();
    void Tick();
    void Toggle();
	void SetHighSpeed(bool enable);
private:
	bool highSpeed = false;
    bool enabled;
    float speed;
};
