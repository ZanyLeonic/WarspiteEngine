#include "DialogueParser.h"
#include "EngineFileSystem.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

bool CDialogueParser::ParseDialogue(std::string path, SDialogue* pOutDiag)
{
    rapidjson::Document dialogueFile;

    if (CEngineFileSystem::ReadJSON(path, &dialogueFile))
    {
        pOutDiag->ID = dialogueFile["id"].GetString();
        pOutDiag->StartNode = dialogueFile["startnode"].GetInt();

        rapidjson::Value nodes = dialogueFile["nodes"].GetArray();

        for (rapidjson::SizeType i = 0; i < nodes.Size(); i++)
        {
            SDialogueNodes node;

            node.Text = nodes[i]["text"].GetString();
            node.Type = nodes[i]["type"].GetString();
            node.NextID = nodes[i].HasMember("nextid") ? nodes[i]["nextid"].GetInt() : -1;

            pOutDiag->Nodes.push_back(node);
        }

        rapidjson::Value vars = dialogueFile["variables"].GetArray();

        for (rapidjson::SizeType j = 0; j < vars.Size(); j++)
        {
            SDialogueProperties prop;

            prop.Name = vars[j]["name"].GetString();
            prop.Type = vars[j]["type"].GetString();
            prop.Value = vars[j]["value"].GetString();

            pOutDiag->Variables.push_back(prop);
        }

        return true;
    }

    return false;
}