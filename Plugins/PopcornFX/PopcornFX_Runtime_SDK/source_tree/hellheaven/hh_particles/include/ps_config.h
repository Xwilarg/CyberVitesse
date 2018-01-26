#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2012/02/26 14:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_CONFIG_H__
#define	__PS_CONFIG_H__

#include <hh_geometrics/include/ge_config.h>	// what for? GE_BILLBOARDS cfg flags?
#include <hh_maths/include/hh_maths_simd_defs.h>

//----------------------------------------------------------------------------

#ifndef	HH_PARTICLES_BUILD_DEBUG_HISTORY
#	if	defined(HH_DESKTOP)
#		define	HH_PARTICLES_BUILD_DEBUG_HISTORY	1
#	else
#		define	HH_PARTICLES_BUILD_DEBUG_HISTORY	0
#	endif
#endif

//----------------------------------------------------------------------------

#ifndef	HH_PARTICLES_ALLOCATOR_PAGESIZE_MIN
#	define	HH_PARTICLES_ALLOCATOR_PAGESIZE_MIN		0x00000200	// 512 bytes, 16 particles @ 32-bytes per particle : won't even happen
#endif

#ifndef	HH_PARTICLES_ALLOCATOR_PAGESIZE_MAX
#	define	HH_PARTICLES_ALLOCATOR_PAGESIZE_MAX		0x00100000	// 1 Mb, 2048 particles @ 512 bytes per particle : unlikely...
#endif

//----------------------------------------------------------------------------
// CPU updater

//----------------------------------------------------------------------------
// GPU updater : Compute shaders

#ifndef	HH_PARTICLES_UPDATER_USE_D3D11
#	if	defined(HH_DESKTOP) && defined(HH_WINDOWS)
#		define	HH_PARTICLES_UPDATER_USE_D3D11					0//1	// don't activate by default yet
#	else
#		define	HH_PARTICLES_UPDATER_USE_D3D11					0
#	endif
#endif	// HH_PARTICLES_UPDATER_USE_D3D11

//----------------------------------------------------------------------------
// Feature control : Evolvers

#ifndef	HH_PARTICLES_BUILD_EVOLVER_SCRIPT
#	define	HH_PARTICLES_BUILD_EVOLVER_SCRIPT					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_FIELD
#	define	HH_PARTICLES_BUILD_EVOLVER_FIELD					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_ROTATION
#	define	HH_PARTICLES_BUILD_EVOLVER_ROTATION					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_DAMPER
#	define	HH_PARTICLES_BUILD_EVOLVER_DAMPER					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_PHYSICS
#	define	HH_PARTICLES_BUILD_EVOLVER_PHYSICS					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_COLLISIONS
#	define	HH_PARTICLES_BUILD_EVOLVER_COLLISIONS				1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_SPAWNER
#	define	HH_PARTICLES_BUILD_EVOLVER_SPAWNER					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_FLOCKING
#	define	HH_PARTICLES_BUILD_EVOLVER_FLOCKING					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_CONTAINMENT
#	define	HH_PARTICLES_BUILD_EVOLVER_CONTAINMENT				1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_LIMITDISTANCE
#	define	HH_PARTICLES_BUILD_EVOLVER_LIMITDISTANCE			1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_PROJECTION
#	define	HH_PARTICLES_BUILD_EVOLVER_PROJECTION				1
#endif

#ifndef HH_PARTICLES_BUILD_EVOLVER_FLIPBOOK
#	define	HH_PARTICLES_BUILD_EVOLVER_FLIPBOOK					1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_LOCALSPACE
#	define	HH_PARTICLES_BUILD_EVOLVER_LOCALSPACE				1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_ATTRACTOR
#	define	HH_PARTICLES_BUILD_EVOLVER_ATTRACTOR				1
#endif

#ifndef	HH_PARTICLES_BUILD_EVOLVER_SPATIAL_INSERTION
#	define	HH_PARTICLES_BUILD_EVOLVER_SPATIAL_INSERTION		1
#endif

//----------------------------------------------------------------------------
// feature control : Samplers

