#pragma once
#ifndef __DOORLINK_H__
#define __DOORLINK_H__

#include <memory>
#include "TileObject.h"

class CDoorLink :
    public CTileObject
{
public:
    CDoorLink();

    void Load(const CObjectParams* pParams);
    void OnPlay();

    void OnOverlapStart();
    void OnOverlapEnd();

    void SetTravelledTo(bool travelled) { m_bTravelledTo = travelled; }
private:
    std::string m_targetDoorID;
    std::shared_ptr<CDoorLink> m_targetDoor;

    bool m_bTravelledTo = false;
};

REG_OBJ_TO_REF(DoorLink, CDoorLink);
#endif 