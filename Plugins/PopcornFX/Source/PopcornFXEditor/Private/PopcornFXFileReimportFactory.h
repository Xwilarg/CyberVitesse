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

#include "PopcornFXFileFactory.h"
#include "EditorReimportHandler.h"

#include "PopcornFXFileReimportFactory.generated.h"

UCLASS(MinimalAPI)
class UPopcornFXFileReimportFactory : public UPopcornFXFileFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()
public:
	virtual bool					CanReimport(UObject *obj, TArray<FString> &outFilenames) override;
	virtual void					SetReimportPaths(UObject *obj, const TArray<FString> &newReimportPaths) override;
	virtual EReimportResult::Type	Reimport(UObject *obj) override;
	virtual int32					GetPriority() const override;
};
