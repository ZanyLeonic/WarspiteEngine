#pragma once
#ifndef __DIAGTYPES_H__
#define __DIAGTYPES_H__

#include <vector>
#include <string>
#include <any>

struct SDialogueNodes
{
	std::string Text;
	std::string Type;
	int NextID;
};

struct SDialogueProperties
{
	std::string Name;
	std::string Type;
	std::any Value;
};

struct SDialogue
{
	std::string ID;
	int StartNode = 0;

	std::vector<SDialogueNodes> Nodes;
	std::vector<SDialogueProperties> Variables;
};

#endif