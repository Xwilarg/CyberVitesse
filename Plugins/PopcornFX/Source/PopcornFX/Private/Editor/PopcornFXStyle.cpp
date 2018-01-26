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
#include "PopcornFXStyle.h"

#include "PopcornFXSDK.h"
#include "PopcornFXPlugin.h"

#include "SlateStyle.h"
#include "Interfaces/IEditorStyleModule.h"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style.RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__ )
#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FPopcornFXStyle::InContent(RelativePath, ".png"), __VA_ARGS__ )

TSharedPtr<FSlateStyleSet> FPopcornFXStyle::PopcornFXStyleInstance = NULL;

void	FPopcornFXStyle::Initialize()
{
	if (!PopcornFXStyleInstance.IsValid())
	{
		PopcornFXStyleInstance = Create();
	}

	SetStyle(PopcornFXStyleInstance.ToSharedRef());
}

void	FPopcornFXStyle::Shutdown()
{
	ResetToDefault();
	HH_VERIFY(PopcornFXStyleInstance.IsUnique());
	PopcornFXStyleInstance.Reset();
}

FString	FPopcornFXStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	return (FPopcornFXPlugin::PopcornFXPluginContentDir() / RelativePath) + Extension;
}

TSharedRef<FSlateStyleSet>	FPopcornFXStyle::Create()
{
	IEditorStyleModule& EditorStyle = FModuleManager::LoadModuleChecked<IEditorStyleModule>(TEXT("EditorStyle"));
	TSharedRef< FSlateStyleSet > StyleRef = EditorStyle.CreateEditorStyleInstance();
	FSlateStyleSet& Style = StyleRef.Get();

	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	Style.Set("ClassIcon.PopcornFXFile", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXFile", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXEffect", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_pkfx_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXEffect", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_pkfx_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXTextureAtlas", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_pkat_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXTextureAtlas", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_pkat_64x"), Icon64x64));

	// TODO: Create custom texture for pksc assets
	Style.Set("ClassIcon.PopcornFXSimulationCache", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXSimulationCache", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXEmitter", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXEmitter", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXSceneActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXSceneActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXEmitterComponent", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXEmitterComponent", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXAttributeSampler", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerShape.med"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXAttributeSampler", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerShape.med"), Icon64x64));

	Style.Set("ClassIcon.PopcornFXAttributeSamplerActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerShape.med"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXAttributeSamplerActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerShape.med"), Icon64x64));

	Style.Set("PopcornFX.BadIcon32", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/BadIcon"), Icon32x32));

	// Abstract, should not be used:
	Style.Set("ClassIcon.PopcornFXAttributeSampler", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/BadIcon"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXAttributeSampler", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/BadIcon"), Icon64x64));
	Style.Set("ClassIcon.PopcornFXAttributeSamplerActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/BadIcon"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXAttributeSamplerActor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/BadIcon"), Icon64x64));

#define ATTRIBSAMPLER_STYLE(__name, __image)																				\
	Style.Set("ClassIcon." __name "", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/") TEXT(__image) TEXT(".med"), Icon16x16));				\
	Style.Set("ClassThumbnail." __name "", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/") TEXT(__image) TEXT(".med"), Icon64x64));		\
	Style.Set("ClassIcon." __name "Actor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/") TEXT(__image) TEXT(".med"), Icon16x16));		\
	Style.Set("ClassThumbnail." __name "Actor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/") TEXT(__image) TEXT(".med"), Icon64x64));

	ATTRIBSAMPLER_STYLE("PopcornFXAttributeSamplerShape", "CParticleSamplerShape");
	ATTRIBSAMPLER_STYLE("PopcornFXAttributeSamplerImage", "CParticleSamplerTexture");
	ATTRIBSAMPLER_STYLE("PopcornFXAttributeSamplerCurve", "CParticleSamplerCurve");
	ATTRIBSAMPLER_STYLE("PopcornFXAttributeSamplerText", "CParticleSamplerText");
	ATTRIBSAMPLER_STYLE("PopcornFXAttributeSamplerSkinnedMesh", "CParticleSamplerSkinnedMesh");

