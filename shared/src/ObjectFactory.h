#pragma once
#ifndef __OBJECTFACTORY_H__
#define __OBJECTFACTORY_H__
#include <cstddef>

// Overridable base stuff
template<class T>
class IObjectFactory
{
public:
	virtual T* Create() = 0;
	virtual size_t GetObjectSize() = 0;
};
#endif // ifndef __OBJECTFACTORY_H__