#include "DialogueParser.h"
#include "EngineFileSystem.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

SDialogue CDialogueParser::ParseDialogue(std::string path)
{
    rapidjson::Document dialogueFile;
    SDialogue outDiag;

    if (CEngineFileSystem::ReadJSON(path, &dialogueFile))
    {
        outDiag.ID = dialogueFile["id"].GetString();
        outDiag.StartNode = dialogueFile["startnode"].GetInt();

        rapidjson::Value nodes = dialogueFile["nodes"].GetArray();

        for (rapidjson::SizeType i = 0; i < nodes.Size(); i++)
        {
            SDialogueNodes node;

            node.Text = nodes[i]["text"].GetString();
            node.Type = nodes[i]["type"].GetString();
            node.NextID = nodes[i].HasMember("nextid") ? nodes[i]["nextid"].GetInt() : -1;

            outDiag.Nodes.push_back(node);
        }

        rapidjson::Value vars = dialogueFile["variables"].GetArray();

        for (rapidjson::SizeType j = 0; j < vars.Size(); j++)
        {
            SDialogueProperties prop;

            prop.Name = nodes[j]["name"].GetString();
            prop.Type = nodes[j]["type"].GetString();
            prop.Value = nodes[j]["value"].GetString();

            outDiag.Variables.push_back(prop);
        }
    }

    return outDiag;
}
