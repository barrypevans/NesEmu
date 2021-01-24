#include "Messager.h"
#include <assert.h>
Messager* Messager::g_messager = nullptr;

Messager::Messager()
{
	assert(!g_messager);
}

Messager::~Messager()
{
}

void Messager::SendMessage(std::string label, void * payload)
{
	for (auto& inbox : m_pInboxes)
	{
		inbox->RecieveMessage(label, payload);
	}
}

void Messager::RegisterInbox(Inbox * pInbox)
{
	for (auto& inbox : m_pInboxes)
		if (inbox == pInbox)
			return;
	m_pInboxes.push_back(pInbox);
}

Messager * Messager::Get()
{
	if (!Messager::g_messager)
	{
		g_messager = new Messager();
	}
	return g_messager;
}
