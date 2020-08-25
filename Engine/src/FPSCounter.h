#pragma once
#ifndef __FPSCOUNTER_H__
#define __FPSCOUNTER_H__
#include "WarspiteObject.h"

class CTexture;

class CFPSCounter :
	public CWarspiteObject
{
public:
	CFPSCounter();

	void Load(const CObjectParams* pParams) override;
	void Draw() override;

	void SetFPSValue(float fps);
private:
	float m_fps = 0.f;
	float m_lfps = -1.f;
};

#endif