#pragma once
#ifndef __ICAMERA_H__
#define __ICAMERA_H__

#include <memory>

class CVector2D;

class ICamera
{
public:
	virtual void OnThink() = 0;

	virtual void SetTarget(CVector2D* target) = 0;
	virtual void SetPosition(const CVector2D& position) = 0;
	virtual void SetLevelSize(const CVector2D& sz) = 0;
	
	virtual CVector2D GetLevelSize() const = 0;
	virtual CVector2D GetPosition() const = 0;
	virtual CVector2D GetPositionT() = 0;
	virtual CVector2D* GetTarget() const = 0;
};
#endif
