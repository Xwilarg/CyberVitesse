//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "Sequencer/Templates/PopcornFXPlaySectionTemplate.h"

#include "Sequencer/Tracks/PopcornFXPlayTrack.h"

#include "PopcornFXPlugin.h"

#include "PopcornFXEmitter.h"
#include "PopcornFXEmitterComponent.h"

#include "IMovieScenePlayer.h"

DECLARE_CYCLE_STAT(TEXT("PopcornFX Play Track Token Execute"), MovieSceneEval_PopcornFXPlayTrack_TokenExecute, STATGROUP_MovieSceneEval);

UPopcornFXEmitterComponent	*TryGetEmitterComponent(UObject *object)
{
	UPopcornFXEmitterComponent	*emitterComponent = Cast<UPopcornFXEmitterComponent>(object);

	if (emitterComponent == null)
	{
		APopcornFXEmitter	*emitter = Cast<APopcornFXEmitter>(object);
		emitterComponent = emitter != null ? emitter->PopcornFXEmitterComponent : null;
	}
	if (emitterComponent == null ||
		emitterComponent->Effect == null)
		return null;
	return emitterComponent;
}

struct FPopcornFXPlayPreAnimatedToken : IMovieScenePreAnimatedToken
{
	FPopcornFXPlayPreAnimatedToken(UObject& inObject)
	{
		m_CachedIsAlive = false;

		UPopcornFXEmitterComponent	*emitterComponent = TryGetEmitterComponent(&inObject);
		if (emitterComponent != null)
			m_CachedIsAlive = emitterComponent->IsEmitterAlive();
	}
	
	virtual void	RestoreState(UObject &object, IMovieScenePlayer &player) override
	{
		UPopcornFXEmitterComponent	*emitterComponent = TryGetEmitterComponent(&object);

		if (emitterComponent == null)
			return;

		const bool	isAlive = emitterComponent->IsEmitterAlive();
		if (m_CachedIsAlive && !isAlive)
			emitterComponent->StartEmitter();
		else if (!m_CachedIsAlive && isAlive)
			emitterComponent->StopEmitter();
	}

	bool	m_CachedIsAlive;
};

struct FPopcornFXPlayPreAnimatedTokenProducer : IMovieScenePreAnimatedTokenProducer
{
	static FMovieSceneAnimTypeID	GetAnimTypeID()
	{
		return TMovieSceneAnimTypeID<FPopcornFXPlayPreAnimatedTokenProducer>();
	}
private:
	virtual IMovieScenePreAnimatedTokenPtr	CacheExistingState(UObject &object) const override
	{
		return FPopcornFXPlayPreAnimatedToken(object);
	}
};

struct FPopcornFXPlayStateKeyState : IPersistentEvaluationData
{
	FKeyHandle	m_LastKeyHandle;
	FKeyHandle	m_InvalidKeyHandle;
};

struct FPopcornFXPlayExecutionToken : IMovieSceneExecutionToken
{
	FPopcornFXPlayExecutionToken(EPopcornFXPlayStateKey::Type keyType, TOptional<FKeyHandle> keyHandle = TOptional<FKeyHandle>())
		: m_KeyType(keyType)
		, m_KeyHandle(keyHandle)
	{

	}

	virtual void	Execute(const FMovieSceneContext &context,
		const FMovieSceneEvaluationOperand &operand,
		FPersistentEvaluationData &persistentData,
		IMovieScenePlayer &player)
	{
		MOVIESCENE_DETAILED_SCOPE_CYCLE_COUNTER(MovieSceneEval_PopcornFXPlayTrack_TokenExecute);

		if (m_KeyHandle.IsSet())
			persistentData.GetOrAddSectionData<FPopcornFXPlayStateKeyState>().m_LastKeyHandle = m_KeyHandle.GetValue();

		for (TWeakObjectPtr<> &weakObject : player.FindBoundObjects(operand))
		{
			UPopcornFXEmitterComponent	*emitterComponent = TryGetEmitterComponent(weakObject.Get());

			if (emitterComponent == null)
				continue;

			player.SavePreAnimatedState(*emitterComponent, FPopcornFXPlayPreAnimatedTokenProducer::GetAnimTypeID(), FPopcornFXPlayPreAnimatedTokenProducer());

			switch (m_KeyType)
			{
				case	EPopcornFXPlayStateKey::Start:
				{
					emitterComponent->RestartEmitter();
					break;
				}
				case	EPopcornFXPlayStateKey::Stop:
				{
					emitterComponent->StopEmitter();
					break;
				}
				case	EPopcornFXPlayStateKey::Toggle:
				{
					emitterComponent->ToggleEmitter(!emitterComponent->IsEmitterStarted());
					break;
				}
			}
		}
	}

	EPopcornFXPlayStateKey::Type	m_KeyType;
	TOptional<FKeyHandle>			m_KeyHandle;
};

FPopcornFXPlaySectionTemplate::FPopcornFXPlaySectionTemplate(const UMovieSceneParticleSection &section)
	: Keys(section.GetParticleCurve())
{
}

void	FPopcornFXPlaySectionTemplate::Evaluate(const FMovieSceneEvaluationOperand &operand,
	const FMovieSceneContext &context,
	const FPersistentEvaluationData &persistentData,
	FMovieSceneExecutionTokens &executionTokens) const
{
	const bool	bPlaying = context.GetDirection() == EPlayDirection::Forwards &&
		context.GetRange().Size<float>() >= 0.0f &&
		context.GetStatus() == EMovieScenePlayerStatus::Playing;

	const FPopcornFXPlayStateKeyState	*sectionData = persistentData.FindSectionData<FPopcornFXPlayStateKeyState>();

	if (!bPlaying)
	{
		executionTokens.Add(FPopcornFXPlayExecutionToken(EPopcornFXPlayStateKey::Stop, sectionData != null ? sectionData->m_InvalidKeyHandle : TOptional<FKeyHandle>()));
	}
	else
	{
		FKeyHandle	prevHandle = Keys.FindKeyBeforeOrAt(context.GetTime());
		if (Keys.IsKeyHandleValid(prevHandle) && (sectionData == null || sectionData->m_LastKeyHandle != prevHandle))
		{
			FPopcornFXPlayExecutionToken	token((EPopcornFXPlayStateKey::Type)Keys.GetKey(prevHandle).Value, prevHandle);

			executionTokens.Add(MoveTemp(token));
		}
	}
}
