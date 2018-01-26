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

#include "PopcornFXMinimal.h"

#include "PopcornFXSettings.h"

#include "Render/RenderManagerBase.h"

#include "Render/DrawCall.h"
#include "Render/DrawCalls_Billboard.h"
#include "Render/DrawCalls_Ribbon.h"
#include "Render/DrawCalls_Mesh.h"
#include "Render/DrawCalls_Light.h"
#include "StrongObjectPtrViaShared.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/Renderers/Std/renderer_std.h>
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_kernel/include/kr_containers_array_wrap.h>
#include <pk_drawers/include/dr_renderers_walker.h>

class UPopcornFXRendererMaterial;
class UMaterialInstanceConstant;

//----------------------------------------------------------------------------

//#define	IF_DEBUG_BBOX(...)			__VA_ARGS__
#define	IF_DEBUG_BBOX(...)

//----------------------------------------------------------------------------

class CParticleCollectedFrameToRender
{
public:
	CParticleCollectedFrameToRender();
	~CParticleCollectedFrameToRender();

	void				Clear();
	bool				Empty() const { return m_TotalDrCount == 0; }

	hh_u32				m_CollectedFrameId;
	hh_u32				m_TotalDrCount;
	bool				m_Built = false;
	bool				m_EarlyReleaseCleared;
	hh_u32				m_DrawCalledCount = 0;

	FPopcornFXRenderSettings		m_RenderSettings;

#if POPCORNFX_RENDER_DEBUG
	uint32							m_HeavyDebugMode = 0;
	bool							m_OverrideColorByDrawCall = false;
	hh_u32							m_OverrideDebugColorCount = 6;
	UMaterialInterface				*m_OverrideAllMaterial;
#endif

	PopcornFX::Drawers::CBillboard_DrawRequests					m_BillboardDrs;
	PopcornFX::TArray<CParticleMaterialDesc_Game>				m_BillboardMaterialDescs;

	PopcornFX::Drawers::CRibbon_DrawRequests					m_RibbonDrs;
	PopcornFX::TArray<CParticleMaterialDesc_Game>				m_RibbonMaterialDescs;

	PopcornFX::Drawers::CMesh_DrawRequests						m_MeshDrs;
	PopcornFX::TArray<CParticleMaterialDesc_MeshDesc_Game>		m_MeshDescMaterialDescs;
	// could be less, but not more
	hh_u32				m_MaxMultiSectionMeshDrawCallCount;

	PopcornFX::Drawers::CLight_DrawRequests						m_LightDrs;
};

//----------------------------------------------------------------------------

class CParticleCollectedFrameToRenderPool;

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleDrawer_RenderManagerMaster);

struct _SGenericCompatFlags
{
public:
	_SGenericCompatFlags() : m_Flags(0) {}
	void			Clear() { m_Flags = 0; }
	void			Set(uint32 flagId, bool value) { HH_ASSERT(flagId < sizeof(m_Flags) * 8); if (value) m_Flags |= (1 << flagId); }
	bool			Has(uint32 flagId) const { HH_ASSERT(flagId < sizeof(m_Flags) * 8); return (m_Flags & (1 << flagId)) != 0; }
	bool			IsCompatible(const _SGenericCompatFlags &other) const { return m_Flags == other.m_Flags; }
	bool			operator == (const _SGenericCompatFlags &other) const { return m_Flags == other.m_Flags; }
private:
	uint32			m_Flags;
};

struct SMaterialCompatFlags : public _SGenericCompatFlags
{
};

struct SDrawerFlags : public _SGenericCompatFlags
{
	enum
	{
		SupportGPUParticles,
		_Last,
	};
};

class CParticleRenderManager : public CParticleRenderManagerBase, public PopcornFX::Drawers::IRenderersWalkerDelegates
{

public:
	CParticleRenderManager();
	~CParticleRenderManager();

	virtual void		Clean() override;
	virtual void		Clear() override;

	virtual void		InstallToMediumCollection(PopcornFX::CParticleMediumCollection &collection) override;

	virtual void		GameThread_PreUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) override;
	virtual void		GameThread_EndUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) override;
	virtual void		ConcurrentThread_SendRenderDynamicData(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) override;

	virtual void		RenderThread_BuildAndRenderNewFrame(PopcornFX::CRendererSubView &views) override;
	virtual void		RenderThread_RenderLastFrame(PopcornFX::CRendererSubView &views) override;

private:
	CParticleCollectedFrameToRenderPool			*m_FramePool = null;
	hh_u32										m_CollectedFrameCount = 0; // for frame id
	hh_u32										m_LastFrameDrawCalledCount = 0;
	PopcornFX::Threads::CCriticalSection		m_RenderingLock;
	CParticleCollectedFrameToRender				*m_Rendering_LockMe = null;

