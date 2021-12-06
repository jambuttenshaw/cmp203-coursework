#include "PortalScene.h"
#include "Portal.h"


void PortalScene::OnRenderPortals()
{
	if (mEntryPortal != nullptr)
	{
		mEntryPortal->Render();
	}

	if (mExitPortal != nullptr)
	{
		if (mEntryPortal != mExitPortal)
		{
			mExitPortal->Render();
		}
	}
}
