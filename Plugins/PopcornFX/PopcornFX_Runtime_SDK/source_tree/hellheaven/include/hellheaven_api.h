#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2003/10/12 4:35 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HELLHEAVEN_API_H__
#define	__HELLHEAVEN_API_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	exports/imports api symbols
//	FIXME: move that into a public module header, so this header does not change
//	(for example, hh_kernel doesn't care about all the others)
//
//----------------------------------------------------------------------------

#if	!defined(HH_RUNTIME_DYNAMIC)
#	define	HH_API_LIB
#	define	HH_KERNEL_STATIC
#	define	HH_BASEOBJECT_STATIC
#	define	HH_ENGINEUTILS_STATIC
#	define	HH_COMPILER_STATIC
#	define	HH_IMAGING_STATIC
#	define	HH_GEOMETRICS_STATIC
#	define	HH_PARTICLES_STATIC
#	define	HH_ENGINE_STATIC
#	define	HH_WM_STATIC
#	define	HH_COMMON_STATIC
#	define	HH_NATIVE_EDITOR_STATIC
#	define	HH_PLUGINS_STATIC
#	define	PK_BILLBOARDER_STATIC
#endif

//----------------------------------------------------------------------------
// POPCORN RUNTIME

// HH_KERNEL
#if	defined(HH_KERNEL_EXPORT)
#	undef	HH_KERNEL_EXPORT
#	define	HH_KERNEL_EXPORT					HH_API_EXPORT
#elif	defined(HH_KERNEL_STATIC)
#	define	HH_KERNEL_EXPORT
#else
#	define	HH_KERNEL_EXPORT					HH_API_IMPORT
#endif

// HH_BASE_OBJECT
#if	defined(HH_BASEOBJECT_EXPORT)
#	undef	HH_BASEOBJECT_EXPORT
#	define	HH_BASEOBJECT_EXPORT				HH_API_EXPORT
#elif	defined(HH_BASEOBJECT_STATIC)
#	define	HH_BASEOBJECT_EXPORT
#else
#	define	HH_BASEOBJECT_EXPORT				HH_API_IMPORT
#endif

// HH_ENGINEUTILS
#if	defined(HH_ENGINEUTILS_EXPORT)
#	undef	HH_ENGINEUTILS_EXPORT
#	define	HH_ENGINEUTILS_EXPORT				HH_API_EXPORT
#elif	defined(HH_ENGINEUTILS_STATIC)
#	define	HH_ENGINEUTILS_EXPORT
#else
#	define	HH_ENGINEUTILS_EXPORT				HH_API_IMPORT
#endif

// HH_COMPILER
#if	defined(HH_COMPILER_EXPORT)
#	undef	HH_COMPILER_EXPORT
#	define	HH_COMPILER_EXPORT					HH_API_EXPORT
#elif	defined(HH_COMPILER_STATIC)
#	define	HH_COMPILER_EXPORT
#else
#	define	HH_COMPILER_EXPORT					HH_API_IMPORT
#endif

// HH_IMAGING
#if	defined(HH_IMAGING_EXPORT)
#	undef	HH_IMAGING_EXPORT
#	define	HH_IMAGING_EXPORT					HH_API_EXPORT
#elif	defined(HH_IMAGING_STATIC)
#	define	HH_IMAGING_EXPORT
#else
#	define	HH_IMAGING_EXPORT					HH_API_IMPORT
#endif

// HH_GEOMETRICS
#if	defined(HH_GEOMETRICS_EXPORT)
#	undef	HH_GEOMETRICS_EXPORT
#	define	HH_GEOMETRICS_EXPORT				HH_API_EXPORT
#elif	defined(HH_GEOMETRICS_STATIC)
#	define	HH_GEOMETRICS_EXPORT
#else
#	define	HH_GEOMETRICS_EXPORT				HH_API_IMPORT
#endif

// HH_PARTICLES
#if	defined(HH_PARTICLES_EXPORT)
#	undef	HH_PARTICLES_EXPORT
#	define	HH_PARTICLES_EXPORT					HH_API_EXPORT
#elif	defined(HH_PARTICLES_STATIC)
#	define	HH_PARTICLES_EXPORT
#else
#	define	HH_PARTICLES_EXPORT					HH_API_IMPORT
#endif

// PK_BILLBOARDER
#if	defined(PK_BILLBOARDER_EXPORT)
#	undef	PK_BILLBOARDER_EXPORT
#	define	PK_BILLBOARDER_EXPORT				HH_API_EXPORT
#elif	defined(PK_BILLBOARDER_STATIC)
#	define	PK_BILLBOARDER_EXPORT
#else
#	define	PK_BILLBOARDER_EXPORT				HH_API_IMPORT
#endif

//----------------------------------------------------------------------------
// POPCORN EDITOR

// HH_ENGINE
#if	defined(HH_ENGINE_EXPORT)
#	undef	HH_ENGINE_EXPORT
#	define	HH_ENGINE_EXPORT					HH_API_EXPORT
#elif	defined(HH_ENGINE_STATIC)
#	define	HH_ENGINE_EXPORT
#else
#	define	HH_ENGINE_EXPORT					HH_API_IMPORT
#endif

// HH_COMMON
#if	defined(HH_COMMON_EXPORT)
#	undef	HH_COMMON_EXPORT
#	define	HH_COMMON_EXPORT					HH_API_EXPORT
#elif	defined(HH_COMMON_STATIC)
#	define	HH_COMMON_EXPORT
#else
#	define	HH_COMMON_EXPORT					HH_API_IMPORT
#endif

// HH_NATIVE_EDITOR
#if	defined(HH_NATIVE_EDITOR_EXPORT)
#	undef	HH_NATIVE_EDITOR_EXPORT
#	define	HH_NATIVE_EDITOR_EXPORT				HH_API_EXPORT
#elif	defined(HH_NATIVE_EDITOR_STATIC)
#	define	HH_NATIVE_EDITOR_EXPORT
#else
#	define	HH_NATIVE_EDITOR_EXPORT				HH_API_IMPORT
#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif
