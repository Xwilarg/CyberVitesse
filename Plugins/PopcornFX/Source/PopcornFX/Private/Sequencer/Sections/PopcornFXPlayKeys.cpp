//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#if WITH_EDITOR

#include "PopcornFXPlayKeys.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Curves/KeyHandle.h"
#include "Curves/IntegralCurve.h"
#include "ISequencer.h"
#include "ScopedTransaction.h"
#include "MovieSceneToolHelpers.h"

#define LOCTEXT_NAMESPACE	"PopcornFXPlayStateCurveKeyEditor"

//----------------------------------------------------------------------------
//
//	SPopcornFXPlayStateCurveKeyEditor
//
//----------------------------------------------------------------------------

void	SPopcornFXPlayStateCurveKeyEditor::Construct(const FArguments &inArgs)
{
	m_Sequencer = inArgs._Sequencer;
	m_OwningSection = inArgs._OwningSection;
	m_Curve = inArgs._Curve;
	m_ExternalValue = inArgs._ExternalValue;

	ChildSlot
	[
		MovieSceneToolHelpers::MakeEnumComboBox(
			inArgs._Enum,
			TAttribute<int32>::Create(TAttribute<int32>::FGetter::CreateSP(this, &SPopcornFXPlayStateCurveKeyEditor::OnGetCurrentValue)),
			FOnEnumSelectionChanged::CreateSP(this, &SPopcornFXPlayStateCurveKeyEditor::OnComboSelectionChanged)
		)
	];
}

//----------------------------------------------------------------------------

int32	SPopcornFXPlayStateCurveKeyEditor::OnGetCurrentValue() const
{
	if (m_ExternalValue.IsSet() && m_ExternalValue.Get().IsSet())
	{
		return m_ExternalValue.Get().GetValue();
	}

	const float currentTime = m_Sequencer->GetLocalTime();

	int32	defaultValue = 0;
	return m_Curve->Evaluate(currentTime, defaultValue);
}

//----------------------------------------------------------------------------

void	SPopcornFXPlayStateCurveKeyEditor::OnComboSelectionChanged(int32 inSelectedItem, ESelectInfo::Type selectInfo)
{
	FScopedTransaction	Transaction(LOCTEXT("SetPlayStateKey", "Set Emitter Play State Key Value"));

	m_OwningSection->SetFlags(RF_Transactional);

	if (m_OwningSection->TryModify())
	{
		const float		currentTime = m_Sequencer->GetLocalTime();
		const bool		bAutoSetTrackDefaults = m_Sequencer->GetAutoSetTrackDefaults();

		const FKeyHandle	currentKeyHandle = m_Curve->FindKey(currentTime);
		if (m_Curve->IsKeyHandleValid(currentKeyHandle))
			m_Curve->SetKeyValue(currentKeyHandle, inSelectedItem);
		else
		{
			if (m_Curve->GetNumKeys() != 0 || !bAutoSetTrackDefaults)
				m_Curve->AddKey(currentTime, inSelectedItem, currentKeyHandle);

			if (m_Curve->GetNumKeys() != 0)
			{
				if (m_OwningSection->GetStartTime() > currentTime)
					m_OwningSection->SetStartTime(currentTime);
				if (m_OwningSection->GetEndTime() < currentTime)
					m_OwningSection->SetEndTime(currentTime);
			}
		}

		if (bAutoSetTrackDefaults)
			m_Curve->SetDefaultValue(inSelectedItem);

		m_Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::TrackValueChangedRefreshImmediately);
	}
}

//----------------------------------------------------------------------------
//
//	FPopcornFXPlayStateKeyArea
//
//----------------------------------------------------------------------------

bool	FPopcornFXPlayStateKeyArea::CanCreateKeyEditor()
{
	return true;
}

//----------------------------------------------------------------------------

TSharedRef<SWidget>	FPopcornFXPlayStateKeyArea::CreateKeyEditor(ISequencer *sequencer)
{
	return SNew(SPopcornFXPlayStateCurveKeyEditor)
		.Sequencer(sequencer)
		.OwningSection(OwningSection)
		.Curve(&Curve)
		.Enum(m_Enum)
		.ExternalValue(ExternalValue);
}

//----------------------------------------------------------------------------

uint8	FPopcornFXPlayStateKeyArea::ConvertCurveValueToIntegralType(int32 curveValue) const
{
	return (uint8)FMath::Clamp<int32>(curveValue, 0, TNumericLimits<uint8>::Max());
}

//----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR
