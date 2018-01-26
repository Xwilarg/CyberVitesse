//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "FileSystemController_UE.h"

#include "PopcornFXPlugin.h"
#include "Assets/PopcornFXFile.h"
#include "Assets/PopcornFXMesh.h"
#include "Assets/PopcornFXStaticMeshData.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"

#define FILE_ASSERT		HH_RELEASE_ASSERT

CFileSystemController_UE	*g_FsController = null;

using PopcornFX::CString;
using PopcornFX::CFileSystem;
using PopcornFX::CLog;
using PopcornFX::HHMin;
using PopcornFX::HHMax;

//#define	FS_DEBUG_LOG		CLog::Log
#define	FS_DEBUG_LOG(...)		do {} while(0)

//----------------------------------------------------------------------------

// static
UObject			*CFileSystemController_UE::LoadUObject(const CString &path, bool pathNotVirtual)
{
	UObject		*object = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(path.Data(), pathNotVirtual);
	FS_DEBUG_LOG(HH_INFO, "FileSystem LoadUObject %s > %p", path.Data(), object);
	return object;
}

//----------------------------------------------------------------------------

//static
UPopcornFXFile		*CFileSystemController_UE::FindDirectPopcornFXFile(UObject *uobject)
{
	UPopcornFXFile	*file = Cast<UPopcornFXFile>(uobject);
	if (file != null)
		return file;
	UStaticMesh			*staticMesh = Cast<UStaticMesh>(uobject);
	if (staticMesh != null)
	{
		UPopcornFXMesh		*mesh = UPopcornFXMesh::FindStaticMesh(staticMesh);
		return mesh;
	}
	USkeletalMesh		*skelMesh = Cast<USkeletalMesh>(uobject);
	if (skelMesh != null)
	{
		UPopcornFXMesh		*mesh = UPopcornFXMesh::FindSkeletalMesh(skelMesh);
		return mesh;
	}
	return null;
}

//----------------------------------------------------------------------------
//
// CFileSystemController_UE
//
//----------------------------------------------------------------------------

CFileStreamFS_UE::CFileStreamFS_UE(
	PopcornFX::PFilePack		pack,
	const CString				&path,
	CFileSystem::EAccessPolicy	mode,
	UPopcornFXFile				*file)
	: CFileStream(pack, path, mode)
	, m_Mode(mode)
	, m_File(file)
	, m_Pos(0)
{
	FILE_ASSERT(m_File != null);
	FS_DEBUG_LOG(HH_INFO, "ctor CFileStreamFS_UE %s filesize:%d", Path().Data(), CFileStreamFS_UE::SizeInBytes());
	if (m_File != null &&
		(m_Mode == CFileSystem::Access_WriteCreate || m_Mode == CFileSystem::Access_ReadWriteCreate))
	{
		hh_u32		oldNum = m_File->FileDataForWriting().Num();
		m_File->FileDataForWriting().Empty(oldNum);
	}
}

//----------------------------------------------------------------------------

CFileStreamFS_UE::~CFileStreamFS_UE()
{
	if (m_File != null &&
		(m_Mode == CFileSystem::Access_WriteCreate || m_Mode == CFileSystem::Access_ReadWriteCreate))
		m_File->MarkPackageDirty();
	FS_DEBUG_LOG(HH_INFO, "dtor CFileStreamFS_UE %s", Path().Data());
}

//----------------------------------------------------------------------------

hh_u64	CFileStreamFS_UE::Read(void *targetBuffer, hh_u64 byteCount)
{
	const hh_u64		totalBytes = CFileStreamFS_UE::SizeInBytes();
	const hh_u64		start = m_Pos;
	if (!HH_VERIFY(start + byteCount <= totalBytes))
		return 0;

	PopcornFX::Mem::Copy(targetBuffer, &(m_File->FileData()[start]), byteCount);
	m_Pos += byteCount;
	return byteCount;
}

//----------------------------------------------------------------------------

