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

#include "PopcornFXMinimal.h"

#include "Runtime/Launch/Resources/Version.h"
#include "PropertyEditorModule.h"

#if (ENGINE_MINOR_VERSION >= 15)
#	include "IDetailCustomization.h"
#endif

class UPopcornFXEmitterComponent;
class UPopcornFXEffect;

class FPopcornFXDetailsEmitterComponent : public IDetailCustomization
{
public:
	FPopcornFXDetailsEmitterComponent();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization>	MakeInstance();

	virtual void		CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	void				OnGenerateElementSingleAttribute(TSharedRef<IPropertyHandle> elementProperty, int32 elementIndex, IDetailChildrenBuilder& childrenBuilder);

	TOptional<float>	OnGetValue(float field) const;
	void				OnSetValue(float newValue, ETextCommit::Type commitInfo, float *field);
	int32				GetVectorDimension(int32 field) const;

	void				ReloadAttributes();

private:
	void				GatherEmitters(TArray<UPopcornFXEmitterComponent*> &outComponents) const;
	void				GatherEffects(TArray<UPopcornFXEffect*> &outEffects);

	FReply				OnStartEmitter();
	FReply				OnStopEmitter();
	FReply				OnRestartEmitter();
	bool				IsStartEnabled() const;
	bool				IsStopEnabled() const;

	FReply				OnReloadEffect();
	FReply				OnReimportEffect();

	/** Cached off reference to the layout builder */
	IDetailLayoutBuilder				*m_DetailLayout;
	TWeakPtr<IPropertyUtilities>		m_PropertyUtilities;

	TArray< TWeakObjectPtr<UObject> >	m_SelectedObjectsList;

	TSharedPtr<IPropertyHandle>		m_EffectPty;
	TSharedPtr<IPropertyHandle>		m_AttributesPty;
};

#endif // WITH_EDITOR
