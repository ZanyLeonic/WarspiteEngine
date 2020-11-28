#pragma once
#ifndef __GARBAGECOLLECTOR_H__
#define __GARBAGECOLLECTOR_H__

#include <memory>
#include <vector>

class CGameStateBase;

class CGarbageCollector
{
public:
	virtual void OnThink();
	virtual void Destroy();

	virtual void MarkStateForDeletion(std::unique_ptr<CGameStateBase> state);

private:
	std::vector<std::unique_ptr<CGameStateBase>> m_StatesDeletionQueue = {};
};
#endif