private:
	// Collect stuff
	CParticleCollectedFrameToRender				*m_Collecting = null;
	PopcornFX::Drawers::CRenderersWalker		m_RenderersWalker;
	hh_u32										m_CollectedUsedMaterialCount = 0;

	void				Collect_Clean();
	virtual bool		RenderersWalk(const PopcornFX::CParticleRenderMedium &renderMedium) override { return true; }
	virtual bool		RenderersWalkReserveAtMost(PopcornFX::Drawers::ERendererClass rendererClass, hh_u32 count) override;
	virtual void		RenderersWalk(const PopcornFX::CParticleRenderer_Billboard &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer) override;
	virtual void		RenderersWalk(const PopcornFX::CParticleRenderer_Ribbon &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer) override;
	virtual void		RenderersWalk(const PopcornFX::CParticleRenderer_Mesh &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer) override;
	virtual void		RenderersWalk(const PopcornFX::CParticleRenderer_Light &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer) override;
	void				AddCollectedUsedMaterial(UMaterialInterface *materialInstance);

private:
	// Render stuff
	void				SetupRender(PopcornFX::CRendererSubView &views, bool newFrame);
	void				StartAndCallDrawCalls();

	bool				CullTest(const PopcornFX::Drawers::SBase_DrawRequest &dr, bool castShadow);
	bool				MergeDrawCall(const PopcornFX::Drawers::SBillboard_DrawRequest &dr, const CParticleMaterialDesc_Game &matDesc);
	bool				MergeDrawCall(const PopcornFX::Drawers::SRibbon_DrawRequest &dr, const CParticleMaterialDesc_Game &matDesc);

	bool				BuildDrawCalls_Billboard();
	bool				StartDrawCalls_Billboard();

	bool				BuildDrawCalls_Ribbon();
	bool				StartDrawCalls_Ribbon();

	bool				BuildDrawCalls_Mesh();
	bool				StartDrawCalls_Mesh();

	bool				BuildDrawCalls_Light();
	//bool				StartDrawCalls_Light();

	bool				_StartRender_MeshDesc(CParticleBillboarder_Mesh_CPU &bb, const CParticleMaterialDesc_MeshDesc_Game &matDesc, hh_u32 meshDesci);

	void				CallDrawCalls();

#if POPCORNFX_RENDER_DEBUG
	void				DrawHeavyDebug(const FPopcornFXSceneProxy *sceneProxy, FPrimitiveDrawInterface* PDI, const FSceneView* view, hh_u32 debugModeMask);
#endif

	struct SProjection
	{
		uint64							m_Key;
		CParticleAbstractDrawCall		*m_DrawCall;
		SProjection(uint64 key, CParticleAbstractDrawCall *drawCall) : m_Key(key), m_DrawCall(drawCall) {}
		HH_FORCEINLINE bool		operator < (const SProjection &other) const  { return m_Key > other.m_Key; } // back to front
		HH_FORCEINLINE bool		operator <= (const SProjection &other) const { return m_Key >= other.m_Key; }
		HH_FORCEINLINE bool		operator == (const SProjection &other) const { return m_Key == other.m_Key; }
	};

	struct SProjections
	{
		PopcornFX::TArray<SProjection>		m_Depths;
	};

	PopcornFX::TStaticArray<SProjections, PopcornFX::CRendererSubView::kMaxViews>	m_ViewsProjections;
	PopcornFX::TArray<CParticleAbstractDrawCall*>									m_SolidDrawCalls;
	PopcornFX::TArray<CParticleAbstractDrawCall*>									m_TransDrawCalls;

	struct SMaterialDescMerged
	{
		SMaterialCompatFlags				m_CompatFlags;
		CParticleMaterialDesc_Render		m_MatDesc;
		void	Clear() { m_MatDesc.Clear(); }
	};

	struct SMaterialDescMerged_Mesh
	{
		bool												m_IsGPU;
		CParticleMaterialDesc_MeshSingleSection_Render		m_MatDesc;
		void	Clear() { m_MatDesc.Clear(); }
	};

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleDrawCall_Billboard> >		m_BillboardDrawCalls;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<SMaterialDescMerged> >				m_BillboardDrawCallsMatDescs;

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleDrawCall_Ribbon> >			m_RibbonDrawCalls;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<SMaterialDescMerged> >				m_RibbonDrawCallsMatDescs;

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleDrawCall_MeshSingleSection> >		m_MeshSingleSectionDrawCalls;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<SMaterialDescMerged_Mesh> >					m_MeshSingleSectionMatDescs;

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleDrawCall_MeshMultiSection> >				m_MeshMultiSectionDrawCalls;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleMaterialDesc_MeshMultiSection_Render> >		m_MeshMultiSectionMatDescs;

	// not a pool, we know we have 1 BB per DR (BB are not merge, draw calls are)
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleBillboarder_Mesh_CPU> >		m_MeshCPUBB;

