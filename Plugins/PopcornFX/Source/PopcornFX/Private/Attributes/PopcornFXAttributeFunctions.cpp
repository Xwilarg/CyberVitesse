//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAttributeFunctions.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXEmitterComponent.h"
#include "Attributes/PopcornFXAttributeList.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_base_types.h>
#include <hh_particles/include/ps_descriptor.h>

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXAttributeFunctions"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXAttributeFunctions, Log, All);

//----------------------------------------------------------------------------

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#	define	PKUE_ATTRIB_ENABLE_CHECKS		1
#else
#	define	PKUE_ATTRIB_ENABLE_CHECKS		0
#endif

//----------------------------------------------------------------------------

namespace
{
	template <typename _Scalar>
	struct IsFp
	{
		enum { Value = false };
	};

	template <>
	struct IsFp<float>
	{
		enum { Value = true };
	};

	bool			_ResetAttribute(UPopcornFXEmitterComponent *component, int32 _attri)
	{
		uint32		attri(_attri);

		if (!HH_VERIFY(component != null))
			return false;
		UPopcornFXAttributeList		*attrList = component->GetAttributeList();
		if (!HH_VERIFY(attrList != null))
			return false;

		if (attri >= attrList->AttributeCount())
		{
			UE_LOG(LogPopcornFXAttributeFunctions, Warning, TEXT("ResetToDefaultValue: invalid InAttributeIndex %d"), _attri);
			return false;
		}

		const PopcornFX::CParticleAttributeDeclaration	*decl = attrList->GetAttributeDeclaration(component->Effect, attri);
		if (!HH_VERIFY(decl != null))
			return false;

		PopcornFX::SAttributesContainer::SAttrib		attribValue;
		decl->GetDefaultValue(attribValue);

		PopcornFX::SAttributesContainer			*container = *(attrList->AttributesContainerIFP());
		if (container != null)
			container->Attributes()[attri] = attribValue;

		attrList->AttributeRawDataAttributes()[attri] = attribValue;

		return true;
	}

	template <typename _Scalar, hh_u32 _Dim>
	bool			_SetAttribute(UPopcornFXEmitterComponent *component, int32 _attri, const PopcornFX::TVector<_Scalar, _Dim> &value)
	{
		typedef PopcornFX::TVector<_Scalar, _Dim>		Vector;

		uint32		attri(_attri);

		if (!HH_VERIFY(component != null))
			return false;
		UPopcornFXAttributeList		*attrList = component->GetAttributeList();
		if (!HH_VERIFY(attrList != null))
			return false;

		if (attri >= attrList->AttributeCount())
		{
			UE_LOG(LogPopcornFXAttributeFunctions, Warning, TEXT("SetAttributeAs: invalid InAttributeIndex %d (%s)"), _attri, *(component->GetPathName()));
			return false;
		}

		const PopcornFX::CParticleAttributeDeclaration	*decl = attrList->GetAttributeDeclaration(component->Effect, attri);
		if (!HH_VERIFY(decl != null))
			return false;

#if PKUE_ATTRIB_ENABLE_CHECKS
		const PopcornFX::CBaseTypeTraits		&attrTraits = PopcornFX::CBaseTypeTraits::Traits((PopcornFX::EBaseTypeID)decl->AttributeType());
		if (_Dim > attrTraits.VectorDimension ||
			IsFp<_Scalar>::Value != attrTraits.IsFp)
		{
			const char			*inType = (IsFp<_Scalar>::Value ? "Float" : "Int");
			const char			*attrType = (attrTraits.IsFp ? "Float" : "Int");
			UE_LOG(LogPopcornFXAttributeFunctions, Warning,
				TEXT("SetAttributeAs: the Attribute [%d] \"%s\" cannot be set as %s %d: the attribute is %s %d (%s)"),
				attri, ANSI_TO_TCHAR(decl->AttributeName().Data()),
				ANSI_TO_TCHAR(inType), _Dim,
				ANSI_TO_TCHAR(attrType), attrTraits.VectorDimension,
				*(component->GetPathName())
				);
			return false;
		}
#endif

		PopcornFX::SAttributesContainer::SAttrib	attribValue;
		attribValue.m_Data32u[0] = 0;
		attribValue.m_Data32u[1] = 0;
		attribValue.m_Data32u[2] = 0;
		attribValue.m_Data32u[3] = 0;
		HH_STATIC_ASSERT(sizeof(attribValue) >= sizeof(value));
		reinterpret_cast<Vector&>(attribValue) = value;
		decl->ClampToRangeIFN(attribValue);

		PopcornFX::SAttributesContainer		*container = *(attrList->AttributesContainerIFP());
		if (container != null)
			container->Attributes()[attri] = attribValue;

		attrList->AttributeRawDataAttributes()[attri] = attribValue;

		return true;
	}

