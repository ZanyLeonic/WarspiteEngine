#pragma once
#ifndef __IDialogueManager__
#define __IDialogueManager__

#include <string>

struct SDialogue;

class IDialogueManager
{
public:
	virtual void Load(std::string id, std::string path) = 0;
	virtual bool Remove(std::string id) = 0;
	virtual void RemoveAll() = 0;
	
	virtual SDialogue* GetDialogue(std::string id) = 0;
};

#endif /* defined(__DialogueManager__) */