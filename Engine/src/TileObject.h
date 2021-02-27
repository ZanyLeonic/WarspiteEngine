#pragma once
#ifndef __TILEENTITY_H__
#define __TILEENTITY_H__

#include "MapObject.h"

struct STileset;

class CTileObject :
    public CMapObject
{
public:
    virtual void Load(CObjectParams* pParams);
    virtual void Draw();

private:
    int m_gid = -1;
    STileset* m_tileset = 0;
};

#ifdef _ENGINE_
REG_OBJ_TO_REF(Bound, CTileObject)
#endif

#endif // __TILEENTITY_H__