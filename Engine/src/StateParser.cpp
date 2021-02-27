#include "StateParser.h"
#include <iostream>
#include <string>
#include "TextureManager.h"
#include "ScriptManager.h"
#include "GameObjectDictionary.h"
#include "ObjectParams.h"
#include "Game.h"
#include <spdlog/spdlog.h>

using namespace rapidjson;

bool CStateParser::ParseState(const char* stateFile, std::string stateID, std::vector<std::unique_ptr<IGameObject>>* pObjects, 
	std::vector<std::string>* pTextureIDs, std::vector<std::string>* pScriptRefs)
{
	// Read our JSON document.
	Document jDoc;

	spdlog::debug("Reading file \"{}\"...", stateFile);

	// load state file
	if (CEngineFileSystem::ReadJSON(stateFile, &jDoc))
	{
		// Create a new Value to store our state (if we find it.)
		Value iState = Value(false);
		Value& state = iState;

		assert(jDoc["states"].IsArray());

		spdlog::debug("*** State information start ***");
		spdlog::debug("Number of states: {}", jDoc["states"].Size());
		spdlog::debug("Looking for state \"{}\"", stateID);

		// Try and find our state the caller wants.
		for (SizeType i = 0; i < jDoc["states"].Size() && state.GetType() == Type::kFalseType; i++)
		{
			spdlog::debug("Found named state \"{}\"", jDoc["states"][i]["id"].GetString());

			// if it matches...
			if (jDoc["states"][i]["id"].GetString() == stateID)
			{
				// store it in.
				state = jDoc["states"][i];
				spdlog::debug("Found state in file!");
			}
		}

		// if we still have a boolean...
		if (state.IsBool())
		{
			// we didn't find it.

			spdlog::error("Could not find state \"{}\" in file \"{}\"", stateID, stateFile);

			return false;
		}

		// Find out what textures we need for the state.
		const Value& textures = state["textures"].GetArray();
		assert(textures.IsArray());

		// Load the textures into the TextureManager
		ParseTextures(&textures, pTextureIDs);

		// Find out what objects we need.
		const Value& objects = state["objects"].GetArray();
		assert(objects.IsArray());

		// Add the Objects to the passed m_GameObjects
		ParseObjects(&objects, pObjects);

		const Value& scripts = state["scripts"].GetArray();
		assert(scripts.IsArray());

		// Load the scripts into the ScriptManager
		ParseScripts(&scripts, pScriptRefs);
		
		spdlog::debug("Parsed state file \"{}\"!", stateFile);
		spdlog::debug("*** State information end ***");

		return true;
	}

	// If our file stream is null - something has gone wrong.
	spdlog::error("Cannot read \"{}\"", stateFile);

	return false;
}

