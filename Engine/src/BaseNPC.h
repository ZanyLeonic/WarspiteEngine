#pragma once
#ifndef __BaseNPC_H__
#define __BaseNPC_H__

#include "TileObject.h"
#include "DialogueTypes.h"

class CDialogueWindow;

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
    SDialogue* m_sLoadedDialogue;

    std::string m_sWorldTextureID;
    std::string m_sDialogueID;
    std::unique_ptr<CDialogueWindow> m_pDialogueWindow = 0;

    int GetRowFromDirection();
};

REG_OBJ_TO_REF(BaseNPC, CBaseNPC);
#endif