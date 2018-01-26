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
#include "PopcornFXDetailsAttributeSamplerShape.h"

#include "PopcornFXAttributeSamplerShape.h"
#include "PopcornFXSDK.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXDetailsAttributeSamplerShape"

//----------------------------------------------------------------------------

FPopcornFXDetailsAttributeSamplerShape::FPopcornFXDetailsAttributeSamplerShape()
	: m_CachedDetailLayoutBuilder(null)
{

}

//----------------------------------------------------------------------------

TSharedRef<IDetailCustomization>	FPopcornFXDetailsAttributeSamplerShape::MakeInstance()
{
	return MakeShareable(new FPopcornFXDetailsAttributeSamplerShape);
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsAttributeSamplerShape::RebuildDetails()
{
	if (!HH_VERIFY(m_CachedDetailLayoutBuilder != null))
		return;
	m_CachedDetailLayoutBuilder->ForceRefreshDetails();
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsAttributeSamplerShape::CustomizeDetails(IDetailLayoutBuilder& detailLayout)
{
	TSharedRef<IPropertyHandle>	shapeType = detailLayout.GetProperty("ShapeType");
	IDetailCategoryBuilder		&detailCategory = detailLayout.EditCategory("PopcornFX AttributeSampler");

	m_CachedDetailLayoutBuilder = &detailLayout;
	if (shapeType->IsValidHandle())
	{
		uint8	value;

		shapeType->GetValue(value);
		switch (value)
		{
		case	EPopcornFXAttribSamplerShapeType::Box:
			detailLayout.HideProperty("Radius");
			detailLayout.HideProperty("InnerRadius");
			detailLayout.HideProperty("Height");
			detailLayout.HideProperty("StaticMesh");
			detailLayout.HideProperty("StaticMeshSubIndex");
			detailLayout.HideProperty("Shapes");
			detailLayout.HideProperty("CollectionSamplingHeuristic");
			detailLayout.HideProperty("CollectionUseShapeWeights");
			break;
		case	EPopcornFXAttribSamplerShapeType::ComplexEllipsoid:
		case	EPopcornFXAttribSamplerShapeType::Sphere:
			detailLayout.HideProperty("BoxDimension");
			detailLayout.HideProperty("Height");
			detailLayout.HideProperty("StaticMesh");
			detailLayout.HideProperty("StaticMeshSubIndex");
			detailLayout.HideProperty("Shapes");
			detailLayout.HideProperty("CollectionSamplingHeuristic");
			detailLayout.HideProperty("CollectionUseShapeWeights");
			break;
		case	EPopcornFXAttribSamplerShapeType::Mesh:
			detailLayout.HideProperty("BoxDimension");
			detailLayout.HideProperty("Radius");
			detailLayout.HideProperty("InnerRadius");
			detailLayout.HideProperty("Height");
			detailLayout.HideProperty("Shapes");
			detailLayout.HideProperty("CollectionSamplingHeuristic");
			detailLayout.HideProperty("CollectionUseShapeWeights");
			break;
		case	EPopcornFXAttribSamplerShapeType::Cylinder:
		case	EPopcornFXAttribSamplerShapeType::Capsule:
			detailLayout.HideProperty("BoxDimension");
			detailLayout.HideProperty("StaticMesh");
			detailLayout.HideProperty("StaticMeshSubIndex");
			detailLayout.HideProperty("Shapes");
			detailLayout.HideProperty("CollectionSamplingHeuristic");
			detailLayout.HideProperty("CollectionUseShapeWeights");
			break;
		case	EPopcornFXAttribSamplerShapeType::Cone:
			detailLayout.HideProperty("BoxDimension");
			detailLayout.HideProperty("InnerRadius");
			detailLayout.HideProperty("StaticMesh");
			detailLayout.HideProperty("StaticMeshSubIndex");
			detailLayout.HideProperty("Shapes");
			detailLayout.HideProperty("CollectionSamplingHeuristic");
			detailLayout.HideProperty("CollectionUseShapeWeights");
			break;
		case	EPopcornFXAttribSamplerShapeType::Collection:
			detailLayout.HideProperty("Weight");
			detailLayout.HideProperty("BoxDimension");
			detailLayout.HideProperty("Radius");
			detailLayout.HideProperty("InnerRadius");
			detailLayout.HideProperty("Height");
			detailLayout.HideProperty("StaticMesh");
			detailLayout.HideProperty("StaticMeshSubIndex");
			break;
		default:
			break;
		}
		shapeType->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsAttributeSamplerShape::RebuildDetails));
	}
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
