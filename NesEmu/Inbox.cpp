#include "Inbox.h"

#include "Messager.h"

void Inbox::Init()
{
	Messager::Get()->RegisterInbox(this);
}