#ifndef	HH_PARTICLES_BUILD_SAMPLER_CURVE
#	define	HH_PARTICLES_BUILD_SAMPLER_CURVE					1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_SHAPE
#	define	HH_PARTICLES_BUILD_SAMPLER_SHAPE					1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_ANIMTRACK
#	define	HH_PARTICLES_BUILD_SAMPLER_ANIMTRACK				1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_TEXTURE
#	define	HH_PARTICLES_BUILD_SAMPLER_TEXTURE					1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_TURBULENCE
#	define	HH_PARTICLES_BUILD_SAMPLER_TURBULENCE				1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_SPECTRUM
#	define	HH_PARTICLES_BUILD_SAMPLER_SPECTRUM					1
#endif

#ifndef	HH_PARTICLES_BUILD_SAMPLER_TEXT
#	define	HH_PARTICLES_BUILD_SAMPLER_TEXT						1
#endif

//----------------------------------------------------------------------------
// feature control : Renderers

#ifndef	HH_PARTICLES_BUILD_RENDERER_BILLBOARD
#	define	HH_PARTICLES_BUILD_RENDERER_BILLBOARD				1
#endif

#ifndef	HH_PARTICLES_BUILD_RENDERER_LIGHT
#	define	HH_PARTICLES_BUILD_RENDERER_LIGHT					1
#endif

#ifndef	HH_PARTICLES_BUILD_RENDERER_MESH
#	define	HH_PARTICLES_BUILD_RENDERER_MESH					1
#endif

#ifndef	HH_PARTICLES_BUILD_RENDERER_RIBBON
#	define	HH_PARTICLES_BUILD_RENDERER_RIBBON					1
#endif

#ifndef	HH_PARTICLES_BUILD_RENDERER_SOUND
#	define	HH_PARTICLES_BUILD_RENDERER_SOUND					1
#endif

//----------------------------------------------------------------------------

#ifndef	HH_PARTICLES_DEBUG_CHECK_STREAMS_FOR_INF_OR_NAN
#	if	defined(HH_DEBUG)
#		define	HH_PARTICLES_DEBUG_CHECK_STREAMS_FOR_INF_OR_NAN				1
#	else
#		define	HH_PARTICLES_DEBUG_CHECK_STREAMS_FOR_INF_OR_NAN				0
#	endif
#endif

#ifndef	HH_PARTICLES_USE_CORRECT_MAX_CONSTANT_RADIUS_COMPUTATION
#	define	HH_PARTICLES_USE_CORRECT_MAX_CONSTANT_RADIUS_COMPUTATION		1
#endif

#ifndef	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER
#	if	defined(HH_PS3) || defined(HH_X360) || defined(HH_PSP2)
#		define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER			256U
#	else
#		define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER			1024U	// PC
#	endif
#endif

#ifndef	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS
#	if	defined(HH_PS3) || defined(HH_X360) || defined(HH_PSP2)
#		define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS	128U
#	else
#		define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS	256U	// PC
#	endif
#endif

//----------------------------------------------------------------------------
// Scene

#ifndef	HH_PARTICLES_HAS_OVERLAP_SCENE
#	define	HH_PARTICLES_HAS_OVERLAP_SCENE		1
#endif

//----------------------------------------------------------------------------
// Post-checks for consistency. no need to tweak these.

// if no physics evolver, force collisions off too.
#if	(HH_PARTICLES_BUILD_EVOLVER_PHYSICS == 0)
#	undef	HH_PARTICLES_BUILD_EVOLVER_COLLISIONS
#	define	HH_PARTICLES_BUILD_EVOLVER_COLLISIONS		0
#endif

//----------------------------------------------------------------------------

#if	!defined(HH_PARTICLES_HAS_STATS)
#	if	!defined(HH_RETAIL)
#		define	HH_PARTICLES_HAS_STATS	1
#	else
#		define	HH_PARTICLES_HAS_STATS	0
#	endif
#endif

//----------------------------------------------------------------------------
// Vector ISA selection:

#if	!defined(PS_VECTOR_SUPPORTS_AVX)
#	if (HH_SIMD == HH_SIMD_SSE) && (HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
#		define	PS_VECTOR_SUPPORTS_AVX	1
#	else
#		define	PS_VECTOR_SUPPORTS_AVX	0
#	endif
#endif

//----------------------------------------------------------------------------

#endif	// __PS_CONFIG_H__
