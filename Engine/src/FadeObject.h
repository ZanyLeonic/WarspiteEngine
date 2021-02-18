#pragma once
#ifndef __FadeObject_H__
#define __FadeObject_H__

#include <map>
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

    float GetTargetOpacity() const { return m_fTargetOpacity; }
    void SetTargetOpacity(float tar) { m_fTargetOpacity = tar; }

    float GetCurrentOpacity() const { return m_fCurrentOpacity; }
    bool IsFading() const { return m_bFading; }

    void AddCallback(std::string id, HFadeComplete callback);
    void RemoveCallback(std::string id);

private:
    void callCallbacks();

    CTexture* m_pFadeTexture = nullptr;

    float m_fCurrentOpacity = 0.f;
    float m_fTargetOpacity = 255.f;
    float m_fFadeSpeed = 6.f;
    
    EPlaybackDirection m_eDirection = EPlaybackDirection::NONE;

    bool m_bFading = false;

    std::map<std::string, HFadeComplete> m_callbacks;
};

#endif // #ifndef __FadeObject_H__