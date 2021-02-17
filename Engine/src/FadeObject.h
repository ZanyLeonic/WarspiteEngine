#pragma once
#ifndef __FadeObject_H__
#define __FadeObject_H__

#include "WarspiteObject.h"
#include "CTexture.h"

class CFadeObject :
    public CWarspiteObject
{
public:
    CFadeObject();
    bool OnThink() override;

    void FadeIn();
    void FadeOut();

    void SetStartOpacity(float start) { m_fStartOpacity = start; }
    float GetStartOpacity() const { return m_fStartOpacity; }

    void SetTargetOpacity(float tar) { m_fTargetOpacity = tar; }
    float GetTargetOpacity() const { return m_fTargetOpacity; }

    float GetCurrentFadeTime() const { return m_fCurrentTime; }

private:
    CTexture* m_pFadeTexture = nullptr;

    float m_fStartOpacity = 0.f;
    float m_fTargetOpacity = 1.f;
    float m_fCurrentTime = 0.f;
    float m_fFadeTimeLeft = 0.f;

    bool m_bFading = false;
};

#endif // #ifndef __FadeObject_H__