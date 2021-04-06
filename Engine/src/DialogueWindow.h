#pragma once
#ifndef __DIALOGUEWINDOW_H__
#define __DIALOGUEWINDOW_H__

#include "UIBase.h"
#include "DialogueTypes.h"

class CDialogueWindow :
    public CUIBase
{
public:
    CDialogueWindow();

	virtual void Load(CObjectParams* pParams);

	void SetDialogue(SDialogue* pDiag);

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();
private:
	void AdvanceText();
	void RenderText();

	void AdvanceBind(SDL_Event e);

	CVector2D* m_pScreenSize = 0;
	CVector2D* m_pPlayerLocation = 0;

	std::shared_ptr<IGameObject> m_pPlayer;

	SDialogue* m_sDialogue = 0;
	std::string m_sFontID = "_DiagTest";

	int m_iTextPadding = 15;
	int m_iTextSize = 16;

	int m_iTextWidth = 0;
	int m_iTextHeight = 0;

	bool m_bFinishedTyping = false;
	bool m_bStartedTyping = false;
	bool m_bReachedEnd = false;

	std::vector<SDialogueNodes> m_sText;
	std::string m_sTypedText;

	int m_iCurrentTextIndex = 0;
	int m_iCurrentCharacter = 0;

	HInputCallback m_hAdvanceText = 0;

	CTexture* m_pAdvanceTextIcon = 0;

	CVector2D m_vAdvLoc;
	CVector2D m_vAdvLocCenter;

	double m_dAdvLocRot = 0.f;
};

#endif