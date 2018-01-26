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

#include "Evaluation/MovieSceneEvalTemplate.h"
#include "Sections/MovieSceneParticleSection.h"
#include "PopcornFXPlaySectionTemplate.generated.h"

USTRUCT()
struct FPopcornFXPlaySectionTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	FPopcornFXPlaySectionTemplate() {}
	FPopcornFXPlaySectionTemplate(const class UMovieSceneParticleSection &section);

	UPROPERTY()
	FIntegralCurve	Keys;

private:
	virtual UScriptStruct	&GetScriptStructImpl() const override { return *StaticStruct(); }

	virtual void			Evaluate(const FMovieSceneEvaluationOperand &operand,
		const FMovieSceneContext &context,
		const FPersistentEvaluationData &persistentData,
		FMovieSceneExecutionTokens &executionTokens) const override;
};
