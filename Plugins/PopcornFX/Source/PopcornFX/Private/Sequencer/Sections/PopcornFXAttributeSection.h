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

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "PropertySection.h"

// Hugo: NOTE -- This is ~exact copy of ParameterSection.cpp/h which is private right now
// Ideally, thirdparty could rely on the default implementation

/**
 * A movie scene section for effect Attributes.
 */
class FPopcornFXAttributeSection : public FPropertySection
{
public:

	FPopcornFXAttributeSection(UMovieSceneSection& InSectionObject, const FText& SectionName)
		: FPropertySection(InSectionObject, SectionName)
	{ }

public:
	//~ ISequencerSection interface
	virtual void	GenerateSectionLayout(class ISectionLayoutBuilder& LayoutBuilder) const override;
	virtual bool	RequestDeleteCategory(const TArray<FName>& CategoryNamePath) override;
	virtual bool	RequestDeleteKeyArea(const TArray<FName>& KeyAreaNamePath) override;
};

#endif //WITH_EDITOR