	template <typename _Scalar, hh_u32 _Dim>
	bool	_GetAttribute(UPopcornFXEmitterComponent *component, int32 _attri, PopcornFX::TVector<_Scalar, _Dim> &outValue)
	{
		typedef PopcornFX::TVector<_Scalar, _Dim>		Vector;

		uint32		attri(_attri);

		if (!HH_VERIFY(component != null))
			return false;
		UPopcornFXAttributeList		*attrList = component->GetAttributeList();
		if (!HH_VERIFY(attrList != null))
			return false;

		if (attri >= attrList->AttributeCount())
		{
			UE_LOG(LogPopcornFXAttributeFunctions, Warning, TEXT("GetAttributeAs: invalid InAttributeIndex %d (%s)"), _attri, *(component->GetPathName()));
			return false;
		}

#if PKUE_ATTRIB_ENABLE_CHECKS
		const PopcornFX::CParticleAttributeDeclaration	*decl = attrList->GetAttributeDeclaration(component->Effect, attri);
		if (!HH_VERIFY(decl != null))
			return false;
		const PopcornFX::CBaseTypeTraits		&attrTraits = PopcornFX::CBaseTypeTraits::Traits((PopcornFX::EBaseTypeID)decl->AttributeType());
		if (_Dim > attrTraits.VectorDimension ||
			IsFp<_Scalar>::Value != attrTraits.IsFp)
		{
			const char			*inType = (IsFp<_Scalar>::Value ? "Float" : "Int");
			const char			*attrType = (attrTraits.IsFp ? "Float" : "Int");
			UE_LOG(LogPopcornFXAttributeFunctions, Warning,
				TEXT("GetAttributeAs: the Attribute [%d] \"%s\" cannot be set as %s %d: the attribute is %s %d (%s)"),
				attri, ANSI_TO_TCHAR(decl->AttributeName().Data()),
				ANSI_TO_TCHAR(inType), _Dim,
				ANSI_TO_TCHAR(attrType), attrTraits.VectorDimension,
				*(component->GetPathName())
				);
			return false;
		}
#endif

		outValue = reinterpret_cast<Vector&>(attrList->AttributeRawDataAttributes()[attri]);
		return true;
	}

} // namespace

//----------------------------------------------------------------------------

UPopcornFXAttributeFunctions::UPopcornFXAttributeFunctions(class FObjectInitializer const &pcip)
	: Super(pcip)
{
}

//----------------------------------------------------------------------------

