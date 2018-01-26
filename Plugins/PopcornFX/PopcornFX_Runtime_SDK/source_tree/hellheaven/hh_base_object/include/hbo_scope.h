#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/08/05 17:11 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_EXEC_SCOPE_H__
#define __HBO_EXEC_SCOPE_H__

#include "hh_kernel/include/kr_profiler.h" // HH_DEFAULT_PROFILE_LOCATION

#ifndef	HBO_EXEC_SCOPE_ENABLED
#	ifdef	HH_RETAIL
#		define	HBO_EXEC_SCOPE_ENABLED		0
#	else
#		if	!defined(HH_PS3) /*&& !defined(HH_X360)*/ && !defined(HH_PSP2)
#			define	HBO_EXEC_SCOPE_ENABLED	1
#		else
#			define	HBO_EXEC_SCOPE_ENABLED	0
#		endif
#	endif
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define HBO_SCOPEDNAMED_ACTIVE(__baseobject, __name, __uniq)											\
	static const PopcornFX::HBOScope::SNodeDesctriptor		GLUE(__HBO_EXEC_NODE_DESC_, __uniq)			\
		(__name, HH_DEFAULT_PROFILE_LOCATION, __LINE__);												\
	const PopcornFX::HBOScope::SScopedRecordNode			GLUE(__HBO_EXEC_RECORD_NODE_, __uniq)		\
		(PopcornFX::HBOScope::MainContext(), &(GLUE(__HBO_EXEC_NODE_DESC_, __uniq)), (__baseobject));

//----------------------------------------------------------------------------

#if	(HBO_EXEC_SCOPE_ENABLED != 0)
#	define HBO_SCOPEDNAMED(__baseobject, __name)		HBO_SCOPEDNAMED_ACTIVE(__baseobject, __name, __COUNTER__)
#else
#	define HBO_SCOPEDNAMED(__baseobject, __name)
#endif

#define HBO_SCOPED(__baseobject)						HBO_SCOPEDNAMED(__baseobject, HH_DEFAULT_PROFILE_NAME)

//----------------------------------------------------------------------------

class	CBaseObject;

namespace	HBOScope
{
	class	CContext;
	class	CRecord;
	struct	SNodeRecord;
	struct	SThreadNodeRecords;

	struct	SNodeDesctriptor
	{
		const char		*m_Name;
		const char		*m_FileAndFunction;
		hh_u32			m_Line;
		SNodeDesctriptor(const char *name, const char *func, hh_u32 line) : m_Name(name), m_FileAndFunction(func), m_Line(line) {}
	};

	struct	SRecordingNodeId
	{
		CContext									*m_Context;
		hh_u32										m_ThreadId;

		SRecordingNodeId() : m_Context(null) { }
		HH_FORCEINLINE bool			Valid() const { return m_Context != null; }
	};

	struct	SScopedRecordNode
	{
		SRecordingNodeId	m_RecordingNodeId;

		SScopedRecordNode(CContext *context, const SNodeDesctriptor *descriptor, const CBaseObject &baseObject);
		SScopedRecordNode(CContext *context, const SNodeDesctriptor *descriptor, const CBaseObject *baseObject);
		~SScopedRecordNode();
	};

	CContext			*MainContext();

	extern CContext		*g_MainContext;

} // namespace	HBOScope

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_EXEC_SCOPE_H__
