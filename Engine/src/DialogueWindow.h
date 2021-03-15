#pragma once
#ifndef __DIALOGUEWINDOW_H__
#define __DIALOGUEWINDOW_H__

#include "UIBase.h"

class CDialogueWindow :
    public CUIBase
{
public:
    CDialogueWindow();

	virtual void Load(CObjectParams* pParams);

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();
private:
	CVector2D* pScreenSize = 0;
	CVector2D* pPlayerLocation = 0;

	std::shared_ptr<IGameObject> pPlayer;

	std::string m_sDialogueFile = "test.json";
	std::string m_sFontID = "_DiagTest";

	int m_iTextPadding = 15;
	int m_iTextSize = 16;

	int m_iTextWidth = 0;
	int m_iTextHeight = 0;

	bool m_bFinishedTyping = false;
	bool m_bStartedTyping = false;

	std::string m_sText;
	std::string m_sTypedText;

	int m_iCurrentCharacter = 0;
};

#endif