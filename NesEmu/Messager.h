#pragma once
#include "Inbox.h"

#include <vector>

class Messager
{
public:
	Messager();
	~Messager();
	void SendMessage(std::string label, void* pPayload);
	void RegisterInbox(Inbox* pInbox);
	
	static Messager* Get();

private:
	std::vector<Inbox*> m_pInboxes;
	static Messager* g_messager;
};

