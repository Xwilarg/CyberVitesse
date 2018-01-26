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


#if (ENGINE_MINOR_VERSION >= 17)
struct FAssetData;
#else
class FAssetData;
#endif


class FPopcornFXCustomizationAssetDep : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// overrides IPropertyTypeCustomization
	virtual void	CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void	CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	class UPopcornFXAssetDep		*Self() const;

	bool			OnFilterAssetPicker(const FAssetData& InAssetData) const;
	FReply			OnResetClicked();
	EVisibility		GetResetVisibility() const;
	FReply			OnImportDefaultClicked();
	FText			GetImportButtonText() const;
	FText			GetCurrentAssetPath() const;

	TSharedPtr<IPropertyHandle>		m_SelfPty;
	TSharedPtr<IPropertyHandle>		m_AssetPty;
};

#endif // WITH_EDITOR
