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

#include "Evaluation/MovieSceneParameterTemplate.h"
#include "PopcornFXAttributeSectionTemplate.generated.h"

USTRUCT()
struct FPopcornFXAttributeSectionTemplate : public FMovieSceneParameterSectionTemplate
{
	GENERATED_BODY()

	FPopcornFXAttributeSectionTemplate() {}
	FPopcornFXAttributeSectionTemplate(const UMovieSceneParameterSection &section, const class UPopcornFXAttributeTrack &track);

private:
	virtual UScriptStruct	&GetScriptStructImpl() const override { return *StaticStruct(); }

	virtual void			Evaluate(const FMovieSceneEvaluationOperand &operand,
		const FMovieSceneContext &context,
		const FPersistentEvaluationData &persistentData,
		FMovieSceneExecutionTokens &executionTokens) const override;
};
