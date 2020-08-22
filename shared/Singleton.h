#pragma once
#ifndef __SINGLETON_H_
#define __SINGLETON_H_
class ISingleton
{
public:
    virtual void Instance()=0;
};

DLL_EXPORT ISingleton &GetSingleton();
#endif