int32	UPopcornFXAttributeFunctions::FindAttributeIndex(const UPopcornFXEmitterComponent *InSelf, FName InAttributeName)
{
	if (!HH_VERIFY(InSelf != null) || !HH_VERIFY(InSelf->Effect != null))
		return -1;
	UPopcornFXAttributeList		*attrList = InSelf->GetAttributeListIFP();
	if (!HH_VERIFY(attrList != null))
		return -1;
	return attrList->FindAttributeIndex(InAttributeName);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeFunctions::ResetToDefaultValue(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex)
{
	return _ResetAttribute(InSelf, InAttributeIndex);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeFunctions::SetAttributeAsFloat(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float InValue, bool InApplyGlobalScale)
{
	CFloat1		inValue(InValue);
	if (InApplyGlobalScale)
		inValue *= FPopcornFXPlugin::GlobalScaleRcp();
	return _SetAttribute(InSelf, InAttributeIndex, inValue);
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsFloat(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float &OutValue, bool InApplyGlobalScale)
{
	CFloat1		outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		if (InApplyGlobalScale)
			outValue *= FPopcornFXPlugin::GlobalScale();
		OutValue = outValue.x();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsFloat2(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float InValueX, float InValueY, bool InApplyGlobalScale)
{
	CFloat2		inValue(InValueX, InValueY);

	if (InApplyGlobalScale)
		inValue *= FPopcornFXPlugin::GlobalScaleRcp();
	return _SetAttribute(InSelf, InAttributeIndex, inValue);
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsVector2D(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FVector2D InValue, bool InApplyGlobalScale)
{
	return SetAttributeAsFloat2(InSelf, InAttributeIndex, InValue.X, InValue.Y, InApplyGlobalScale);
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsFloat2(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float &OutValueX, float &OutValueY, bool InApplyGlobalScale)
{
	CFloat2		outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		if (InApplyGlobalScale)
			outValue *= FPopcornFXPlugin::GlobalScale();
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsVector2D(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FVector2D &OutValue, bool InApplyGlobalScale)
{
	return GetAttributeAsFloat2(InSelf, InAttributeIndex, OutValue.X, OutValue.Y, InApplyGlobalScale);
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsFloat3(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float InValueX, float InValueY, float InValueZ, bool InApplyGlobalScale)
{
	CFloat3		inValue(InValueX, InValueY, InValueZ);

	if (InApplyGlobalScale)
		inValue *= FPopcornFXPlugin::GlobalScaleRcp();
	return _SetAttribute(InSelf, InAttributeIndex, inValue);
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsVector(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FVector InValue, bool InApplyGlobalScale)
{
	return SetAttributeAsFloat3(InSelf, InAttributeIndex, InValue.X, InValue.Y, InValue.Z, InApplyGlobalScale);
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsFloat3(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float &OutValueX, float &OutValueY, float &OutValueZ, bool InApplyGlobalScale)
{
	CFloat3		outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		if (InApplyGlobalScale)
			outValue *= FPopcornFXPlugin::GlobalScale();
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		OutValueZ = outValue.z();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsVector(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FVector &OutValue, bool InApplyGlobalScale)
{
	return GetAttributeAsFloat3(InSelf, InAttributeIndex, OutValue.X, OutValue.Y, OutValue.Z, InApplyGlobalScale);
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsFloat4(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float InValueX, float InValueY, float InValueZ, float InValueW, bool InApplyGlobalScale)
{
	CFloat4		inValue(InValueX, InValueY, InValueZ, InValueW);

	if (InApplyGlobalScale)
		inValue *= FPopcornFXPlugin::GlobalScaleRcp();
	return _SetAttribute(InSelf, InAttributeIndex, inValue);
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FLinearColor InValue, bool InApplyGlobalScale)
{
	return SetAttributeAsFloat4(InSelf, InAttributeIndex, InValue.R, InValue.G, InValue.B, InValue.A, InApplyGlobalScale);
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsFloat4(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, float &OutValueX, float &OutValueY, float &OutValueZ, float &OutValueW, bool InApplyGlobalScale)
{
	CFloat4		outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		if (InApplyGlobalScale)
			outValue *= FPopcornFXPlugin::GlobalScale();
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		OutValueZ = outValue.z();
		OutValueW = outValue.w();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsLinearColor(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, FLinearColor &OutValue, bool InApplyGlobalScale)
{
	return GetAttributeAsFloat4(InSelf, InAttributeIndex, OutValue.R, OutValue.G, OutValue.B, OutValue.A, InApplyGlobalScale);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeFunctions::SetAttributeAsInt(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 InValue)
{
	return _SetAttribute(InSelf, InAttributeIndex, CInt1(InValue));
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsInt(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 &OutValue)
{
	CInt1	outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		OutValue = outValue.x();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsInt2(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 InValueX, int32 InValueY)
{
	return _SetAttribute(InSelf, InAttributeIndex, CInt2(InValueX, InValueY));
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsInt2(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 &OutValueX, int32 &OutValueY)
{
	CInt2	outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsInt3(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 InValueX, int32 InValueY, int32 InValueZ)
{
	return _SetAttribute(InSelf, InAttributeIndex, CInt3(InValueX, InValueY, InValueZ));
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsInt3(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 &OutValueX, int32 &OutValueY, int32 &OutValueZ)
{
	CInt3	outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		OutValueZ = outValue.z();
		return true;
	}
	return false;
}

bool	UPopcornFXAttributeFunctions::SetAttributeAsInt4(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 InValueX, int32 InValueY, int32 InValueZ, int32 InValueW)
{
	return _SetAttribute(InSelf, InAttributeIndex, CInt4(InValueX, InValueY, InValueZ, InValueW));
}

bool	UPopcornFXAttributeFunctions::GetAttributeAsInt4(UPopcornFXEmitterComponent *InSelf, int32 InAttributeIndex, int32 &OutValueX, int32 &OutValueY, int32 &OutValueZ, int32 &OutValueW)
{
	CInt4	outValue;
	if (_GetAttribute(InSelf, InAttributeIndex, outValue))
	{
		OutValueX = outValue.x();
		OutValueY = outValue.y();
		OutValueZ = outValue.z();
		OutValueW = outValue.w();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