hh_u64	CFileStreamFS_UE::Write(const void *sourceBuffer, hh_u64 byteCount)
{
	// only in that case you have cleared the FileData();
	FILE_ASSERT(m_Mode == CFileSystem::Access_WriteCreate || m_Mode == CFileSystem::Access_ReadWriteCreate);

	TArray<hh_u8>			&fileData = m_File->FileDataForWriting();
	if (m_Pos + byteCount > fileData.Num())
		fileData.SetNumUninitialized(m_Pos + byteCount);

	check(fileData.Num() >= m_Pos + byteCount);

	PopcornFX::Mem::Copy(&(fileData[m_Pos]), sourceBuffer, byteCount);
	m_Pos += byteCount;
	return byteCount;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_UE::Seek(hh_i64 offset, ESeekMode whence)
{
	hh_i64		i = hh_i32(whence == SeekSet ? 0 : m_Pos) + offset;
	FILE_ASSERT(i > 0 && i <= hh_i32(CFileStreamFS_UE::SizeInBytes()));
	m_Pos = hh_u64(HHMin(HHMax(i, 0), hh_i64(CFileStreamFS_UE::SizeInBytes())));
	return true;
}

//----------------------------------------------------------------------------

hh_u64	CFileStreamFS_UE::Tell() const
{
	return m_Pos;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_UE::Eof() const
{
	return m_Pos >= m_File->FileData().Num();
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_UE::Flush()
{
	HH_ASSERT_NOT_REACHED();
	return true;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_UE::Close()
{
	m_Pos = 0;
}

//----------------------------------------------------------------------------

hh_u64	CFileStreamFS_UE::SizeInBytes() const
{
	return m_File->FileData().Num();
}

//----------------------------------------------------------------------------

void	CFileStreamFS_UE::Timestamps(PopcornFX::SFileTimes &timestamps)
{
	HH_ASSERT_NOT_REACHED();
}

//----------------------------------------------------------------------------

CFileStreamFS_UE	*CFileStreamFS_UE::Open(const CString &path, bool pathNotVirtual, CFileSystem::EAccessPolicy mode)
{
	CFileStreamFS_UE		*stream = null;
	UObject					*uobject = CFileSystemController_UE::LoadUObject(path, pathNotVirtual);
	if (uobject != null)
	{
		UPopcornFXFile	*file = CFileSystemController_UE::FindDirectPopcornFXFile(uobject);
		if (file != null)
		{
			if (file->IsFileValid())
				stream = HH_NEW(CFileStreamFS_UE(FPopcornFXPlugin::Get().FilePack(), path, mode, file));
			else
			{
				CLog::Log(HH_WARNING, "CFileStreamFS_UE: Open: invalid UPopcornFXFile '%s'", path.Data());
				//HH_ASSERT_NOT_REACHED();
			}
		}
		else
		{
			CLog::Log(HH_WARNING, "CFileStreamFS_UE: Open: cannot open '%s'", path.Data()); // This error may be generated if objects have the same path
			//HH_ASSERT_NOT_REACHED();
		}
	}
	else
		CLog::Log(HH_WARNING, "CFileStreamFS_UE: Open: file not found '%s'", path.Data()); // This error may be generated if objects have the same path
	return stream;
}

//----------------------------------------------------------------------------
//
// CFileSystemController_UE
//
//----------------------------------------------------------------------------

CFileSystemController_UE::CFileSystemController_UE()
{
	
}

//----------------------------------------------------------------------------

CFileSystemController_UE::~CFileSystemController_UE()
{
	
}

//----------------------------------------------------------------------------

PopcornFX::PFileStream		CFileSystemController_UE::OpenStream(const CString &path, CFileSystem::EAccessPolicy mode, bool pathNotVirtual)
{
	CFileStreamFS_UE	*fs = CFileStreamFS_UE::Open(path, pathNotVirtual, mode);
	if (fs != null)
		FS_DEBUG_LOG(HH_INFO, "CFileSystemController_UE OpenStream OK '%s' pathNotVirtual:%d", path.Data(), pathNotVirtual);
	else
		CLog::Log(HH_WARNING, "CFileSystemController_UE OpenStream FAIL '%s' pathNotVirtual:%d", path.Data(), pathNotVirtual);
	return fs;
}

//----------------------------------------------------------------------------

bool			CFileSystemController_UE::Exists(const CString &path, bool pathNotVirtual /*= false*/)
{
	UObject			*uobject = LoadUObject(path, pathNotVirtual);
	if (uobject != null)
		FS_DEBUG_LOG(HH_INFO, "CFileSystemController_UE Exists OK '%s' pathNotVirtual:%d", path.Data(), pathNotVirtual);
	else
		CLog::Log(HH_WARNING, "CFileSystemController_UE Exists FAIL '%s' pathNotVirtual:%d", path.Data(), pathNotVirtual);
	return uobject != null;
}

//----------------------------------------------------------------------------

bool			CFileSystemController_UE::FileDelete(const CString &path, bool pathNotVirtual /*= false*/)
{
	HH_ASSERT_NOT_REACHED();
	return false;
}

//----------------------------------------------------------------------------


bool			CFileSystemController_UE::FileCopy(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual /*= false*/)
{
	HH_ASSERT_NOT_REACHED();
	return false;
}

//----------------------------------------------------------------------------


void			CFileSystemController_UE::GetDirectoryContents(char *dpath, char *virtualPath, hh_u32 pathLength, PopcornFX::CFileDirectoryWalker *walker, const PopcornFX::CFilePack *pack)
{
	HH_ASSERT_NOT_REACHED();
	return;
}

//----------------------------------------------------------------------------


bool			CFileSystemController_UE::CreateDirectoryChainIFN(const CString &directoryPath, bool pathNotVirtual /*= false*/)
{
	HH_ASSERT_NOT_REACHED();
	return false;
}

//----------------------------------------------------------------------------
