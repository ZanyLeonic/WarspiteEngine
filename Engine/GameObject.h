#pragma once
#ifndef __GameObject__
#define __GameObject__
class CObjectParams;
class CVector2D;

class IGameObject
{
public:
	virtual void OnPlay() = 0;
	virtual void Draw() = 0;
	virtual bool OnThink() = 0;
	virtual void Destroy() = 0;

	virtual void Load(const CObjectParams* pParams) = 0;

	virtual CVector2D GetPosition() = 0;
	virtual void SetPosition(CVector2D& nPos) = 0;

	virtual const char* GetName() = 0;
	virtual bool ShouldCollide() = 0;
protected:
	IGameObject() {} 
	virtual ~IGameObject() {}
};
#endif /* defined(__GameObject__) */