#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 03:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_FILE_H__
#define	__KR_FILE_H__

#include "hh_kernel/include/kr_containers.h"
#include "hh_kernel/include/kr_streams.h"
#include "hh_kernel/include/kr_threads_basics.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(FileStream);
HH_FORWARD_DECLARE(RefCountedMemoryBuffer);
class	IFileSystemController;
class	CFileDirectoryWalker;

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFileStats
{
private:
	friend class	CFileInternals;

	static Threads::CCriticalSection	m_FileOpenLock;
	static TArray<CString>				m_CurrentlyOpenedFiles;

	static bool							Startup();
	static void							Shutdown();

public:
	static TAtomic<hh_u32>				m_OpenedFileCountVFS;
	static TAtomic<hh_u32>				m_OpenedFileCountPACK;

	// return by copy for thread safety, this should only be called only a few times per frame for stats display and stuff anyway.
	static TArray<CString>				CurrentlyOpenedFiles() { HH_CRITICAL_SECTION_SCOPEDLOCK(m_FileOpenLock); return m_CurrentlyOpenedFiles; }
};

//----------------------------------------------------------------------------

struct	SFileTimes
{
	hh_u64	m_CreationTime;
	hh_u64	m_LastAccessTime;
	hh_u64	m_LastWriteTime;

	SFileTimes() : m_CreationTime(0), m_LastAccessTime(0), m_LastWriteTime(0) {}
	bool	Empty() const { return m_CreationTime != 0; }
	void	Clear() { m_CreationTime = 0; m_LastAccessTime = 0; m_LastWriteTime = 0; }
	bool	operator == (const SFileTimes &other) const { return m_CreationTime == other.m_CreationTime && m_LastAccessTime == other.m_LastAccessTime && m_LastWriteTime == other.m_LastWriteTime; }
	bool	operator != (const SFileTimes &other) const { return !(*this == other); }
};

//----------------------------------------------------------------------------
//
//	Packs
//
//	If a physical path has the form $BASE/path/file.ext
//	A virtual path will have the form: $MOUNT/path/file.ext
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFilePack : public CRefCountedObject
{
public:
	static const CFilePack								Invalid;

private:
	friend class					CFileInternals;

	CString							m_RawPath;
	CString							m_RawPathSlash;
	CString							m_VirtualMountPathSlash;
	hh_u32							m_RefCount;		// >= m_References.Count()
	bool							m_ReadOnly;

protected:
	CFilePack(const CString &rawPath, const CString &virtualMountingPath, bool readOnly);

public:
	~CFilePack();

	bool							ReadOnly() const { return m_ReadOnly; }
	void							SetReadOnly(bool readOnly) { m_ReadOnly = readOnly; }

	// Mount path with trailing "/":
	const CString					&VirtualMountPathSlash() const { return m_VirtualMountPathSlash; }

	const CString					&Path() const { return m_RawPath; }
	// Path() with trailing "/":
	const CString					&PathSlash() const { return m_RawPathSlash; }

	hh_u32							RefCount() const { return m_RefCount; }
};
HH_DECLARE_REFPTRCLASS(FilePack);

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFilePackPath
{
private:
	PFilePack				m_Pack;
	CString					m_PathInPack;

public:
	CFilePackPath(const PFilePack &pack, const CString &pathInPack);
	~CFilePackPath();

	static CFilePackPath	FromVirtualPath(const CString &virtualPath);
	static CFilePackPath	FromPhysicalPath(const CString &physicalPath);

	bool					Empty() const { HH_ASSERT(m_PathInPack.Empty() == (m_Pack == null)); return m_PathInPack.Empty(); }
	CFilePack				*Pack() const { return m_Pack.Get(); }
	const CString			&Path() const { return m_PathInPack; }
	CString					FullPath() const;	// will alloc strings, does a 'return Pack()->Path() + "/" + Path()'
};