public:
	// ParticleDrawCall utils:

	FPopcornFXRenderSettings		m_RenderSettings;

	bool			m_IsNewFrame;

	bool			m_Can_CastShadows;
	bool			m_CullTest_DrawRequest;

	//bool			m_CPUParticleGPUBillboarding;
	//bool			m_ForceSplitBillboarding;

	PopcornFX::CRendererSubView				*m_Views;
	ERHIFeatureLevel::Type					m_FeatureLevel;

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleBillboarder_Billboard_CPU> >	m_BillboardCPUBBPool;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleBillboarder_Billboard_GPU> >	m_BillboardGPUBBPool;

	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<CParticleBillboarder_Ribbon_CPU> >		m_RibbonCPUBBPool;

	PopcornFX::CStringId			m_DiffuseColorStringId;

#if POPCORNFX_RENDER_DEBUG
	FVertexBuffer					*m_OverrideColorVertexBuffer = null;
	const void						*m_OverrideColorBuffer = null;
	int32							m_OverrideColorBufferOffset;
	FMaterialRenderProxy			*m_OverrideAllMaterialRenderProxy = null;

	TResourceArray<FLinearColor, 0x10>	m_DebugColorsBuffer;
	FVertexBuffer						m_DebugColorsVertexBuffer;
#endif // POPCORNFX_RENDER_DEBUG

	// Master Drawer stuff:

private:
	bool	_Drawer_Setup(CParticleDrawer_RenderManagerMaster *drawer);

public:
	PopcornFX::CParticleDrawer_Std		*Drawer_BuildRemapperWrapper(PopcornFX::CParticleRenderMedium *rMedium, PopcornFX::CParticleRenderManager_Std *manager);

	void	Drawer_HarvestStuff(const CParticleDrawer_RenderManagerMaster *drawer, const PopcornFX::CParticleRenderer *rendererDesc, float &outMaxScale, float &outMaxConstant);
	bool	Drawer_AreRenderersCompatible(const CParticleDrawer_RenderManagerMaster *drawer, const PopcornFX::CParticleRenderer *rendererA, const PopcornFX::CParticleRenderer *rendererB);
	bool	Drawer_GetStoragePrefs(const CParticleDrawer_RenderManagerMaster *drawer, hh_u32 &outSupportedLocations, hh_u32 &outPrefferedLocations);

	//PParticleDrawer_RenderManagerMaster	m_MasterDrawer;
};
HH_DECLARE_REFPTRCLASS(ParticleRenderManager);

//----------------------------------------------------------------------------

class	CParticleDrawer_RenderManagerMaster : public PopcornFX::CParticleDrawer_Std
{
public:
	friend class CParticleRenderManager;

	CParticleDrawer_RenderManagerMaster(CParticleRenderManager *manager, PopcornFX::CParticleRenderMedium *rMedium) : m_Manager(manager), m_RenderMedium(rMedium) { }
	virtual ~CParticleDrawer_RenderManagerMaster() {}

	virtual void	Clear() override {}
	//virtual void	Update() override {}
	virtual bool	CanRenderInView(PopcornFX::CRendererSubView &view) const override { return false; }

	//virtual void	PreRender(PopcornFX::CRendererSubView &view) override { }
	virtual void	StartRender(PopcornFX::CRendererSubView &view, const CFloat3 &geometricMean, const TMemoryView<const PopcornFX::CParticleRenderMedium::SMediumRendererAssoc> &drawRequests) override { }
	virtual void	KickRender(PopcornFX::CRendererSubView &view) override { }

	//virtual void	EditorRender(CRendererSubView &view, const CParticlePage *page, const TMemoryView<const hh_u32> &sparseIndices, EDebugRenderMode mode, const CParticleRenderer *renderer) { }

	virtual void	HarvestParticleRadiusVisibilityCoefficients(const PopcornFX::CParticleRenderer *rendererDesc, float &outMaxScale, float &outMaxConstant) const override { m_Manager->Drawer_HarvestStuff(this, rendererDesc, outMaxScale, outMaxConstant);}
	virtual bool	AreRenderersCompatible(const PopcornFX::CParticleRenderer *rendererA, const PopcornFX::CParticleRenderer *rendererB) override { return m_Manager->Drawer_AreRenderersCompatible(this, rendererA, rendererB);}

	//virtual void	OnParentDescriptorChanged(const CParticleRenderer *parent) { }
	virtual bool	GetStoragePrefs(hh_u32 &outSupportedLocations, hh_u32 &outPrefferedLocations) override { return m_Manager->Drawer_GetStoragePrefs(this, outSupportedLocations, outPrefferedLocations); }

	CParticleRenderManager				*Manager() const { return m_Manager; }
	PopcornFX::CParticleRenderMedium	*RenderMedium() const { return m_RenderMedium; }
	const SDrawerFlags					&DrawerFlags() const { return m_DrawerFlags; }
	void								SetDrawerFlags(const SDrawerFlags &flags) { m_DrawerFlags = flags; }

private:
	CParticleRenderManager				*m_Manager;
	PopcornFX::CParticleRenderMedium	*m_RenderMedium;
	SDrawerFlags						m_DrawerFlags;
	TStrongObjectPtrViaShared<UPopcornFXRendererMaterial>			m_RendererMaterial;
};
HH_DECLARE_REFPTRCLASS(ParticleDrawer_RenderManagerMaster);
