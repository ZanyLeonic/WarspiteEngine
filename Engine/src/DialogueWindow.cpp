#include "DialogueWindow.h"
#include "FontManager.h"
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"

CDialogueWindow::CDialogueWindow()
    : CUIBase()
{

}

void CDialogueWindow::Load(CObjectParams* pParams)
{
    CUIBase::Load(pParams);

    // Background setup
    m_pScreenSize = CBaseGame::Instance()->GetViewportSize();
    m_sTextureID = "_diagBG";
    m_iHeight = int(m_pScreenSize->GetX() * 0.25);
    m_iWidth = int(m_pScreenSize->GetX());
    m_vPosition = CVector2D(0, m_pScreenSize->GetY() - m_iHeight);
    m_pPlayer = CBaseGame::Instance()->GetPlayer();

    // Font used by typing
    CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto", CEngineFileSystem::EPathType::FONTS), "Regular", m_iTextSize);

    // Advance text bind
    m_hAdvanceText = std::bind(&CDialogueWindow::AdvanceBind, this, std::placeholders::_1);
    CInputHandler::Instance()->AddOnMouseDown(m_hAdvanceText);

    // Advance text icon 
    if (CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath("advtext.png", CEngineFileSystem::EPathType::TEXTURE),
        "_diagAdv", CBaseGame::Instance()->GetRenderer(), 0xFF)) // Opacity 75%
    {
        m_pAdvanceTextIcon = CTextureManager::Instance()->m_textureMap["_diagAdv"];

        m_vAdvLoc.SetX((m_vPosition.GetX() + m_iWidth) - (m_iTextPadding + m_pAdvanceTextIcon->GetWidth()));
        m_vAdvLoc.SetY((m_vPosition.GetY() + m_iHeight) - (m_iTextPadding + m_pAdvanceTextIcon->GetHeight()));

        m_vAdvLocCenter.SetX(m_pAdvanceTextIcon->GetWidth() / 2);
        m_vAdvLocCenter.SetY(m_pAdvanceTextIcon->GetHeight() / 2);
    }
}

void CDialogueWindow::SetDialogue(SDialogue* pDiag)
{
    m_sDialogue = pDiag;
    m_sText = m_sDialogue->Nodes;

    m_bStartedTyping = true;
}

void CDialogueWindow::Draw()
{
    CUIBase::Draw();

    if (m_bStartedTyping && m_bDrawObject)
    {
        CTextureManager::Instance()->DrawFrame(m_sFontID, m_iTextPadding + (int)m_vPosition.GetX(),
            m_iTextPadding + (int)m_vPosition.GetY(), m_iTextWidth, m_iTextHeight,
            m_iCurrentRow, m_iCurrentFrame, CBaseGame::Instance()->GetRenderer());
    
        if (m_bFinishedTyping && !m_bReachedEnd && m_pAdvanceTextIcon != nullptr)
        {
            CTextureManager::Instance()->DrawFrame(m_pAdvanceTextIcon, int(m_vAdvLoc.GetX()), int(m_vAdvLoc.GetY()), 1, 0, 
                CBaseGame::Instance()->GetRenderer(),
                m_dAdvLocRot, &m_vAdvLocCenter);
        }
    }
}

bool CDialogueWindow::OnThink()
{
    if (m_sDialogue == 0) return false;

    // Type writer effect
    if (!m_bReachedEnd && m_bStartedTyping)
    {
        if (m_sTypedText.length() != m_sText[m_iCurrentTextIndex].Text.length())
        {
            m_sTypedText += m_sText[m_iCurrentTextIndex].Text[m_iCurrentCharacter];
            RenderText();
            m_iCurrentCharacter++;
        }
        // Typing is completed, but there is still more text
        else
        {
            if (m_iCurrentTextIndex + 1 > m_sText.size() - 1)
            {
                m_bFinishedTyping = true;
                m_bReachedEnd = true;
            }
            else
            {
                m_bFinishedTyping = true;
            }
        }
    }

    if (m_pAdvanceTextIcon != 0)
    {
        if ((m_dAdvLocRot + 5.f) > 360.f)
        {
            m_dAdvLocRot = 0.f;
        }
        else
        {
            m_dAdvLocRot += 5.f;
        }
    }

    SDL_Delay(5);

    return true;
}

void CDialogueWindow::Destroy()
{
    CInputHandler::Instance()->RemoveOnMouseDown(m_hAdvanceText);
    SetDrawState(false);
}

void CDialogueWindow::AdvanceText()
{
    if (m_bFinishedTyping && m_sText[m_iCurrentTextIndex].Type == "end")
    {
        Destroy();
    }
    else if (!m_bFinishedTyping && m_bStartedTyping)
    {
        m_sTypedText = m_sText[m_iCurrentTextIndex].Text;
        RenderText();
        m_iCurrentCharacter = int(m_sTypedText.length() - 1);
    }
    else if (m_bFinishedTyping)
    {
        m_iCurrentTextIndex++;
        m_iCurrentCharacter = 0;
        m_sTypedText = "";
        m_bFinishedTyping = false;

        RenderText();
    }
}

void CDialogueWindow::RenderText()
{
    CFontManager::Instance()->RenderText(m_sTypedText, "Roboto-Regular-16", "_DiagTest", CFontManager::EFontRenderType::BLENDED_WRAPPED, { 255, 255, 255 }, { 255, 255, 255 }, m_iWidth - m_iTextPadding * 2);

    m_iTextWidth = CTextureManager::Instance()->m_textureMap[m_sFontID]->GetWidth();
    m_iTextHeight = CTextureManager::Instance()->m_textureMap[m_sFontID]->GetHeight();
}

void CDialogueWindow::AdvanceBind(SDL_Event e)
{
    if (!m_bDrawObject) return;

    // Get the mouse Position on the screen
    CVector2D* pMousePos = CInputHandler::Instance()->
        GetMousePosition();

    // Is it within the boundaries of the CButton?
    if (pMousePos->GetX() < (m_vPosition.GetX() + m_iWidth)
        && pMousePos->GetX() > m_vPosition.GetX()
        && pMousePos->GetY() < (m_vPosition.GetY() + m_iHeight)
        && pMousePos->GetY() > m_vPosition.GetY())
    {
        AdvanceText();
    }
}