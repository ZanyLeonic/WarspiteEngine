#include "DialogueWindow.h"
#include "FontManager.h"
#include "Game.h"
#include "TextureManager.h"

CDialogueWindow::CDialogueWindow()
    : CUIBase()
{
    m_sText = "This is a long piece of text whew, what will it look like in the box?\nKeep it going I feel like this will soon be enough, but will it? I don't know, but I hope it   wrap correctly if there is too much text for one line.";
}

void CDialogueWindow::Load(CObjectParams* pParams)
{
    CUIBase::Load(pParams);

    pScreenSize = &CBaseGame::Instance()->GetViewportSize();

    m_sTextureID = "_diagBG";
    m_iHeight = int(pScreenSize->GetX() * 0.25);
    m_iWidth = pScreenSize->GetX();

    m_vPosition = CVector2D(0, pScreenSize->GetY() - m_iHeight);

    pPlayer = CBaseGame::Instance()->GetPlayer();

    CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto", CEngineFileSystem::EPathType::FONTS), "Regular", m_iTextSize);

    m_bStartedTyping = true;
}

void CDialogueWindow::Draw()
{
    CUIBase::Draw();

    if (m_bStartedTyping)
    {
        CTextureManager::Instance()->DrawFrame(m_sFontID, m_iTextPadding + (int)m_vPosition.GetX(),
            m_iTextPadding + (int)m_vPosition.GetY(), m_iTextWidth, m_iTextHeight,
            m_iCurrentRow, m_iCurrentFrame, CBaseGame::Instance()->GetRenderer());
    }
}

bool CDialogueWindow::OnThink()
{
    //if (pPlayer)
    //{
    //    if (pPlayer->GetViewportPosition().GetY() > (pScreenSize->GetX() / 2))
    //    {
    //        m_vPosition.SetY(0);
    //    }
    //    else
    //    {
    //        m_vPosition.SetY(pScreenSize->GetY() - m_iHeight);
    //    }
    //}

    if (!m_bFinishedTyping && m_bStartedTyping)
    {
        if (m_sTypedText.length() != m_sText.length())
        {
            m_sTypedText += m_sText[m_iCurrentCharacter];

            CFontManager::Instance()->RenderText(m_sTypedText, "Roboto-Regular-16", "_DiagTest", CFontManager::EFontRenderType::BLENDED_WRAPPED, { 255, 255, 255 }, { 255, 255, 255 }, m_iWidth - m_iTextPadding*2);

            m_iTextWidth = CTextureManager::Instance()->m_textureMap[m_sFontID]->GetWidth();
            m_iTextHeight = CTextureManager::Instance()->m_textureMap[m_sFontID]->GetHeight();

            m_iCurrentCharacter++;

            SDL_Delay(5);
        }
    }
    return true;
}

void CDialogueWindow::Destroy()
{
}
