#include "DialogueWindow.h"
#include "FontManager.h"
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "SDL_ttf.h"

CDialogueWindow::CDialogueWindow()
    : CUIBase()
{}

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

void CDialogueWindow::SetDialogue(SDialogue pDiag)
{
    // Setup the object
    m_sDialogue = pDiag;
    m_sText = m_sDialogue.Nodes;

    m_bStartedTyping = true;

    CBaseGame::Instance()->DisablePlayerMovement(true);
}

void CDialogueWindow::Draw()
{
    CUIBase::Draw();

    // Only draw the object if it is visible and has started typing
    if (m_bStartedTyping && m_bDrawObject)
    {
        // Draw the text with our padding rules and general rules
        CTextureManager::Instance()->DrawFrame(m_sFontID, m_iTextPadding + (int)m_vPosition.GetX(),
            m_iTextPadding + (int)m_vPosition.GetY(), m_iTextWidth, m_iTextHeight,
            m_iCurrentRow, m_iCurrentFrame, CBaseGame::Instance()->GetRenderer());
    
        // Only draw the advance indicator if there is more nodes.
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
    // Type writer effect
    if (!m_bReachedEnd && m_bStartedTyping)
    {
        // Have we not printed all the characters from the current node yet?
        if (m_sTypedText.length() != m_sText[m_iCurrentTextIndex].Text.length())
        {
            // Then print the next character into our typed text
            m_sTypedText += m_sText[m_iCurrentTextIndex].Text[m_iCurrentCharacter];
            RenderText();
            m_iCurrentCharacter++;
        }
        // Typing is completed, but there is still more text
        else
        {
            // If it is the last node
            if (m_sText[m_iCurrentTextIndex].NextID == -1)
            {
                // Toggle the flags that we have reached the end of the dialogue file
                m_bFinishedTyping = true; 
                m_bReachedEnd = true;
            }
            else
            {
                // Toggle the flag that we have reached the end of this node
                m_bFinishedTyping = true;
            }
        }
    }

    // Make sure the object has loaded
    if (m_pAdvanceTextIcon != 0)
    {
        // Rotate the object until the text has been advanced.
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
    // Clean up
    CInputHandler::Instance()->RemoveOnMouseDown(m_hAdvanceText);
    SetDrawState(false);

    CBaseGame::Instance()->DisablePlayerMovement(false);
}

void CDialogueWindow::AdvanceText()
{
    // If we are advancing on a end node
    if (m_bFinishedTyping && m_sText[m_iCurrentTextIndex].Type == "end" || m_bReachedEnd)
    {
        Destroy(); // Destroy the window
    }
    // if we are still typing when we advance the text
    else if (!m_bFinishedTyping && m_bStartedTyping) 
    {
        // Output all the current text into our typed text
        m_sTypedText = m_sText[m_iCurrentTextIndex].Text;
        RenderText();
        m_iCurrentCharacter = int(m_sTypedText.length() - 1);

        TTF_
    }
    // If we have finished typing (and we aren't on the last node) and text is advanced
    else if (m_bFinishedTyping && !m_bReachedEnd)
    {
        // Move to the next node
        m_iCurrentTextIndex++;
        m_iCurrentCharacter = 0;
        m_sTypedText = "";
        m_bFinishedTyping = false;

        RenderText();
    }
}

void CDialogueWindow::RenderText()
{
    // Render to the texture diag test
    // TODO: Rich text/tags
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

    // Is it within the boundaries of the dialogue window?
    if (pMousePos->GetX() < (m_vPosition.GetX() + m_iWidth)
        && pMousePos->GetX() > m_vPosition.GetX()
        && pMousePos->GetY() < (m_vPosition.GetY() + m_iHeight)
        && pMousePos->GetY() > m_vPosition.GetY())
    {
        AdvanceText();
    }
}