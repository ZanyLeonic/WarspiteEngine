#pragma once
#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "IDialogueManager.h"

#include <map>
#include <string>
#include <memory>
#include "DialogueTypes.h"

class CDialogueManager : public IDialogueManager
{
	static IDialogueManager* s_pInstance;
	CDialogueManager() {};

public:
	// more singleton stuff
	static IDialogueManager* Instance();

	virtual void Load(std::string id, std::string path);
	virtual bool Remove(std::string id);
	virtual void RemoveAll();

	virtual SDialogue* GetDialogue(std::string id);

private:
	std::map<std::string, std::unique_ptr<SDialogue*>> m_sMLoadedDialogue;
};

#endif