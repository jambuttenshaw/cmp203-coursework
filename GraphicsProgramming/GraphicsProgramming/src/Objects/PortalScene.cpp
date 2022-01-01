#include "PortalScene.h"
#include "Portal.h"


void PortalScene::OnRenderPortals()
{
	// nullptr checks,
	if (mEntryPortal != nullptr)
	{
		mEntryPortal->Render();
	}

	if (mExitPortal != nullptr)
	{
		// quite often the entry portal and exit portal will be identical
		if (mEntryPortal != mExitPortal)
		{
			mExitPortal->Render();
		}
	}
}
