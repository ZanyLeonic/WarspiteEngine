#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__

class Layer
{
public:

	virtual void OnPlay() = 0;
	virtual void Destroy() = 0;

	virtual void OnThink() = 0;
	virtual void Draw() = 0;

protected:
	virtual ~Layer() {}

};
#endif