//----------------------------------------------------------------------------
//
//	Global Filesystem
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFileSystem
{
public:
	enum	EAccessPolicy
	{
		Access_Read = 0,
		Access_ReadWrite,
		Access_WriteCreate,
		Access_ReadWriteCreate,

		__MaxAccessPolicies
	};

	static IFileSystemController	*Controller();
	static void						SetController(IFileSystemController *controller);

	// SBasePath
	typedef TArray<PFilePack>		TPackList;

	static const TPackList			&PrioritizedPacks();
	//static PFilePack				GetPack(CFilePack::CPackID);

	static PFilePack				LoadPack(const CString &path, const CString &virtualMountPath = CString::EmptyString, bool readOnly = false);
	static bool						UnloadPack(const CString &path);
	static void						UnloadPack(CFilePack *pack);

	static void						UnloadAllPacks();

	static PFilePack				FindPack(const CString &path);	// returns null if not loaded
	static PFilePack				FindPackAndLoadIFN(const CString &path, const CString &virtualMountPath = CString::EmptyString, bool readOnly = false);

	// almost deprecated:

	// when a file has to be created in the multipack virtual file system, the top-level pack is used.
	// if you want to temporarily override this default behaviour, you can call SetForcedFileCreationPack()
	// the forced file creation pack should be used cautiously and briefly... and you should restore it after using it
	static PFilePack				ForcedFileCreationPack();
	static void						SetForcedFileCreationPack(PFilePack pack);

	enum	EPackNotificationStatus
	{
		PackStatus_Loaded = 0,
		PackStatus_Unloaded,
	};
	typedef void (*CbPackNotifier)(void *arg, CFilePack *pack, EPackNotificationStatus status);
	static void						AddPackNotifier(CbPackNotifier callback, void *arg);
	static void						RemovePackNotifier(CbPackNotifier callback, void *arg);
};

//----------------------------------------------------------------------------

class	IFileSystemController
{
public:
	virtual ~IFileSystemController();

	virtual PFileStream				OpenStream(const CString &path, CFileSystem::EAccessPolicy mode, bool pathNotVirtual = false) = 0;
	virtual PFileStream				OpenStream(const CFilePackPath &path, CFileSystem::EAccessPolicy mode);

//	virtual PFileMapping			OpenMapping(const CString &path, CFileSystem::EAccessPolicy mode, bool pathNotVirtual = false) = 0;
//	virtual PFileMapping			OpenMapping(const CFilePackPath &path, CFileSystem::EAccessPolicy mode);

	virtual hh_u8					*Bufferize(const CString &path, hh_u32 *outputFileSize = null, bool pathNotVirtual = false);
	virtual hh_u8					*Bufferize(const CFilePackPath &path, hh_u32 *outputFileSize = null);
	virtual CString					BufferizeToString(const CString &path, bool pathNotVirtual = false);
	virtual CString					BufferizeToString(const CFilePackPath &path);

	virtual bool					Timestamps(const CString &path, SFileTimes &timestamps, bool pathNotVirtual = false);
	virtual bool					Timestamps(const CFilePackPath &path, SFileTimes &timestamps);
	virtual bool					Exists(const CString &path, bool pathNotVirtual = false) = 0;
	virtual bool					Exists(const CFilePackPath &path);
	virtual bool					FileDelete(const CString &path, bool pathNotVirtual = false) = 0;
	virtual bool					FileDelete(const CFilePackPath &path);
	virtual bool					FileCopy(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual = false) = 0;
	virtual bool					FileCopy(const CFilePackPath &sourcePath, const CFilePackPath &targetPath);
	virtual bool					FileMove(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual = false);
	virtual bool					FileMove(const CFilePackPath &sourcePath, const CFilePackPath &targetPath);

