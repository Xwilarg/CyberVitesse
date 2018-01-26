#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/04/02 17:17 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_FILE_H__
#define __HBO_FILE_H__

#include "hh_base_object/include/hbo_object.h"
#include "hh_base_object/include/hbo_config.h"

#ifndef	HBO_FILES_COMPUTE_DIGEST
#	define	HBO_FILES_COMPUTE_DIGEST	0
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if	(HH_BASE_OBJECT_HANDLE_UNKNOWN != 0)
namespace	HBO
{
	class	CUnknownClass;		// defined in hbo_unknown.h
}
#endif

//----------------------------------------------------------------------------

struct	SEngineVersion
{
	hh_u32	m_Major;
	hh_u32	m_Minor;
	hh_u32	m_Patch;
	hh_u32	m_Revision;

	SEngineVersion() : m_Major(0), m_Minor(0), m_Patch(0), m_Revision(0) {}

	SEngineVersion(hh_u32 ma, hh_u32 mi, hh_u32 pa, hh_u32 rev)
	:	m_Major(ma)
	,	m_Minor(mi)
	,	m_Patch(pa)
	,	m_Revision(rev)
	{}

	bool		Empty() const { return m_Major == 0 && m_Minor == 0 && m_Patch == 0 && m_Revision == 0; }
	bool		Valid() const { return !Empty(); }
	CString		ToS() const { return CString::Format("%d.%d.%d.%d", m_Major, m_Minor, m_Patch, m_Revision);}

	bool		operator < (const SEngineVersion &o) const
	{
		if (m_Major != o.m_Major)
			return m_Major < o.m_Major;
		if (m_Minor != o.m_Minor)
			return m_Minor < o.m_Minor;
		if (m_Patch != o.m_Patch)
			return m_Patch < o.m_Patch;
		return m_Revision < o.m_Revision;
	}
	bool		operator > (const SEngineVersion &o) const
	{
		if (m_Major != o.m_Major)
			return m_Major > o.m_Major;
		if (m_Minor != o.m_Minor)
			return m_Minor > o.m_Minor;
		if (m_Patch != o.m_Patch)
			return m_Patch > o.m_Patch;
		return m_Revision > o.m_Revision;
	}

	bool		operator == (const SEngineVersion &o) const { return m_Major == o.m_Major && m_Minor == o.m_Minor && m_Patch == o.m_Patch && m_Revision == o.m_Revision; }
	bool		operator != (const SEngineVersion &o) const { return m_Major != o.m_Major || m_Minor != o.m_Minor || m_Patch != o.m_Patch || m_Revision != o.m_Revision; }

	bool		operator >= (const SEngineVersion &o) const { return !(*this < o); }
	bool		operator <= (const SEngineVersion &o) const { return !(*this > o); }

};

//----------------------------------------------------------------------------

class	HH_BASEOBJECT_EXPORT CBaseObjectFile : public CRefCountedObject
{
private:
	friend class						HBO::CContext;
	friend class						CBaseObject;	// beeerk :(

public:
	CString								m_Path;
	CString								m_LastValidPhysicalPath;
	TArray<PBaseObject>					m_ObjectList;
#if	(HH_BASE_OBJECT_HANDLE_UNKNOWN != 0)
	TArray<HBO::CUnknownClass>			m_UnknownObjects;
#endif
	HBO::CContext						*m_Context;
#if	(HBO_FILES_COMPUTE_DIGEST != 0)
	CDigestMD5							m_Digest;
#endif
	bool								m_IsBinary;
	bool								m_Serializable;
	SEngineVersion						m_Version;

private:
	mutable void						*m_InternalUserData;

protected:
public:
	bool								_AddVirginObject(CBaseObject &object);
	bool								_RawRemoveObject(CBaseObject &object);

public:
	CBaseObjectFile(HBO::CContext *context = HBO::g_Context, bool binary = false);
	~CBaseObjectFile();

	HBO::CContext						*Context() const { return m_Context; }

	bool								Modified() const { return NumberOfModifiedObjects() != 0; }
	bool								Serializable() const { return m_Serializable; }
	void								SetSerializable(bool serializable) { m_Serializable = serializable; }
	void								ForceModificationStatusForAllObjects(bool modified);
	hh_u32								NumberOfModifiedObjects() const;
	const CString						&Path() const { return m_Path; }
	bool								SetFilePath(const CString &path);

	bool								Binary() const { return m_IsBinary; }
	void								SetBinary(bool isBinary) { m_IsBinary = isBinary; }

	bool								Write(const CString &rawPath = CString::EmptyString, bool pathNotVirtual = false, bool removeOldFileIfPathDifferent = true);
	bool								Write(const CFilePackPath &packPath, bool removeOldFileIfPathDifferent = true) { HH_ASSERT(!packPath.Empty()); return Write(packPath.FullPath(), true, removeOldFileIfPathDifferent); }

	void								Clear();
	void								Unload();	// unloads the file

	const TArray<PBaseObject>			&ObjectList() const { return m_ObjectList; }

#if	(HH_BASE_OBJECT_HANDLE_UNKNOWN != 0)
	const TArray<HBO::CUnknownClass>	&UnknownObjects() const { return m_UnknownObjects; }
	TArray<HBO::CUnknownClass>			&UnknownObjects() { return m_UnknownObjects; }
#endif

	// null initialized user data
	void								SetInternalUserData(void *userData) const { m_InternalUserData = userData; }
	void								*InternalUserData() const { return m_InternalUserData; }

};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_FILE_H__
