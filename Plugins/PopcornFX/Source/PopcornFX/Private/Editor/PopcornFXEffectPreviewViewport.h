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

#include "SDockTab.h"
#include "PreviewScene.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"

class UPopcornFXEffect;

class SPopcornFXEffectPreviewViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SPopcornFXEffectPreviewViewport) { }
		SLATE_ARGUMENT(TWeakPtr<class FPopcornFXEffectEditor>, EffectEditor)
	SLATE_END_ARGS()

	void	Construct(const FArguments &args);
	~SPopcornFXEffectPreviewViewport();

	void	SetPreviewEffect(UPopcornFXEffect *effect);
	void	OnAddedToTab(const TSharedRef<SDockTab> &ownerTab);

	virtual void	AddReferencedObjects(FReferenceCollector& Collector) override;

	virtual TSharedRef<class SEditorViewport>	GetViewportWidget() override;
	virtual TSharedPtr<FExtender>				GetExtenders() const override;
	virtual void								OnFloatingButtonClicked() override { };

	virtual TSharedRef<FEditorViewportClient>	MakeEditorViewportClient() override;
	virtual void								PopulateViewportOverlays(TSharedRef<class SOverlay> Overlay) override;
	virtual EVisibility							OnGetViewportContentVisibility() const override;
	virtual void								BindCommands() override;
	virtual void								OnFocusViewportToSelection() override;
	virtual void								Tick(const FGeometry &AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
public:
	void			ResetEmitterAttributes();
	void			ResetEmitter();

	void			ToggleLoopEmitter(ECheckBoxState newState);
	ECheckBoxState	IsToggleLoopEmitterChecked() const;
	float			OnGetLoopDelayValue() const;
	void			OnLoopDelayValueChanged(float newDelay);
private:
	void			TogglePreviewFloor();
	bool			IsTogglePreviewFloorChecked() const { return m_DisplayFloor; }
	void			TogglePreviewCubemap();
	bool			IsTogglePreviewCubemapChecked() const { return m_DisplayCubemap; }
	void			TogglePreviewGrid();
	bool			IsTogglePreviewGridChecked() const { return m_DisplayGrid; }
	void			ToggleDrawBounds();
	bool			IsToggleDrawBoundsChecked() const;

	class UPopcornFXSceneComponent	*SceneComponent() const;

	virtual bool	IsVisible() const override;
private:
	bool	m_DisplayFloor;
	bool	m_DisplayCubemap;
	bool	m_DisplayGrid;

	float	m_Time;

	FPreviewScene						m_PreviewScene;
	class UPopcornFXEmitterComponent	*m_EmitterComponent;

	TWeakPtr<SDockTab>						m_ParentTab;
	TWeakPtr<class FPopcornFXEffectEditor>	m_EffectEditor;

	TSharedPtr<class FPopcornFXEffectPreviewViewportClient>	m_ViewportClient;
};

#endif // WITH_EDITOR
