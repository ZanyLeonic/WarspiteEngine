#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__

class Layer
{
public:

	virtual void OnThink() = 0;
	virtual void Draw() = 0;

protected:
	virtual ~Layer() {}

};
#endif
