#pragma once
#ifndef __DIALOGUECHOICE_H__
#define __DIALOGUECHOICE_H__

#include "UIBase.h"

class CDialogueWindow;

class CDialogueChoice :
    public CUIBase
{
public:
    CDialogueChoice(CDialogueWindow* pParent);

    virtual void Load(CObjectParams* pParams);

private:
    CDialogueWindow* m_pParent = 0;
};

#endif