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

#ifndef	__GE_CONFIG_H__
#define	__GE_CONFIG_H__

//----------------------------------------------------------------------------
// Billboards

#ifndef	HH_GEOMETRICS_BUILD_BILLBOARDS
#	define	HH_GEOMETRICS_BUILD_BILLBOARDS	1
#endif

//----------------------------------------------------------------------------
// Mesh

#ifndef	HH_GEOMETRICS_BUILD_MESH_DEFORMERS
#	if	!defined(HH_PS3) && !defined(HH_X360)
#		define	HH_GEOMETRICS_BUILD_MESH_DEFORMERS	1
#	else
#		define	HH_GEOMETRICS_BUILD_MESH_DEFORMERS	0
#	endif
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESH_WRITERS
#	if	defined(HH_DESKTOP)
#		define	HH_GEOMETRICS_BUILD_MESH_WRITERS	1	// only on desktop (editor / tools build)
#	else
#		define	HH_GEOMETRICS_BUILD_MESH_WRITERS	0
#	endif
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESH_IMPORTERS
#	if	defined(HH_DESKTOP)
#		define	HH_GEOMETRICS_BUILD_MESH_IMPORTERS	1	// only on desktop (editor / tools build)
#	else
#		define	HH_GEOMETRICS_BUILD_MESH_IMPORTERS	0
#	endif
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESH_INSPECTOR
#	if	defined(HH_DESKTOP)
#		define	HH_GEOMETRICS_BUILD_MESH_INSPECTOR	1	// only on desktop (editor / tools build)
#	else
#		define	HH_GEOMETRICS_BUILD_MESH_INSPECTOR	0
#	endif
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE
#	define	HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE	1
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME
#	define	HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME		1
#endif

#ifndef	HH_GEOMETRICS_BUILD_MESHPROJECTION
#	define	HH_GEOMETRICS_BUILD_MESHPROJECTION			1
#endif

//----------------------------------------------------------------------------
// Kd-Tree

#ifndef	HH_GEOMETRICS_BUILD_KDTREE
#	define	HH_GEOMETRICS_BUILD_KDTREE	(HH_GEOMETRICS_BUILD_MESHPROJECTION /*| ...*/)
#endif

#ifndef	HH_GEOMETRICS_BUILD_KDTREE_COMPILER
#	define	HH_GEOMETRICS_BUILD_KDTREE_COMPILER	HH_GEOMETRICS_BUILD_KDTREE
#endif

#ifndef	HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER
#	if	defined(HH_RETAIL) || !defined(HH_DESKTOP)
#		define	HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER	0
#	else
#		define	HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER	HH_GEOMETRICS_BUILD_KDTREE	// only on PC / debug|release
#	endif
#endif

//----------------------------------------------------------------------------
// post-check KdTree dependencies
// This is in case the meshprojection wasn't explicitly overridden, but the KdTree was explicitely disabled

#if	(HH_GEOMETRICS_BUILD_KDTREE != HH_GEOMETRICS_BUILD_MESHPROJECTION)
#	error	Configuration error: mismatching Kd-Tree and meshprojection enable-flags. If explicit overrides are used, you must explicitely activate or deactivate both. see your sdk_config headers in hellheaven/include/license/$(CompanyName).
#endif

//----------------------------------------------------------------------------
// Colliders

#ifndef	HH_TRACE_DEBUG_ENABLED
#	ifndef	HH_RETAIL
#		define	HH_TRACE_DEBUG_ENABLED	1
#	else
#		define	HH_TRACE_DEBUG_ENABLED	0
#	endif
#endif


//----------------------------------------------------------------------------
// Shapes

#ifndef	GE_SHAPE_SPHERE_ENABLE_UNIFORM_VOLUME_SAMPLES
#	define	GE_SHAPE_SPHERE_ENABLE_UNIFORM_VOLUME_SAMPLES	1
#endif

//----------------------------------------------------------------------------

#endif	// __GE_CONFIG_H__