#undef ATTRIBSAMPLER_STYPE

	// No actor, is a component only
	Style.Set("ClassIcon.PopcornFXAttributeSamplerCurveDynamic", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerCurve.med"), Icon16x16));
	Style.Set("ClassThumbnail.PopcornFXAttributeSamplerCurveDynamic", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/CParticleSamplerCurve.med"), Icon64x64));

	Style.Set("PopcornFX.Attribute.F1", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeF1"), Icon32x32));
	Style.Set("PopcornFX.Attribute.F2", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeF2"), Icon32x32));
	Style.Set("PopcornFX.Attribute.F3", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeF3"), Icon32x32));
	Style.Set("PopcornFX.Attribute.F4", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeF4"), Icon32x32));

	Style.Set("PopcornFX.Attribute.I1", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeI1"), Icon32x32));
	Style.Set("PopcornFX.Attribute.I2", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeI2"), Icon32x32));
	Style.Set("PopcornFX.Attribute.I3", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeI3"), Icon32x32));
	Style.Set("PopcornFX.Attribute.I4", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/AttributeI4"), Icon32x32));

	Style.Set("PopcornFXSequencer.StartEmitter", new IMAGE_BRUSH("Icons/generic_play_16x", Icon16x16));
	Style.Set("PopcornFXSequencer.StopEmitter", new IMAGE_BRUSH("Icons/generic_stop_16x", Icon16x16));

#if WITH_EDITOR
	Style.Set("PopcornFXEffectEditor.TogglePreviewGrid", new IMAGE_BRUSH("Icons/icon_MatEd_Grid_40x", Icon32x32));
	Style.Set("PopcornFXEffectEditor.TogglePreviewGrid.Small", new IMAGE_BRUSH("Icons/icon_MatEd_Grid_40x", Icon16x16));

	Style.Set("PopcornFXEffectEditor.ToggleDrawBounds", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/EffectEditor_Bounds"), Icon16x16));
	Style.Set("PopcornFXEffectEditor.ToggleDrawBounds.Small", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/EffectEditor_Bounds"), Icon16x16));

	Style.Set("PopcornFXEffectEditor.ResetEmitter", new IMAGE_BRUSH("Icons/icon_Cascade_RestartInLevel_40x", Icon40x40));
	Style.Set("PopcornFXEffectEditor.ResetEmitter.Small", new IMAGE_BRUSH("Icons/icon_Cascade_RestartInLevel_40x", Icon20x20));

	Style.Set("PopcornFXEffectEditor.ReimportEffect", new IMAGE_BRUSH("Icons/icon_TextureEd_Reimport_40x", Icon40x40));
	Style.Set("PopcornFXEffectEditor.ReimportEffect.Small", new IMAGE_BRUSH("Icons/icon_TextureEd_Reimport_40x", Icon20x20));

	Style.Set("PopcornFXEffectEditor.OpenInPopcornFXEditor", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_64x"), Icon40x40));
	Style.Set("PopcornFXEffectEditor.OpenInPopcornFXEditor.Small", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/icon_PopcornFX_Logo_16x"), Icon20x20));

	Style.Set("PopcornFXLevelEditor.OpenSourcePack", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/LevelEditor_OpenSourcePack_40x"), Icon40x40));
	Style.Set("PopcornFXLevelEditor.OpenSourcePack.Small", new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/LevelEditor_OpenSourcePack_20x"), Icon20x20));
#endif // WITH_EDITOR

#define STYLE_PKFX_NODE(__node)		\
	Style.Set("PopcornFX.Node." __node, new IMAGE_PLUGIN_BRUSH(TEXT("SlateBrushes/") TEXT(__node) TEXT(".med"), Icon32x32))

	STYLE_PKFX_NODE("CParticleSamplerCurve");
	STYLE_PKFX_NODE("CParticleSamplerProceduralTurbulence");
	STYLE_PKFX_NODE("CParticleSamplerShape");
	STYLE_PKFX_NODE("CParticleSamplerTexture");
	STYLE_PKFX_NODE("CParticleSamplerText");
	STYLE_PKFX_NODE("CShapeDescriptor_Box");
	STYLE_PKFX_NODE("CShapeDescriptor_Capsule");
	STYLE_PKFX_NODE("CShapeDescriptor_Collection");
	STYLE_PKFX_NODE("CShapeDescriptor_ComplexEllipsoid");
	STYLE_PKFX_NODE("CShapeDescriptor_Cone");
	STYLE_PKFX_NODE("CShapeDescriptor_Cylinder");
	STYLE_PKFX_NODE("CShapeDescriptor_Mesh");
	STYLE_PKFX_NODE("CShapeDescriptor_Plane");
	STYLE_PKFX_NODE("CShapeDescriptor_Sphere");

#undef STYLE_PKFX_NODE

	return StyleRef;
}

#endif // WITH_EDITOR
