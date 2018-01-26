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

class UPopcornFXAttributeList;

class FPopcornFXDetailsAttributeList : public IDetailCustomization
{
public:
	typedef FPopcornFXDetailsAttributeList		TSelf;

	FPopcornFXDetailsAttributeList();
	~FPopcornFXDetailsAttributeList();

	static TSharedRef<IDetailCustomization>	MakeInstance();
	virtual void		CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	//const UPopcornFXAttributeList	*AttrList() const;
	UPopcornFXAttributeList			*AttrList();

public:
	TSharedPtr<IPropertyHandle>		m_AttributesPty;
	TSharedPtr<IPropertyHandle>		m_SamplersPty;
	//TSharedPtr<IPropertyHandle>	m_AttributesRawDataPty;

private:
	EVisibility					AttribVisibilityAndRefresh();
	UPopcornFXAttributeList		*UnsafeAttrList();

	void						Rebuild();
	void						RebuildIFN();
	void						RebuildAttributes();
	void						RebuildSamplers();
	void						RebuildAndRefresh();

	TArray<TWeakObjectPtr<UObject> >	m_BeingCustomized;
	//IDetailLayoutBuilder		*m_DetailBuilder;
	TSharedPtr<IPropertyUtilities>		m_PropertyUtilities;

	TSharedPtr<IPropertyHandle>	m_FileVersionIdPty;
	uint32						m_FileVersionId;

	TSharedPtr<SVerticalBox>	m_SAttributes;
	TSharedPtr<SVerticalBox>	m_SSamplers;
};

#endif // WITH_EDITOR
