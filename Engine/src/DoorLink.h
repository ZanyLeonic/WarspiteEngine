#pragma once
#ifndef __DOORLINK_H__
#define __DOORLINK_H__

#include <memory>
#include "TileObject.h"
#include "ISoundManager.h"

class CPlayer;

class CDoorLink :
    public CTileObject
{
public:
    CDoorLink();

    void Load(CObjectParams* pParams) override;

    void OnPlay() override;
    void Destroy() override;

    bool OnThink() override;
    void Draw() override;

    void OnOverlapStart() override;
    void OnOverlapEnd() override;

    void SetTravelledTo(bool travelled) { m_bTravelledTo = travelled; }

    void PlayDoorAnimation(EPlaybackDirection direction);

    bool InteractAction(IGameObject* pOther);
private:
    std::string m_sTargetDoorID;
    std::string m_sWorldTextureID;
    std::shared_ptr<CDoorLink> m_pTargetDoor;

    int m_iAnimationFrame = 0;
    bool m_bDoDoorAnimation = false;
    EPlaybackDirection m_eAnimationPlayback = EPlaybackDirection::NONE;

    EDirection m_eExitDirection = EDirection::NONE;

    SWaveFile m_prWaveFile;
    void fadeCallback(EPlaybackDirection direction, bool fadeFinished);
    void movementEnd(CPlayer* pPlayer);

    void playDoorAnimation(EPlaybackDirection direction);

    bool m_bTravelledTo = false;
    bool m_bTravellingFrom = false; // Flag whether we are travelling from this door
    bool m_bMadePlayerMove = false;
};

REG_OBJ_TO_REF(DoorLink, CDoorLink);
#endif 