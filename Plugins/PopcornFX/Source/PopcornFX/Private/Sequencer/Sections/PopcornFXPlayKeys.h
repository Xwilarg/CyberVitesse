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
#include "Misc/Attribute.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "IntegralKeyArea.h"

class ISequencer;

/**
* A widget for editing a curve representing emitter play state keys.
*/
class SPopcornFXPlayStateCurveKeyEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPopcornFXPlayStateCurveKeyEditor) {}

	/** The sequencer which is controlling this key editor. */
	SLATE_ARGUMENT(ISequencer*, Sequencer)

		/** The section that owns the data edited by this key editor. */
		SLATE_ARGUMENT(UMovieSceneSection*, OwningSection)

		/** The curve being edited by this curve editor. */
		SLATE_ARGUMENT(struct FIntegralCurve*, Curve)

		/** The UEnum type which provides options for the curve being edited. */
		SLATE_ARGUMENT(const UEnum*, Enum)

		/** Allows the value displayed and edited by this key editor to be supplied from an external source.  This
		is useful for curves on property tracks who's property value can change without changing the animation. */
		SLATE_ATTRIBUTE(TOptional<uint8>, ExternalValue)

		SLATE_END_ARGS();

	void Construct(const FArguments &inArgs);

private:
	int32	OnGetCurrentValue() const;
	void	OnComboSelectionChanged(int32 inSelectedItem, ESelectInfo::Type selectInfo);

	ISequencer						*m_Sequencer;
	UMovieSceneSection				*m_OwningSection;
	struct FIntegralCurve			*m_Curve;
	TAttribute<TOptional<uint8>>	m_ExternalValue;
};

/**
 * A key area for displaying and editing integral curves representing enums.
 */
class FPopcornFXPlayStateKeyArea : public FIntegralKeyArea<uint8>
{
public:
	FPopcornFXPlayStateKeyArea(FIntegralCurve &inCurve, UMovieSceneSection *inOwningSection, const UEnum *inEnum)
		: FIntegralKeyArea(inCurve, inOwningSection)
		, m_Enum(inEnum)
	{
	}

	FPopcornFXPlayStateKeyArea(FIntegralCurve &inCurve, TAttribute<TOptional<uint8>> externalValue, UMovieSceneSection *inOwningSection, const UEnum *inEnum)
		: FIntegralKeyArea(inCurve, externalValue, inOwningSection)
		, m_Enum(inEnum)
	{
	}
public:
	virtual bool					CanCreateKeyEditor() override;
	virtual TSharedRef<SWidget>		CreateKeyEditor(ISequencer *sequencer) override;
	uint8							ConvertCurveValueToIntegralType(int32 curveValue) const override;
private:
	const UEnum		*m_Enum;
};

#endif // WITH_EDITOR
