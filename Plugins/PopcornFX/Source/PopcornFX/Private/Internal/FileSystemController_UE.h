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

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_file.h>

class CFileSystemController_UE;
class UPopcornFXFile;
extern CFileSystemController_UE		*g_FsController;

class	CFileStreamFS_UE : public PopcornFX::CFileStream
{
private:
	CFileStreamFS_UE(
		PopcornFX::PFilePack					pack,
		const PopcornFX::CString				&path,
		PopcornFX::CFileSystem::EAccessPolicy	mode,
		UPopcornFXFile							*file);

public:
	virtual ~CFileStreamFS_UE();

	static CFileStreamFS_UE	*Open(const PopcornFX::CString &path, bool pathNotVirtual, PopcornFX::CFileSystem::EAccessPolicy mode);

	virtual hh_u64		Read(void *targetBuffer, hh_u64 byteCount) override;
	virtual hh_u64		Write(const void *sourceBuffer, hh_u64 byteCount) override;
	virtual bool		Seek(hh_i64 offset, ESeekMode whence) override;
	virtual hh_u64		Tell() const override;
	virtual bool		Eof() const override;
	virtual bool		Flush() override;
	virtual void		Close() override;
	virtual hh_u64		SizeInBytes() const override;
	virtual void		Timestamps(PopcornFX::SFileTimes &timestamps) override;

private:
	PopcornFX::CFileSystem::EAccessPolicy	m_Mode;
	UPopcornFXFile		*m_File;
	hh_u64				m_Pos;
};
HH_DECLARE_REFPTRCLASS(FileStreamFS_UE);

class	CFileSystemController_UE : public PopcornFX::IFileSystemController
{
public:
	static UObject		*LoadUObject(const PopcornFX::CString &path, bool pathNotVirtual);
	static UPopcornFXFile		*FindDirectPopcornFXFile(UObject *uobject);

public:
	CFileSystemController_UE();
	~CFileSystemController_UE();

	virtual PopcornFX::PFileStream	OpenStream(const PopcornFX::CString &path, PopcornFX::CFileSystem::EAccessPolicy mode, bool pathNotVirtual) override;
	virtual bool					Exists(const PopcornFX::CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileDelete(const PopcornFX::CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileCopy(const PopcornFX::CString &sourcePath, const PopcornFX::CString &targetPath, bool pathsNotVirtual = false) override;
	virtual void					GetDirectoryContents(char *dpath, char *virtualPath, hh_u32 pathLength, PopcornFX::CFileDirectoryWalker *walker, const PopcornFX::CFilePack *pack) override;
	virtual bool					CreateDirectoryChainIFN(const PopcornFX::CString &directoryPath, bool pathNotVirtual = false) override;
};
