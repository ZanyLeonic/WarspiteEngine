#pragma once
#ifndef __TRIGGER_H__
#define __TRIGGER_H__

#include "EngineTypes.h"
#include "TileObject.h"

class CTrigger :
    public CTileObject
{
public:
    CTrigger();

    void Load(const CObjectParams* pParams) override;

    void OnPlay() override;
    void Destroy() override;

    bool OnThink() override;
    void Draw() override;

    void OnOverlapStart() override;
    void OnOverlapEnd() override;

private:
    HGenericCallback m_hOverlapStartCallback;
    HGenericCallback m_hOverlapEndCallback;

    std::string m_sStartOverlap;
    std::string m_sEndOverlap;

    bool m_bFireOnce;
};

REG_OBJ_TO_REF(Trigger, CTrigger);
#endif 