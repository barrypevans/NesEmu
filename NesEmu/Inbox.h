#pragma once
#include <string>

class Inbox
{
public:
	void Init();
	virtual void RecieveMessage(std::string label, void* payload) = 0;
};