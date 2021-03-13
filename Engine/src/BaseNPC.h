#pragma once
#ifndef __BaseNPC_H__
#define __BaseNPC_H__

#include "TileObject.h"

class CBaseNPC :
    public CTileObject
{
public:
    CBaseNPC();

    void Load(CObjectParams* pParams) override;

    void OnPlay() override;
    void Destroy() override;

    bool OnThink() override;
    void Draw() override;

    bool InteractAction(IGameObject* pOther) override;
private:
    std::string m_sWorldTextureID;

    int GetRowFromDirection();
};

REG_OBJ_TO_REF(BaseNPC, CBaseNPC);
#endif