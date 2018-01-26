//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "GameFramework/HUD.h"

#include "PopcornFXHUDProfiler.generated.h"

class CPopcornFXHUDProfiler_Data;

UCLASS(MinimalAPI, Config=Game)
class APopcornFXHUDProfiler : public AHUD
{
	GENERATED_UCLASS_BODY()
public:
	~APopcornFXHUDProfiler();

	void				DrawDebugHUD(UCanvas* canvas, APlayerController* controller);

	virtual void		PostActorCreated() override;
	virtual void		Destroyed() override;

private:
	void				DrawVBar(float minX, float maxX, float y, float value, float maxValue, float thickness);
	void				DrawVBarPercCol(float minX, float maxX, float y, float perc, float percForColor, float thickness);

	bool				m_ProfilerStarted = false;
	bool				m_HasHBODescriptors = false;

	class CPopcornFXHUDProfiler_Data	*m_Data;
};