void CStateParser::ParseObjects(const rapidjson::Value* pStateRoot, std::vector<std::unique_ptr<IGameObject>>* pObjects)
{
	spdlog::debug("*** Parsing Objects start ***");
	// Get the object array.
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of objects defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Get the current object we are working with.
		const Value& b = t[i];

		if (!b.HasMember("type"))
		{
			spdlog::error("Missing object type attribute for {} item - ignoring...", i);
			continue;
		}

		if (b["type"].GetString() == "")
		{
			spdlog::error("Missing object type for {} item - ignoring...", i);
			continue;
		}

		if (!b.HasMember("x"))
		{
			spdlog::error("Missing X position for object of type \"{}\" - ignoring...", b["type"].GetString());
			continue;
		}

		if (!b.HasMember("y"))
		{
			spdlog::error("Missing Y position for object of type \"{}\" - ignoring...", b["type"].GetString());
			continue;
		}

		spdlog::debug("*** Object creation start ***");
		spdlog::debug("Creating object of \"{}\" at ({}, {})", b["type"].GetString(), 
			b["x"].GetFloat(), b["y"].GetFloat());

		// Attempt to create the object type.
		std::unique_ptr<IGameObject> pGameObject =
			CGameObjectDictionary::Instance()->Create(b["type"].GetString());

		CObjectParams* pOP = new CObjectParams(b["x"].GetFloat(), b["y"].GetFloat());
		// Retrieve the relevant information from the object declaration...
		// Required
		if (b.HasMember("textureWidth"))
			pOP->SetProperty("textureWidth", b["textureWidth"].GetInt());
		else
		{
			spdlog::warn("No width declared for object type \"{}\"", b["type"].GetString());
			pOP->SetProperty("textureWidth", 0);
		}
		
		if (b.HasMember("textureHeight"))
			pOP->SetProperty("textureHeight", b["textureHeight"].GetInt());
		else
		{
			spdlog::warn("No height declared for object type \"{}\"", b["type"].GetString());
			pOP->SetProperty("textureHeight", 0);
		}

		if (b.HasMember("textureID"))
			pOP->SetProperty("textureID", std::string(b["textureID"].GetString()));
		else
		{
			spdlog::warn("No textureID declared for object type \"{}\"", b["type"].GetString());
			pOP->SetProperty("textureID", "");
		}

		pOP->SetName(b.HasMember("name") ? b["name"].GetString() : "");
		pOP->SetFactoryID(b["type"].GetString());

		spdlog::debug("Name: {}", pOP->GetName());
		spdlog::debug("FactoryID: {}", pOP->GetFactoryID());
		spdlog::debug("TextureID: {}", pOP->GetProperty<std::string>("textureID"));
		spdlog::debug("Object Dimensions: {}x{}", pOP->GetProperty<int>("textureWidth"), pOP->GetProperty<int>("textureHeight"));

		// Optional
		pOP->SetProperty("numFrames", b.HasMember("numFrames") ? b["numFrames"].GetInt() : 1);
		pOP->SetProperty("animSpeed", b.HasMember("animSpeed") ? b["animSpeed"].GetInt() : 1);

		pOP->SetProperty("onClickCallback", b.HasMember("onClickCallback") ? b["onClickCallback"].GetInt() : 0);
		pOP->SetProperty("onEnterCallback", b.HasMember("onEnterCallback") ? b["onEnterCallback"].GetInt() : 0);
		pOP->SetProperty("onLeaveCallback", b.HasMember("onLeaveCallback") ? b["onLeaveCallback"].GetInt() : 0);

		pOP->SetProperty("runScript", b.HasMember("runScript") ? std::string(b["runScript"].GetString()) : "");
		pOP->SetProperty("soundPath", b.HasMember("soundPath") ? std::string(b["soundPath"].GetString()) : "");

		spdlog::debug("Number of Frames: {}", pOP->GetProperty<int>("numFrames"));
		spdlog::debug("Animation Speed: {}", pOP->GetProperty<int>("animSpeed"));
		
		spdlog::debug("OnClickID: {}", pOP->GetProperty<int>("onClickCallback"));
		spdlog::debug("OnEnterID: {}", pOP->GetProperty<int>("onEnterCallback"));
		spdlog::debug("OnLeaveID: {}", pOP->GetProperty<int>("onLeaveCallback"));

		spdlog::debug("Associated Script: {}", pOP->GetProperty<std::string>("runScript"));
		spdlog::debug("Assocaited Sound: {}", pOP->GetProperty<std::string>("soundPath"));

		// Provide the extracting info to the object.
		pGameObject->Load(pOP);

		// Add it to the m_GameObjects
		pObjects->push_back(std::move(pGameObject));

		spdlog::debug("Pushed GameObject with info!");
		spdlog::debug("*** Object creation finished ***");
	}
	spdlog::debug("*** Parsing Objects finished ***");
}

void CStateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	spdlog::debug("*** Parsing Textures start ***");
	// Get the texture array. (For some reason the array is in an array?)
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of textures defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Gets the current texture we are working with.
		const Value& b = t[i];

		// Add it to a list so it can be destroyed later.
		pTextureIDs->push_back(b["id"].GetString());

		spdlog::debug("id: \"{}\"", b["id"].GetString());
		spdlog::debug("path: \"{}\"", b["path"].GetString());

		// Load our texture into memory.
		CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath(b["path"].GetString(),
																   CEngineFileSystem::EPathType::TEXTURE),
									b["id"].GetString(), CBaseGame::Instance()->GetRenderer());
	}
	spdlog::debug("*** Parsing Textures finished ***");
}

void CStateParser::ParseScripts(const rapidjson::Value* pStateRoot, std::vector<std::string>* pScriptsID)
{
	spdlog::debug("*** Parsing Scripts start ***");
	// Get the script array. (For some reason the array is in an array?)
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of scripts defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Gets the current script we are working with.
		const Value& b = t[i];

		spdlog::debug("id: \"{}\"", b["id"].GetString());
		spdlog::debug("path: \"{}\"", b["path"].GetString());

		// Add it to a list so it can be destroyed later.
		pScriptsID->push_back(b["id"].GetString());

		// Load our script path and name into memory so it can be loaded later
		CScriptManager::Instance()->Load(SGameScript::file(b["id"].GetString(), 
			CEngineFileSystem::ResolvePath(b["path"].GetString(), CEngineFileSystem::EPathType::SCRIPT)));
	}
	spdlog::debug("*** Parsing Scripts finished ***");
}