	virtual void					GetDirectoryContents(char *dpath, char *virtualPath, hh_u32 pathLength, CFileDirectoryWalker *walker, const CFilePack *pack) = 0;
	virtual bool					CreateDirectoryChainIFN(const CString &directoryPath, bool pathNotVirtual = false) = 0;
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFilePath
{
public:
	// Without trailing "/"
	// "Toto/Whatever/Hello.pkfx" --> "Toto/Whatever"
	// "" -> ""
	// "/" -> ""
	// "Hello/" -> "Hello"
	// "Hello" -> ""
	static void								StripFilenameInPlace(char *path);
	static void								StripFilenameInPlace(CString &path) { StripFilenameInPlace(path.RawDataForWriting()); path.RebuildAfterRawModification(); }

	// With trailing "/"
	// "Toto/Whatever/Hello.pkfx" --> "Toto/Whatever/"
	// "" -> ""
	// "/" -> "/"
	// "Hello/" -> "Hello/"
	// "Hello" -> ""
	static PK_WARN_UNUSED_RETURN CString	StripFilename(const CString &path);
	static PK_WARN_UNUSED_RETURN CString	ExtractFilename(const CString &path);

	// "Toto/Whatever/Hello.pkfx"	--> "Toto/Whatever/Hello"
	// "Toto/Whatever/Hello"		--> "Toto/Whatever/Hello"
	// "Toto/Whatever.hohoho/"		--> "Toto/Whatever.hohoho/"
	static PK_WARN_UNUSED_RETURN CString	StripExtension(const CString &path);
	static void								StripExtensionInPlace(char *path);
	static void								StripExtensionInPlace(CString &path) { StripExtensionInPlace(path.RawDataForWriting()); path.RebuildAfterRawModification(); }
	static PK_WARN_UNUSED_RETURN const char	*ExtractExtension(const char *path, hh_u32 length = 0);
	static PK_WARN_UNUSED_RETURN const char	*ExtractExtension(const CString &path) { return ExtractExtension(path.Data(), path.Length()); }
	static PK_WARN_UNUSED_RETURN CGuid		FindExtension(const char *path, hh_u32 length = 0);
	static PK_WARN_UNUSED_RETURN CGuid		FindExtension(const CString &path) { return FindExtension(path.Data(), path.Length()); }

	static void				Purify(char *path);
	static void				Purify(CString &path);
	static void				PurifyAndStripExtension(char *path);
	static void				PurifyAndStripExtension(CString &path);

	static bool				IsPure(const char *path);
	static bool				IsPure(const CString &path) { return IsPure(path.Data()); }

	static CString			RelativizePure(const char *base, const char *path);
	static CString			Relativize(const char *base, const char *path);

	static CString			PhysicalToVirtual(const CString &physicalPath, PFilePack *outPack = null);
	static CString			PhysicalToVirtual_Pure(const CString &physicalPath, PFilePack *outPack = null);
	static CString			VirtualToPhysical(const CString &virtualPath, CFileSystem::EAccessPolicy mode, PFilePack *outPack = null);
	static CString			VirtualToPhysical_Pure(const CString &virtualPath, CFileSystem::EAccessPolicy mode, PFilePack *outPack = null);
};

//----------------------------------------------------------------------------
//
//	Standard file stream
//
//----------------------------------------------------------------------------

struct	CDigestMD5;		// #include <hh_kernel/include/kr_checksum>
struct	CDigestSHA1;

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CFileStream : public CStream
{
private:
protected:
	PFilePack									m_Pack;
	CString										m_Path;
	CFileSystem::EAccessPolicy					m_Mode;

	CFileStream(PFilePack pack, const CString &path, CFileSystem::EAccessPolicy mode);

public:
	virtual ~CFileStream();

	hh_u8										*Bufferize(hh_u32 &outSize);
	PRefCountedMemoryBuffer						BufferizeToRefCountedMemoryBuffer();
	CString										BufferizeToString();

	bool										Digest(CDigestMD5 &outout);
	bool										Digest(CDigestSHA1 &outout);
	hh_u32										DigestCRC32(const hh_u32 *lookup = null);
	hh_u16										DigestCRC16(const hh_u16 *lookup = null);
	hh_u8										DigestCRC8(const hh_u8 *lookup = null);

	// FIXME: now, m_Path stores the full, real path, we should make this explicit, with, for example, a RawPath() or something... see where and what Path() is used for...
	HH_FORCEINLINE const CString				&Path() const { return m_Path; }
	HH_FORCEINLINE CString						&Path() { return m_Path; }
	HH_FORCEINLINE CFileSystem::EAccessPolicy	Mode() { return m_Mode; }
	HH_FORCEINLINE PFilePack					Pack() const { return m_Pack; }

	virtual bool								Flush() = 0;
	virtual void								Timestamps(SFileTimes &timestamps) = 0;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif
