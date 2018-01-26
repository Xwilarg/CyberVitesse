//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAttributeList.h"

#include "PopcornFXPlugin.h"
#include "Render/RendererSubView.h"
#include "Internal/ParticleScene.h"
#include "Assets/PopcornFXEffect.h"
#include "Editor/EditorHelpers.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXAttributeSamplerShape.h"
#include "PopcornFXCustomVersion.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_descriptor.h>
#include <hh_particles/include/ps_samplers_classes.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXAttributeList, Log, All);

//#define DBG_HERE()	UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- %20s --- %p --- %s"), ANSI_TO_TCHAR(__FUNCTION__), this, *GetFullName());
#define DBG_HERE()

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

enum {
	kAttributeSize = sizeof(PopcornFX::SAttributesContainer::SAttrib)
};

using PopcornFX::CShapeDescriptor;
using PopcornFX::PShapeDescriptor;
using PopcornFX::PCShapeDescriptor;

hh_u32					ToPkShapeType(EPopcornFXAttribSamplerShapeType::Type ueShapeType)
{
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Box					== (hh_u32)CShapeDescriptor::ShapeBox);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Sphere				== (hh_u32)CShapeDescriptor::ShapeSphere);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::ComplexEllipsoid		== (hh_u32)CShapeDescriptor::ShapeComplexEllipsoid);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Cylinder				== (hh_u32)CShapeDescriptor::ShapeCylinder);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Capsule				== (hh_u32)CShapeDescriptor::ShapeCapsule);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Cone					== (hh_u32)CShapeDescriptor::ShapeCone);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Plane				== (hh_u32)CShapeDescriptor::ShapePlane);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Mesh					== (hh_u32)CShapeDescriptor::ShapeMesh);
	HH_STATIC_ASSERT(EPopcornFXAttribSamplerShapeType::Collection			== (hh_u32)CShapeDescriptor::ShapeCollection);
	return static_cast<CShapeDescriptor::EShapeType>(ueShapeType);
}

namespace
{
	const char		*_ResolveAttribSamplerShapeNodeName(CShapeDescriptor::EShapeType shapeType)
	{
		switch (shapeType)
		{
		case PopcornFX::CShapeDescriptor::ShapeBox:
			return "CShapeDescriptor_Box";
		case PopcornFX::CShapeDescriptor::ShapeSphere:
			return "CShapeDescriptor_Sphere";
		case PopcornFX::CShapeDescriptor::ShapeComplexEllipsoid:
			return "CShapeDescriptor_ComplexEllipsoid";
		case PopcornFX::CShapeDescriptor::ShapeCylinder:
			return "CShapeDescriptor_Cylinder";
		case PopcornFX::CShapeDescriptor::ShapeCapsule:
			return "CShapeDescriptor_Capsule";
		case PopcornFX::CShapeDescriptor::ShapeCone:
			return "CShapeDescriptor_Cone";
		case PopcornFX::CShapeDescriptor::ShapePlane:
			return "CShapeDescriptor_Plane";
		case PopcornFX::CShapeDescriptor::ShapeMesh:
			return "CShapeDescriptor_Mesh";
		case PopcornFX::CShapeDescriptor::ShapeCollection:
			return "CShapeDescriptor_Collection";
		default:
			break;
		}
		HH_ASSERT_NOT_REACHED();
		return "CParticleSamplerShape";
	}
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

// static
EPopcornFXAttributeSamplerType::Type		ResolveAttribSamplerType(const PopcornFX::CParticleSampler *sampler)
{
	if (sampler == null)
		return EPopcornFXAttributeSamplerType::None;
	if (sampler->GetSamplerDefaultDescriptor() != null)
		return ResolveAttribSamplerType(*(sampler->GetSamplerDefaultDescriptor().Get()));

	// Does not necessarily mean that it is to be discarded. It could be a skinned mesh
	if (PopcornFX::HBO::Cast<const PopcornFX::CParticleSamplerShape>(sampler) != null)
		return EPopcornFXAttributeSamplerType::Shape;
	return EPopcornFXAttributeSamplerType::None;
}

//----------------------------------------------------------------------------

// static
EPopcornFXAttributeSamplerType::Type		ResolveAttribSamplerType(const PopcornFX::CParticleSamplerDescriptor &desc)
{
	const hh_u32		samplerId = desc.SamplerTypeID();
	if (samplerId == PopcornFX::CParticleSamplerDescriptor_Shape::SamplerTypeID())
		return EPopcornFXAttributeSamplerType::Shape;
	else if (samplerId == PopcornFX::CParticleSamplerDescriptor_Curve::SamplerTypeID())
		return EPopcornFXAttributeSamplerType::Curve;
	else if (samplerId == PopcornFX::CParticleSamplerDescriptor_Image::SamplerTypeID())
		return EPopcornFXAttributeSamplerType::Image;
	else if (samplerId == PopcornFX::CParticleSamplerDescriptor_Text::SamplerTypeID())
		return EPopcornFXAttributeSamplerType::Text;
	//HH_ASSERT_NOT_REACHED();
	return EPopcornFXAttributeSamplerType::None;
}

//----------------------------------------------------------------------------

const char		*ResolveAttribSamplerShapeNodeName(EPopcornFXAttribSamplerShapeType::Type shapeType)
{
	return _ResolveAttribSamplerShapeNodeName((CShapeDescriptor::EShapeType)ToPkShapeType(shapeType));
}

//----------------------------------------------------------------------------

const char		*ResolveAttribSamplerNodeName(const PopcornFX::CParticleSampler *sampler, EPopcornFXAttributeSamplerType::Type samplerType)
{
	if (!HH_VERIFY(sampler != null))
		return null;

	// Same as for ResolveAttribSamplerType, no need to discard everything that doesn't have a default descriptor
	const PopcornFX::CParticleSamplerDescriptor	*desc = sampler->GetSamplerDefaultDescriptor().Get();
	switch (samplerType)
	{
		case	EPopcornFXAttributeSamplerType::Shape:
		{
			if (sampler->GetSamplerDefaultDescriptor() == null)
				return "CParticleSamplerShape";
			const PopcornFX::CParticleSamplerDescriptor_Shape	*shapeDesc = static_cast<const PopcornFX::CParticleSamplerDescriptor_Shape*>(desc);
			if (!HH_VERIFY(shapeDesc->m_Shape != null))
				return "CParticleSamplerShape";
			return _ResolveAttribSamplerShapeNodeName(shapeDesc->m_Shape->ShapeType());
		}
		case	EPopcornFXAttributeSamplerType::Curve:
			return "CParticleSamplerCurve";
		case	EPopcornFXAttributeSamplerType::Image:
			return "CParticleSamplerTexture";
		case	EPopcornFXAttributeSamplerType::Text:
			return "CParticleSamplerText";
		default:
			HH_ASSERT_NOT_REACHED();
			return null;
	}
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

void	FPopcornFXAttributeDesc::Reset(const PopcornFX::CParticleAttributeDeclaration *decl)
{
	if (decl != null)
	{
		m_AttributeFName = FName(ANSI_TO_TCHAR(decl->AttributeName().Data()));
		m_AttributeType = decl->AttributeType();
	}
	else
		Reset();
}

//----------------------------------------------------------------------------

void	FPopcornFXSamplerDesc::Reset(const PopcornFX::CParticleSampler *sampler)
{
	if (sampler != null)
	{
		m_SamplerFName = FName(ANSI_TO_TCHAR(sampler->SamplerName().Data()));
		m_SamplerType = ResolveAttribSamplerType(sampler);
	}
	else
		Reset();
}

//----------------------------------------------------------------------------

UPopcornFXAttributeSampler		*FPopcornFXSamplerDesc::ResolveAttributeSampler(AActor *fallbackActor, UObject *enableLogForOwner) const
{
	const bool		validCompProperty = m_AttributeSamplerComponentProperty.IsValid() && !m_AttributeSamplerComponentProperty.IsNone();
	AActor			*parentActor = m_AttributeSamplerActor == null ? fallbackActor : m_AttributeSamplerActor;
	if (parentActor == null && !validCompProperty)
	{
		// nothing to do here
		return null;
	}
	if (!HH_VERIFY_MESSAGE(parentActor != null, "AttributeSampler is set but no parent Actor: should not happen"))
		return null;

	UPopcornFXAttributeSampler		*attrib = null;
	if (validCompProperty)
	{
		UObjectPropertyBase		*prop = FindField<UObjectPropertyBase>(parentActor->GetClass(), m_AttributeSamplerComponentProperty);
		if (prop != null)
			attrib = Cast<UPopcornFXAttributeSampler>(prop->GetObjectPropertyValue_InContainer(parentActor));
	}
	else
	{
		attrib = Cast<UPopcornFXAttributeSampler>(parentActor->GetRootComponent());
	}
	if (attrib == null)
	{
		const bool		userSpecifedSomething = (m_AttributeSamplerActor != null || validCompProperty);
		if (enableLogForOwner != null && userSpecifedSomething) // do not log for default attrib sampler
		{
			UE_LOG(LogPopcornFXAttributeList, Warning,
				TEXT("Could not find component 'UPopcornFXAttributeSampler %s.%s' for attribute sampler '%s' in '%s'"),
				*parentActor->GetName(), (m_AttributeSamplerComponentProperty.IsValid() ? *m_AttributeSamplerComponentProperty.ToString() : TEXT("RootComponent")),
				*SamplerName(), *enableLogForOwner->GetFullName());
		}
		return null;
	}
	return attrib;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

UPopcornFXAttributeList::UPopcornFXAttributeList(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, m_FileVersionId(0)
	, m_AttributesContainer(null)
{
	SetFlags(RF_Transactional);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeList::CheckDataIntegrity() const
{
	bool		ok = true;
	ok &= HH_VERIFY(m_AttributesRawData.Num() == m_Attributes.Num() * kAttributeSize);
	if (m_AttributesContainer != null)
	{
		ok &= HH_VERIFY(m_AttributesContainer->AttribCount() == m_Attributes.Num());
		ok &= HH_VERIFY(m_AttributesContainer->SamplerCount() == m_Samplers.Num());
	}
	return ok;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeList::Valid() const
{
	return m_Effect != null;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeList::IsUpToDate(UPopcornFXEffect *effect) const
{
	if (effect == null)
	{
		HH_ASSERT(AttributeCount() == 0);
		HH_ASSERT(SamplerCount() == 0);
		HH_ASSERT(m_AttributesRawData.Num() == 0);
		HH_ASSERT(CheckDataIntegrity());
		return m_Effect == null && m_FileVersionId == 0;
	}
	if (effect != m_Effect)
		return false;
	if (effect->FileVersionId() != m_FileVersionId)
		return false;
	if (effect->IsTheDefaultAttributeList(this))
	{
		HH_ASSERT(CheckDataIntegrity());
		return true;
	}
	HH_ONLY_IF_ASSERTS(const UPopcornFXAttributeList		*defAttribs = effect->GetDefaultAttributeList());
	HH_ASSERT(this != defAttribs); // checked with IsTheDefaultAttributeList
	HH_ASSERT(defAttribs->IsUpToDate(effect));
	HH_ASSERT(defAttribs->AttributeCount() == AttributeCount());
	HH_ASSERT(defAttribs->SamplerCount() == SamplerCount());
	HH_ASSERT(CheckDataIntegrity());
	// @TOD may other cases
	return true;
}

//----------------------------------------------------------------------------

UPopcornFXAttributeList::~UPopcornFXAttributeList()
{
	if (m_AttributesContainer != null)
	{
		HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeList::~UPopcornFXAttributeList", POPCORNFX_UE_PROFILER_COLOR);
#if 0
		PopcornFX::CLog::Log(HH_INFO,
			"UPopcornFXAttributeList::~UPopcornFXAttributeList DESTROY  %p cont: %p **%p",
				this,
				m_AttributesContainer,
				&m_AttributesContainer);
				//TCHAR_TO_ANSI(*GetPathName()));
#endif
		m_AttributesContainer->Destroy();
		m_AttributesContainer = null;
	}
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeList::IsEmpty() const
{
	return m_Attributes.Num() == 0 && m_Samplers.Num() == 0;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::Clean()
{
	DBG_HERE();
#if WITH_EDITOR
	Modify();
#endif

	m_Effect = null;
	m_FileVersionId = 0;
#if WITH_EDITOR
	bool		hasChanged = (m_Attributes.Num() != 0 || m_Samplers.Num() != 0 || m_AttributesRawData.Num() != 0);
#endif // WITH_EDITOR
	m_Attributes.Empty(m_Attributes.Num());
	m_Samplers.Empty(m_Samplers.Num());
	m_AttributesRawData.Empty(m_AttributesRawData.Num());
	_DestroyAttributesContainer();
	HH_ASSERT(CheckDataIntegrity());

#if WITH_EDITOR
	//if (hasChanged)
	//{
	//	MarkPackageDirty();
	//	if (GetOuter())
	//		GetOuter()->MarkPackageDirty();
	//}
#endif // WITH_EDITOR
}

//----------------------------------------------------------------------------

int32	UPopcornFXAttributeList::FindAttributeIndex(FName fname) const
{
	HH_ASSERT(CheckDataIntegrity());
	for (int32 attri = 0; attri < m_Attributes.Num(); ++attri)
	{
		if (m_Attributes[attri].m_AttributeFName == fname)
			return attri;
	}
	return -1;
}

//----------------------------------------------------------------------------

int32	UPopcornFXAttributeList::FindSamplerIndex(FName fname) const
{
	HH_ASSERT(CheckDataIntegrity());
	for (int32 sampleri = 0; sampleri < m_Samplers.Num(); ++sampleri)
	{
		if (m_Samplers[sampleri].m_SamplerFName == fname)
			return sampleri;
	}
	return -1;
}

//----------------------------------------------------------------------------

const UPopcornFXAttributeList		*UPopcornFXAttributeList::GetDefaultAttribteList(UPopcornFXEffect *effect) const
{
	if (!HH_VERIFY(IsUpToDate(effect)))
		return null;
	if (effect == null)
		return null;
	HH_ASSERT(CheckDataIntegrity());
	const UPopcornFXAttributeList	*defAttribs = effect->GetDefaultAttributeList();
	if (!HH_VERIFY(defAttribs != null))
		return null;
	return defAttribs;
}

//----------------------------------------------------------------------------

const FPopcornFXAttributeDesc	*UPopcornFXAttributeList::GetAttributeDesc(uint32 attributeId) const
{
	HH_ASSERT(CheckDataIntegrity());
	if (!HH_VERIFY(int32(attributeId) < m_Attributes.Num()))
		return null;
	return &(m_Attributes[attributeId]);
}

//----------------------------------------------------------------------------

const PopcornFX::CParticleAttributeDeclaration		*UPopcornFXAttributeList::GetAttributeDeclaration(UPopcornFXEffect *effect, uint32 attributeId) const
{
	if (!HH_VERIFY(int32(attributeId) < m_Attributes.Num()))
		return null;
	const UPopcornFXAttributeList				*defAttribs = GetDefaultAttribteList(effect);
	if (defAttribs == null)
		return null;
	const PopcornFX::CParticleAttributeList		*attrList = effect->AttributeList().Get();
	if (!HH_VERIFY(attrList != null))
		return null;
	HH_ASSERT(attrList->UniqueAttributeList().Count() == m_Attributes.Num());
	return attrList->UniqueAttributeList()[attributeId];
}

//----------------------------------------------------------------------------

const FPopcornFXSamplerDesc		*UPopcornFXAttributeList::GetSamplerDesc(uint32 samplerId) const
{
	HH_ASSERT(CheckDataIntegrity());
	if (!HH_VERIFY(int32(samplerId) < m_Samplers.Num()))
		return null;
	return &(m_Samplers[samplerId]);
}

//----------------------------------------------------------------------------

const PopcornFX::CParticleSampler	*UPopcornFXAttributeList::GetParticleSampler(UPopcornFXEffect *effect, uint32 samplerId) const
{
	HH_ASSERT(CheckDataIntegrity());
	if (!HH_VERIFY(int32(samplerId) < m_Samplers.Num()))
		return null;
	const UPopcornFXAttributeList			*defAttribs = GetDefaultAttribteList(effect);
	if (defAttribs == null)
		return null;
	const PopcornFX::CParticleAttributeList		*attrList = effect->AttributeList().Get();
	if (!HH_VERIFY(attrList != null))
		return null;
	HH_ASSERT(attrList->UniqueSamplerList().Count() == m_Samplers.Num());
	return attrList->UniqueSamplerList()[samplerId];
}

//----------------------------------------------------------------------------

typedef PopcornFX::TMemoryView<PopcornFX::CParticleAttributeDeclaration const * const> CMVAttributes;
typedef PopcornFX::TMemoryView<PopcornFX::CParticleSampler const * const> CMVSamplers;

void	UPopcornFXAttributeList::SetupDefault(UPopcornFXEffect *effect, bool force)
{
	DBG_HERE();

	if (!force && IsUpToDate(effect))
		return;

#if WITH_EDITOR
	Modify();
#endif

	if (effect == null)
	{
		Clean();
		return;
	}

	m_Effect = effect;
	m_FileVersionId = effect->FileVersionId();
	_DestroyAttributesContainer();

	const PopcornFX::PCParticleAttributeList	&attrListPtr = effect->AttributeList();
	if (attrListPtr == null || *(attrListPtr->DefaultAttributes()) == null)
	{
		Clean();
		m_Effect = effect;
		m_FileVersionId = effect->FileVersionId();
		return;
	}

	bool		hasChanged = false;

	const PopcornFX::CParticleAttributeList		&attrList = *(attrListPtr.Get());

	const CMVAttributes		attrs = attrList.UniqueAttributeList();
	const CMVSamplers		samplers = attrList.UniqueSamplerList();
	const int32				attrCount = attrs.Count();

#if WITH_EDITOR
	hasChanged |= (m_Attributes.Num() != attrs.Count() || m_Samplers.Num() != samplers.Count());
#endif // WITH_EDITOR

	m_Attributes.SetNum(attrCount);
	for (int32 attri = 0; attri < attrCount; ++attri)
	{
#if WITH_EDITOR
		if (!hasChanged)
		{
			HH_ASSERT(attrs[attri] != null); // UniqueAttributeList() removes null attrs
			hasChanged = m_Attributes[attri].AttributeName() != ANSI_TO_TCHAR(attrs[attri]->AttributeName().Data());
		}
		// @TODO check type ?
#endif
		m_Attributes[attri].Reset(attrs[attri]);
	}

	m_Samplers.SetNum(samplers.Count());
	for (int32 sampleri = 0; sampleri < int32(samplers.Count()); ++sampleri)
	{
#if WITH_EDITOR
		if (!hasChanged)
		{
			if (samplers[sampleri] == null) // !! UniqueSamplerList() does NOT remove null samplers !!
				hasChanged = m_Samplers[sampleri].Valid(); // null <=> FName_None // so has changed if IsValid()
			else
				hasChanged = (m_Samplers[sampleri].SamplerName() != ANSI_TO_TCHAR(samplers[sampleri]->SamplerName().Data()));
			// @TODO check type ?
		}
#endif
		m_Samplers[sampleri].Reset(samplers[sampleri]);
	}

	const PopcornFX::SAttributesContainer		*defContainer = *(attrList.DefaultAttributes());
	HH_ASSERT(defContainer != null);

	const uint32	attribBytes = defContainer->Attributes().CoveredBytes();
	m_AttributesRawData.SetNumUninitialized(attribBytes);
	if (attribBytes > 0)
	{
#if WITH_EDITOR
		hasChanged = true; // @TODO better check
#endif // WITH_EDITOR
		PopcornFX::Mem::Copy(m_AttributesRawData.GetData(), defContainer->Attributes().Data(), attribBytes);
	}

	HH_ASSERT(CheckDataIntegrity());

#if WITH_EDITOR
	//if (hasChanged)
	//{
	//	MarkPackageDirty();
	//	if (GetOuter())
	//		GetOuter()->MarkPackageDirty();
	//}
#endif // WITH_EDITOR
}

//----------------------------------------------------------------------------

namespace
{
	void	CopyAttributeRawData(TArray<uint8> &dst, uint32 dstAttribIndex, const TArray<uint8> &src, uint32 srcAttribIndex)
	{
		reinterpret_cast<PopcornFX::SAttributesContainer_SAttrib&>(dst[dstAttribIndex * kAttributeSize]) = reinterpret_cast<const PopcornFX::SAttributesContainer_SAttrib&>(src[srcAttribIndex * kAttributeSize]);
	}

	void	SwapAttributeRawData(TArray<uint8> &dst, uint32 dstAttribIndex, TArray<uint8> &src, uint32 srcAttribIndex)
	{
		FMemory::Memswap(&dst[dstAttribIndex * kAttributeSize], &src[srcAttribIndex * kAttributeSize], kAttributeSize);
	}
} // namespace

bool	UPopcornFXAttributeList::Prepare(UPopcornFXEffect *effect, bool force)
{
	DBG_HERE();

	HH_ASSERT(CheckDataIntegrity());

	if (!force && IsUpToDate(effect))
		return true;

#if WITH_EDITOR
	Modify();
#endif

	if (effect == null)
	{
		Clean();
		return false;
	}

	const UPopcornFXAttributeList		*refAttrList = effect->GetDefaultAttributeList();
	if (!HH_VERIFY(refAttrList != null)) // should not happen ?
	{
		Clean();
		m_Effect = effect;
		m_FileVersionId = 0; // try next time to reload
		return false;
	}

	if (m_Effect != effect && IsEmpty())
	{
		CopyFrom(refAttrList);
		return true;
	}

	HH_ASSERT(FPopcornFXPlugin::IsMainThread());

	HH_ONLY_IF_ASSERTS(
		{
			UPopcornFXEmitterComponent	*cpnt = Cast<UPopcornFXEmitterComponent>(GetOuter());
			if (cpnt != null)
			{
				HH_ASSERT(m_Owner == null || cpnt == m_Owner);
				HH_ASSERT(!cpnt->IsEmitterStarted());
				if (cpnt->_GetParticleScene() != null &&
					cpnt->_GetParticleScene()->Unsafe_ParticleMediumCollection() != null)
				{
					HH_ASSERT(!cpnt->_GetParticleScene()->Unsafe_ParticleMediumCollection()->UpdatePending());
				}
				//cpnt->TerminateEmitter();
			}
		});

#if 0	// This log happens too often to be useful !
	UE_LOG(LogPopcornFXAttributeList, Warning, TEXT("AttributeList \"%s\":%s re-loading attributes from \"%s\""),
		*GetFullName(),
		(force ? TEXT(" FORCE") : TEXT("")),
		*effect->GetFullName());
#endif

	m_Effect = effect;
	m_FileVersionId = effect->FileVersionId();
	_DestroyAttributesContainer();

	TArray<FPopcornFXAttributeDesc>			*attrs = &m_Attributes;
	TArray<FPopcornFXSamplerDesc>			*samplers = &m_Samplers;
	TArray<uint8>							*rawData = &m_AttributesRawData;

	const TArray<FPopcornFXAttributeDesc>	*refAttrs = &refAttrList->m_Attributes;
	const TArray<FPopcornFXSamplerDesc>		*refSamplers = &refAttrList->m_Samplers;
	const TArray<uint8>						*refRawData = &refAttrList->m_AttributesRawData;

	bool		attrsChanged = false;

	// Re-match Attributes
	if (refRawData->Num() == 0)
	{
		attrsChanged |= !(attrs->Num() == 0);
		attrs->Empty();
		rawData->Empty();
	}
	else if (attrs->Num() == 0)
	{
		attrsChanged = true;
		*attrs = *refAttrs;
		*rawData = *refRawData;
	}
	else
	{
		attrs->Reserve(refAttrs->Num());
		rawData->Reserve(refRawData->Num());

		int32		attri = 0;
		for (; attri < refAttrs->Num() && attri < attrs->Num(); ++attri)
		{
			const FPopcornFXAttributeDesc	&refAttr = (*refAttrs)[attri];
			FPopcornFXAttributeDesc			*attr = &((*attrs)[attri]);

			HH_ASSERT(refAttr.Valid());
			if (attr->AttributeFName() != refAttr.AttributeFName())
			{
				attrsChanged = true;
				PopcornFX::CGuid		found;
				for (int32 findAttri = attri + 1; findAttri < attrs->Num(); ++findAttri)
				{
					FPopcornFXAttributeDesc		&findAttr = (*attrs)[findAttri];
					if (findAttr.AttributeFName() == refAttr.AttributeFName())
					{
						found = findAttri;
						break;
					}
				}
				if (!found.Valid())
				{
					// (Can result to superfluous copies, but, here, dont care, and prefer simpler code)

					// Copy the refAttr/default to the back
					// and make it the found one...
					found = attrs->Add(refAttr);
					attr = &((*attrs)[attri]); // minds Add() realloc
					HH_ASSERT(rawData->Num() == found * kAttributeSize); // CheckDataIntegrity() should have asserted ? no ?
					rawData->SetNumUninitialized(rawData->Num() + 1 * kAttributeSize); // Push
					CopyAttributeRawData(*rawData, found, *refRawData, attri);
				}
				attrs->SwapMemory(attri, found);
				SwapAttributeRawData(*rawData, attri, *rawData, found);
				HH_ASSERT(attr->Valid() && attr->AttributeFName() == refAttr.AttributeFName());
			}

			if (attr->AttributeBaseTypeID() != refAttr.AttributeBaseTypeID())
			{
				attrsChanged = true;
				const PopcornFX::CBaseTypeTraits		&refTraits = PopcornFX::CBaseTypeTraits::Traits((PopcornFX::EBaseTypeID)refAttr.AttributeBaseTypeID());
				const PopcornFX::CBaseTypeTraits		&traits = PopcornFX::CBaseTypeTraits::Traits((PopcornFX::EBaseTypeID)attr->AttributeBaseTypeID());
				// If attribute changed float <-> int, reset value to default
				if (refTraits.IsFp != traits.IsFp)
				{
					CopyAttributeRawData(*rawData, attri, *refRawData, attri);
				}
				attr->m_AttributeType = refAttr.m_AttributeType;
			}

			check(attr->ExactMatch(refAttr)); // Name and Type must match now
		}

		// Removes removed attributes OR Sets new one to default
		attrs->SetNum(refAttrs->Num());
		rawData->SetNumUninitialized(refRawData->Num());
		for (; attri < refAttrs->Num(); ++attri)
		{
			const FPopcornFXAttributeDesc	&refAttr = (*refAttrs)[attri];
			FPopcornFXAttributeDesc			&attr = (*attrs)[attri];
			attr = refAttr;
			CopyAttributeRawData(*rawData, attri, *refRawData, attri);
		}
	}

	bool	samplersChanged = false;

	// Re-match Samplers
	if (refSamplers->Num() == 0)
	{
		samplersChanged |= !(samplers->Num() == 0);
		samplers->Empty();
	}
	else if (samplers->Num() == 0)
	{
		samplersChanged = true;
		*samplers = *refSamplers;
	}
	else
	{
		samplers->Reserve(refSamplers->Num());

		int32	sampleri = 0;
		for (; sampleri < refSamplers->Num() && sampleri < samplers->Num(); ++sampleri)
		{
			const FPopcornFXSamplerDesc		&refSampler = (*refSamplers)[sampleri];
			FPopcornFXSamplerDesc			*sampler = &((*samplers)[sampleri]);

			HH_ASSERT(refSampler.ValueIsEmpty());

			if (sampler->SamplerFName() != refSampler.SamplerFName())
			{
				samplersChanged = true;
				PopcornFX::CGuid		found;
				for (int32 foundSampleri = sampleri + 1; foundSampleri < samplers->Num(); ++foundSampleri)
				{
					FPopcornFXSamplerDesc	&foundSampler = (*samplers)[foundSampleri];
					if (foundSampler.SamplerFName() == refSampler.SamplerFName())
					{
						found = foundSampleri;
						break;
					}
				}
				if (!found.Valid())
				{
					// (Can result to superfluous copies, but, here, dont care, and prefer simpler code)
					found = samplers->Add(refSampler);
					sampler = &((*samplers)[sampleri]); // minds Add() realloc
				}
				samplers->SwapMemory(sampleri, found);
				HH_ASSERT(sampler->Valid() && sampler->SamplerFName() == refSampler.SamplerFName());
			}

			// If type missmatch, just set to default (ResetValue())
			if (sampler->SamplerType() != refSampler.SamplerType())
			{
				samplersChanged = true;
				*sampler = refSampler;
			}

			HH_ASSERT(sampler->ExactMatch(refSampler));
		}

		samplers->SetNum(refSamplers->Num());
		for (; sampleri < refSamplers->Num(); ++sampleri)
		{
			const FPopcornFXSamplerDesc	&refSampler = (*refSamplers)[sampleri];
			FPopcornFXSamplerDesc		&sampler = (*samplers)[sampleri];
			HH_ASSERT(refSampler.ValueIsEmpty());
			sampler = refSampler;
		}
	}

	HH_ASSERT(CheckDataIntegrity());

#if WITH_EDITOR
	if (attrsChanged || samplersChanged)
	{
		ForceSetPackageDiry(this);
	}
#endif

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::CopyFrom(const UPopcornFXAttributeList *other, AActor *patchParentActor)
{
	DBG_HERE();

#if WITH_EDITOR
	Modify();
#endif

	if (!HH_VERIFY(other != null))
	{
		Clean();
		return;
	}

	HH_ASSERT(other->CheckDataIntegrity());

	m_Effect = other->m_Effect;
	m_FileVersionId = other->m_FileVersionId;
	m_Attributes = other->m_Attributes;
	m_Samplers = other->m_Samplers;
	m_AttributesRawData = other->m_AttributesRawData;
	_DestroyAttributesContainer();

	if (patchParentActor != null)
	{
		for (int32 sampleri = 0; sampleri < m_Samplers.Num(); ++sampleri)
		{
			FPopcornFXSamplerDesc		&desc = m_Samplers[sampleri];
			if (desc.m_AttributeSamplerActor == null && desc.m_AttributeSamplerComponentProperty.IsValid() && !desc.m_AttributeSamplerComponentProperty.IsNone())
			{
				desc.m_AttributeSamplerActor = patchParentActor;
			}
		}
	}

#if WITH_EDITOR
	//MarkPackageDirty();
	//if (GetOuter())
	//	GetOuter()->MarkPackageDirty();
#endif

	HH_ASSERT(CheckDataIntegrity());

}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::RestoreAttributesFromCachedRawData(const TArray<uint8> &rawData)
{
	const hh_u32	coveredBytes = rawData.Num();
	if (!HH_VERIFY(CheckDataIntegrity()) &&
		!HH_VERIFY(m_AttributesRawData.Num() == coveredBytes))
		return;
	PopcornFX::Mem::Copy(m_AttributesRawData.GetData(), rawData.GetData(), coveredBytes);
	if (m_AttributesContainer != null)
		PopcornFX::Mem::Copy(m_AttributesContainer->Attributes().Data(), rawData.GetData(), coveredBytes);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::ResetToDefaultValues(UPopcornFXEffect *effect)
{
	DBG_HERE();

	if (!HH_VERIFY(Valid()))
		return;

#if WITH_EDITOR
	Modify();
#endif

	HH_ASSERT(CheckDataIntegrity());

	const UPopcornFXAttributeList		*defAttribs = GetDefaultAttribteList(effect);
	if (defAttribs == null)
		return;

	const TArray<uint8>			&defRawData = defAttribs->m_AttributesRawData;
	HH_ASSERT(m_AttributesRawData.Num() == defRawData.Num());
	m_AttributesRawData = defRawData;
	if (m_AttributesContainer != null)
	{
		if (HH_VERIFY(defRawData.Num() > 0) &&
			HH_VERIFY(m_AttributesContainer->Attributes().CoveredBytes() == defRawData.Num()))
		{
			PopcornFX::Mem::Copy(m_AttributesContainer->Attributes().Data(), defRawData.GetData(), defRawData.Num());
		}
	}

	for (int32 i = 0; i < m_Samplers.Num(); ++i)
		m_Samplers[i].ResetValue();

#if WITH_EDITOR
	//MarkPackageDirty();
	//if (GetOuter())
	//	GetOuter()->MarkPackageDirty();
#endif

}

//----------------------------------------------------------------------------

PopcornFX::SAttributesContainer*const*	UPopcornFXAttributeList::AttributesContainer()
{
	//DBG_HERE();

	HH_ASSERT(CheckDataIntegrity());
	if (m_AttributesContainer == null)
	{
		HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeList::AttributesContainer Create", POPCORNFX_UE_PROFILER_COLOR);

		m_AttributesContainer = PopcornFX::SAttributesContainer::Create(m_Attributes.Num(), m_Samplers.Num());

#if 0
		PopcornFX::CLog::Log(HH_INFO,
			"UPopcornFXAttributeList::AttributesContainer CREATE %p cont: %p **%p %s",
				this,
				m_AttributesContainer,
				&m_AttributesContainer,
				TCHAR_TO_ANSI(*GetPathName()));
#endif

		HH_ASSERT(CheckDataIntegrity());
		if (HH_VERIFY(m_AttributesContainer != null))
		{
			const hh_u32	attributeBytes = m_AttributesContainer->Attributes().CoveredBytes();
			if (HH_VERIFY(attributeBytes == m_AttributesRawData.Num()))
			{
				PopcornFX::Mem::Copy(m_AttributesContainer->Attributes().Data(), m_AttributesRawData.GetData(), attributeBytes);
			}
		}
	}
	else
	{
		// should have been checked in CheckDataIntegrity
	}
	return &m_AttributesContainer;
}

void	UPopcornFXAttributeList::_DestroyAttributesContainer()
{
	DBG_HERE();
	if (m_AttributesContainer != null)
	{
		HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeList::_DestroyAttributesContainer", POPCORNFX_UE_PROFILER_COLOR);

		HH_ONLY_IF_ASSERTS(
			if (!IsEmpty())
			{
				UPopcornFXEmitterComponent	*cpnt = Cast<UPopcornFXEmitterComponent>(GetOuter());
				if (cpnt != null && cpnt->IsValidLowLevel())
				{
					HH_ASSERT(m_Owner == null || cpnt == m_Owner);
					HH_RELEASE_ASSERT(!cpnt->IsEmitterStarted());
					//cpnt->TerminateEmitter();
				}
			}
		);

#if 0
		PopcornFX::CLog::Log(HH_INFO,
			"UPopcornFXAttributeList::_DestroyAttributesContainer DESTROY %p cont: %p **%p %s",
				this,
				m_AttributesContainer,
				&m_AttributesContainer,
				TCHAR_TO_ANSI(*GetPathName()));
#endif

		m_AttributesContainer->Destroy();
		m_AttributesContainer = null;
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::PostLoad()
{
	DBG_HERE();
	m_Owner = null;
	//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- PostLoad 1           --- %3d %3d %3d --- %s"), m_AttributesRawData.Num(), m_Attributes.Num(), m_Samplers.Num(), *GetFullName());
	Super::PostLoad();
	HH_ASSERT(CheckDataIntegrity());
	//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- PostLoad 2           --- %3d %3d %3d --- %s"), m_AttributesRawData.Num(), m_Attributes.Num(), m_Samplers.Num(), *GetFullName());
	//Prepare(m_Effect.Get());
}

void	UPopcornFXAttributeList::PostInitProperties()
{
	DBG_HERE();
	m_Owner = null;
	Super::PostInitProperties();
	//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- PostInitProperties --- %3d %3d %3d --- %s"), m_AttributesRawData.Num(), m_Attributes.Num(), m_Samplers.Num(), *GetFullName());
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 13)
void	UPopcornFXAttributeList::PreSave(const class ITargetPlatform* TargetPlatform)
#else
void	UPopcornFXAttributeList::PreSave()
#endif
{
	DBG_HERE();
	HH_ASSERT(CheckDataIntegrity());
	// make sure m_AttributesContainer up to date
	//if (m_Effect != null)
	//	Prepare(m_Effect.Get());
	HH_ASSERT(CheckDataIntegrity());

#if (ENGINE_MINOR_VERSION >= 13)
	Super::PreSave(TargetPlatform);
#else
	Super::PreSave();
#endif
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::Serialize(FArchive& Ar)
{
	DBG_HERE();
//	HH_ASSERT(CheckDataIntegrity());

	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FPopcornFXCustomVersion::GUID);

	//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- ATTRLIST Serizalie save %p %s --- %s"), this, (Ar.IsSaving() ? "saving" : "restoring"), *GetFullName());

	const int32 version = Ar.CustomVer(FPopcornFXCustomVersion::GUID);
	//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- Serialize load:%d %d --- %3d %3d %3d --- %s"), Ar.IsLoading() == 1, version, m_AttributesRawData.Num(), m_Attributes.Num(), m_Samplers.Num(), *GetFullName());

	if (version < FPopcornFXCustomVersion::BeforeCustomVersionWasAdded)
	{
		TArray<uint8>	attribs;
		int32			attribCount = 0;
		int32			samplerCount = 0;

		Ar << attribs;
		Ar << samplerCount;
		Ar << attribCount;

		//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- Serialize %d data     --- %3d %3d %3d --- %s"), Ar.IsLoading() == 1, attribs.Num(), attribCount, samplerCount, *GetFullName());
		//UE_LOG(LogPopcornFXAttributeList, Log, TEXT("--- Serialize %d OLD     --- %3d %3d %3d --- %s"), Ar.IsLoading() == 1, attribs.Num(), attribCount, samplerCount, *GetFullName());
		if (Ar.IsLoading())
		{
			if (attribs.Num() > 0)
				m_AttributesRawData = attribs;
			if (!HH_VERIFY(m_AttributesRawData.Num() == m_Attributes.Num() * kAttributeSize))
			{
				//UE_LOG(LogPopcornFXAttributeList, Warning, TEXT("Attribute values seems corrupted, attribute will be reset for '%s'"), *GetFullName());
				m_Effect = null;
				m_FileVersionId = 0;
				m_Attributes.Empty(m_Attributes.Num());
				m_Samplers.Empty(m_Samplers.Num());
				m_AttributesRawData.Empty(m_AttributesRawData.Num());
			}
		}
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::BeginDestroy()
{
	DBG_HERE();
	// Clean(); // WTF no
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::PostEditUndo()
{
	if (m_AttributesContainer != null &&
		HH_VERIFY(m_AttributesRawData.Num() == m_AttributesContainer->AttribCount() * kAttributeSize))
	{
		PopcornFX::Mem::Copy(m_AttributesContainer->Attributes().Data(), m_AttributesRawData.GetData(), m_AttributesContainer->Attributes().CoveredBytes());
	}
	Super::PostEditUndo();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	DBG_HERE();
	HH_ASSERT(CheckDataIntegrity());
	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::Scene_PreUpdate(CParticleScene *scene, UPopcornFXEmitterComponent *emitter, float deltaTime, enum ELevelTick tickType)
{
	HH_ASSERT(emitter != null);
	HH_ASSERT(m_Owner == null || emitter == m_Owner);
	m_Owner = emitter;

	for (int32 sampleri = 0; sampleri < m_Samplers.Num(); ++sampleri)
	{
		FPopcornFXSamplerDesc			&desc = m_Samplers[sampleri];
		if (desc.m_NeedUpdate)
		{
			UPopcornFXAttributeSampler		*attribSampler = desc.ResolveAttributeSampler(emitter->GetOwner(), null/*dont log each frame*/);
			if (attribSampler != null)
			{
				attribSampler->_AttribSampler_PreUpdate(scene, deltaTime, tickType);
			}
		}
	}
}

//----------------------------------------------------------------------------

#if WITH_EDITOR
void	UPopcornFXAttributeList::AttributeSamplers_IndirectSelectedThisTick(UPopcornFXEmitterComponent *emitter) const
{
	for (int32 sampleri = 0; sampleri < m_Samplers.Num(); ++sampleri)
	{
		const FPopcornFXSamplerDesc		&desc = m_Samplers[sampleri];
		UPopcornFXAttributeSampler		*attribSampler = desc.ResolveAttributeSampler(emitter->GetOwner(), null/*dont log at each frame*/);
		if (attribSampler != null)
			attribSampler->_AttribSampler_IndirectSelectedThisTick();
	}
}
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

void	UPopcornFXAttributeList::_RefreshAttributeSamplers(UPopcornFXEmitterComponent *emitter, bool reload)
{
	HH_ASSERT(emitter != null);
	HH_ASSERT(m_Owner == null || emitter == m_Owner);
	m_Owner = emitter;

	if (!HH_VERIFY(emitter != null))
		return;
	UPopcornFXEffect							*effect = emitter->Effect;
	Prepare(effect);

	HH_ASSERT(IsUpToDate(effect));
	if (!HH_VERIFY(effect != null))
		return;

	if (m_Samplers.Num() == 0)
		return;

	AttributesContainer(); // build container ifn

	if (!HH_VERIFY(m_AttributesContainer != null))
		return;

	HH_ASSERT(CheckDataIntegrity());

	if (!HH_VERIFY(m_Samplers.Num() == m_AttributesContainer->Samplers().Count()))
		return;
	const PopcornFX::PCParticleAttributeList	&attrListPtr = effect->AttributeList();
	if (attrListPtr == null || *(attrListPtr->DefaultAttributes()) == null)
		return;
	const PopcornFX::SAttributesContainer	*defCont = *(attrListPtr->DefaultAttributes());
	if (!HH_VERIFY(defCont != null && defCont->Samplers().Count() == m_Samplers.Num()))
		return;

	for (int32 sampleri = 0; sampleri < m_Samplers.Num(); ++sampleri)
	{
		const PopcornFX::SAttributesContainer::SSampler		&defSampler = defCont->Samplers()[sampleri];
		PopcornFX::SAttributesContainer::SSampler			&outSampler = m_AttributesContainer->Samplers()[sampleri];

		FPopcornFXSamplerDesc				&desc = m_Samplers[sampleri];
		desc.m_NeedUpdate = false;

		if (desc.SamplerType() == EPopcornFXAttributeSamplerType::None)
		{
			outSampler = defSampler;
			continue;
		}

		HH_ASSERT(
			defSampler.m_Desc == null ||
			desc.SamplerType() == ResolveAttribSamplerType(*(defSampler.m_Desc)));

		UPopcornFXAttributeSampler			*attribSampler = desc.ResolveAttributeSampler(emitter->GetOwner(), emitter);
		if (attribSampler == null ||
			!HH_VERIFY(desc.SamplerType() == attribSampler->SamplerType()))
		{
			outSampler = defSampler;
			continue;
		}

		PopcornFX::CParticleSamplerDescriptor		*samplerDescriptor = (reload ? null : desc.m_SamplerDescriptor.Get());

		// Dont setup samplers when cooking
		if (!IsRunningCommandlet() && attribSampler != null)
			samplerDescriptor = attribSampler->_AttribSampler_SetupSamplerDescriptor(desc, defSampler.m_Desc.Get());

		if (samplerDescriptor != null)
			outSampler.m_Desc = samplerDescriptor;
		else
		{
			// default value
			outSampler = defSampler;
		}
	}
}

void	UPopcornFXAttributeList::CheckEmitter(const class UPopcornFXEmitterComponent *emitter)
{
	HH_ASSERT(emitter != null);
	HH_ASSERT(m_Owner == null || m_Owner == emitter);
	m_Owner = emitter;
}


//----------------------------------------------------------------------------
