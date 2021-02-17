#pragma once
#ifndef __FadeObject_H__
#define __FadeObject_H__

#include "WarspiteObject.h"
#include "CTexture.h"
#include "EngineTypes.h"

class CFadeObject :
    public CWarspiteObject
{
public:
    CFadeObject();

    void Draw() override;
    bool OnThink() override;

    void FadeIn();
    void FadeOut();

    float GetCurrentFadeSpeed() const { return m_fFadeSpeed; }
    void SetFadeSpeed(float speed) { m_fFadeSpeed = speed; }

    float GetCurrentOpacity() const { return m_fCurrentOpacity; }
    bool IsFading() const { return m_bFading; }

private:
    CTexture* m_pFadeTexture = nullptr;

    float m_fCurrentOpacity = 0.f;
    float m_fFadeSpeed = 1.f;

    EPlaybackDirection m_eDirection = EPlaybackDirection::NONE;

    bool m_bFading = false;
};

#endif // #ifndef __FadeObject_H__