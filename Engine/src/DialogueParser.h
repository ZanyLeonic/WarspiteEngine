#pragma once
#ifndef __DIAGPARSER_H__
#define __DIAGPARSER_H__

#include "DialogueTypes.h"

class CDialogueParser
{
public:
	static bool ParseDialogue(std::string path, SDialogue* pOutDiag);
};

#endif