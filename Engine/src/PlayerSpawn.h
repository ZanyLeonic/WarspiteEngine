#pragma once
#ifndef __PLAYERSPAWN_H__
#define __PLAYERSPAWN_H__

#include "TileObject.h"
class CPlayerSpawn :
    public CTileObject
{
public:
    CPlayerSpawn();
    void Load(const CObjectParams* pParams) override;

    void OnPlay() override;
private:
    bool bDrawObject = false;
    CObjectParams* m_playerSpawnParam = nullptr;  // TODO: make a unique object to store player specific spawn parameters
};

REG_OBJ_TO_REF(PlayerSpawn, CPlayerSpawn);
#endif