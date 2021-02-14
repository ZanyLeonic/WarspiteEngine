#pragma once
#ifndef __DOORLINK_H__
#define __DOORLINK_H__

#include <memory>
#include "TileObject.h"

class CDoorLink :
    public CTileObject
{
public:
    void Load(const CObjectParams* pParams);
    void OnPlay();
private:
    std::string m_targetDoorID;
    std::shared_ptr<CDoorLink> m_targetDoor;
};

REG_OBJ_TO_REF(DoorLink, CDoorLink);
#endif 