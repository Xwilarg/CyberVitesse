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

#include "PropertyEditorModule.h"
#include "AssetThumbnail.h"

class FPopcornFXCustomizationCollisionFilter : public IPropertyTypeCustomization
{
public:
	typedef FPopcornFXCustomizationCollisionFilter		Self;

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	FPopcornFXCustomizationCollisionFilter();

	// overrides IPropertyTypeCustomization
	virtual void	CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void	CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	ECheckBoxState				IsChecked(int32 channel) const;
	void						OnCheckStateChanged(ECheckBoxState checked, int32 channel);

	class UCollisionProfile				*m_CollisionProfile;
	TSharedPtr<IPropertyHandle>			m_ObjectTypesToQuery;
};

#endif // WITH_EDITOR
