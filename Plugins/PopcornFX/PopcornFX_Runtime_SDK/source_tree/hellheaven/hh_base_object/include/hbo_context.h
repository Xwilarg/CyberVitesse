#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/04/02 17:27 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_CONTEXT_H__
#define __HBO_CONTEXT_H__

#include "hh_base_object/include/hbo_object.h"
#include "hh_kernel/include/kr_containers_hash.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBaseObjectContextInternals;

namespace	HBO
{

	enum	EContextAction
	{
		Action_Modified = 0,
		Action_Added,
		Action_Removed,
		Action_Renamed,
	};

	class	CContextCallbacks;

	//----------------------------------------------------------------------------
	//
	//	contexts
	//
	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CContext
	{
	private:
		friend class							::PopcornFX::CBaseObjectContextInternals;

		THashMap<CBaseObject*, CString>			m_ObjectList;
		Threads::CCriticalSection				m_ObjectListLock;

		THashMap<PBaseObjectFile, CString>		m_FileList;
		Threads::CCriticalSection				m_FileListLock;

		TArray<CContextCallbacks*>				m_ModificationCallbacks;

		bool									RegisterObject(CBaseObject *object, bool enableNotifications = true);
		bool									UnregisterObject(CBaseObject *object, bool enableNotifications = true);

	protected:
	public:
		CContext();
		virtual ~CContext();

		bool									RegisterFile(CBaseObjectFile *file);
		bool									UnregisterFile(CBaseObjectFile *file);

		void									Clear();

		bool									Empty() const;

		bool									AddModificationCallbacks(CContextCallbacks *cb, bool notifyAllExistingObjects = true);
		bool									RemoveModificationCallbacks(CContextCallbacks *cb, bool notifyAllRemainingObjects = true);

		PBaseObjectFile							FindFile(const CString &filePath);
		virtual PBaseObjectFile					LoadFile(const CString &path, bool reload = true);
		virtual PBaseObjectFile					LoadFile_AndCreateIFN(const CString &path, bool reload = true);

		bool									ContainsBaseObject(const CString &objectName) const;
		PBaseObject								FindBaseObject(const CString &objectName) const;
		PBaseObject								LoadBaseObject(const CString &objectName);
		PBaseObject								LoadBaseObject(const CStringView &objectName);
		template<typename _Type>
		TRefPtr<_Type>							FindObject(const CString &objectName);			// returns null if not found or if types mismatch
		template<typename _Type>
		TRefPtr<_Type>							LoadObject(const CString &objectName);			// returns null if not found/loaded or if types mismatch
		template<typename _Type>
		TRefPtr<_Type>							FindObjectStrict(const CString &objectName);	// same as FindObject(), but asserts if the real type differs from _Type
		template<typename _Type>
		TRefPtr<_Type>							LoadObjectStrict(const CString &objectName);	// same as LoadObject(), but asserts if the real type differs from _Type

		bool									RefreshObjectFromName(CBaseObject *object, const CString &objectName);

		void									ListObjects(void (*callback)(void *arg, CBaseObject* &object, const CString &uniqueInternalKey), void *arg) const;
		void									ListFiles(void (*callback)(void *arg, PBaseObjectFile &file, const CString &uniqueInternalKey), void *arg) const;
		void									ListObjects(TArray<TPair<CString, CBaseObject**> > &objectList) const;
		void									ListFiles(TArray<TPair<CString, PBaseObjectFile*> > &fileList) const;

		void									NotifyObjectModificationCallback(CBaseObject *object, const char *oldEntry, const char *newEntry, EContextAction action);
		void									NotifyFieldModificationCallback(CBaseObject *object, hh_u32 fieldId);

		template<typename _Type>
		TRefPtr<_Type>							NewObject(	CBaseObjectFile *parentFile,
															const PopcornFX::CString &name = PopcornFX::CString(),
															CBaseObjectEditorInfos *editorInfos = null)
		{
			HH_RELEASE_ASSERT_MESSAGE(_Type::m_Handler != null, "HBO handler invoked without prior call to RegisterHandler() !");

			PBaseObject	o = _Type::m_Handler->NewObject(this, parentFile, name, editorInfos, true);
			return static_cast<_Type*>(o.Get());
		}
	};

	//----------------------------------------------------------------------------
	//
	//	Implementation
	//
	//----------------------------------------------------------------------------

	template<typename _Type>	// returns null if not found or if types mismatch
	TRefPtr<_Type>			CContext::FindObject(const CString &objectName)
	{
		return Cast<_Type>(FindBaseObject(objectName));
	}

	template<typename _Type>	// returns null if not found/loaded or if types mismatch
	TRefPtr<_Type>			CContext::LoadObject(const CString &objectName)
	{
		return Cast<_Type>(LoadBaseObject(objectName));
	}

	template<typename _Type>	// same as FindObject(), but asserts if the real type differs from _Type
	TRefPtr<_Type>			CContext::FindObjectStrict(const CString &objectName)
	{
		PBaseObject		hbo = FindBaseObject(objectName);
		if (hbo == null)
			return null;
		TRefPtr<_Type>	typedHbo = Cast<_Type>(hbo);
		HH_ASSERT_MESSAGE(typedHbo != null, "Unexpected HBO type");
		return typedHbo;
	}

	template<typename _Type>	// same as LoadObject(), but asserts if the real type differs from _Type
	TRefPtr<_Type>			CContext::LoadObjectStrict(const CString &objectName)
	{
		PBaseObject		hbo = LoadBaseObject(objectName);
		if (hbo == null)
			return null;
		TRefPtr<_Type>	typedHbo = Cast<_Type>(hbo);
		HH_ASSERT_MESSAGE(typedHbo != null, "Unexpected HBO type");
		return typedHbo;
	}

}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_CONTEXT_H__
