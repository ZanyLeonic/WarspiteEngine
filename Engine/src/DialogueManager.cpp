#include "DialogueManager.h"

#include "DialogueParser.h"
#include <spdlog/spdlog.h>

IDialogueManager* CDialogueManager::s_pInstance = 0;

IDialogueManager* CDialogueManager::Instance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new CDialogueManager();
    }

    return s_pInstance;
}

void CDialogueManager::Load(std::string id, std::string path)
{
    SDialogue nDiag;

    if (CDialogueParser::ParseDialogue(path, &nDiag))
    {
        m_sMLoadedDialogue[id] = nDiag;
    }
    else
    {
        spdlog::warn("Failed to read dialogue ID \"{}\" from path \"{}\"!", id, path);
    }
}

bool CDialogueManager::Remove(std::string id)
{
    auto search = m_sMLoadedDialogue.find(id);
    if (search != m_sMLoadedDialogue.end())
    {
        m_sMLoadedDialogue.erase(search);
        return true;
    }
    return false;
}

void CDialogueManager::RemoveAll()
{
    
}

SDialogue CDialogueManager::GetDialogue(std::string id)
{
    return m_sMLoadedDialogue[id];
}
