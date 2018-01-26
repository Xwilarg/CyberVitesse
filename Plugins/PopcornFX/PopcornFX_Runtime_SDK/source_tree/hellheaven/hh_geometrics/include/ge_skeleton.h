#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2004/04/06 7:13 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_SKELETON_H__
#define	__GE_SKELETON_H__

#include "hh_maths/include/hh_maths.h"
#include "hh_maths/include/hh_maths_quaternion.h"
#include "hh_kernel/include/kr_checksum.h"
#include "hh_geometrics/include/ge_coordinate_frame.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CSkeletonView
{
private:
	hh_u32							m_BoneCount;
	CFloat4x4						*m_Matrices;
	CFloat4x4						*m_InverseMatrices;

public:
	CSkeletonView(hh_u32 count, CFloat4x4 *m, CFloat4x4 *im) : m_BoneCount(count), m_Matrices(m), m_InverseMatrices(im)
	{
		HH_ASSERT_ALIGNED16(m_Matrices);
		HH_ASSERT_ALIGNED16(m_InverseMatrices);
	}

	hh_u32							BoneCount() const { return m_BoneCount; }
	const CFloat4x4					*Matrices() const { return m_Matrices; }
	const CFloat4x4					*InverseMatrices() const { return m_InverseMatrices; }
};

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(Skeleton);

class	HH_GEOMETRICS_EXPORT CSkeleton : public CRefCountedObject
{
public:
	struct	SBoneRecord
	{
		CQuaternion						m_Orientation;
		CFloat3							m_Translations;
		CFloat3							m_Scale;
		CGuid							m_Parent;
		CString							m_Name;
		TSemiDynamicArray<hh_u32, 4>	m_Childs;
//		CFloat3							m_Translations;	// extract from 'm_Matrix'
//		CQuaternion						m_Rotations;	// extract from 'm_Matrix'

		bool	operator == (const SBoneRecord &other) const;
		bool	operator != (const SBoneRecord &other) const { return !(*this == other); }
	};

private:
protected:
	friend class	CSkeletonState;
	CSkeleton();

	TArray<SBoneRecord>					m_Bones;
	TArray<CFloat4x4, TArrayAligned16>	m_Matrices;
	TArray<CFloat4x4, TArrayAligned16>	m_InverseMatrices;
	CDigestMD5							m_HierarchicalDigest;

	void							RecursiveBaseMatrixBuilder(hh_u32 bone, const CFloat4x4 &parent);

public:
	CSkeleton(const TMemoryView<const SBoneRecord> &src);
	~CSkeleton();

	bool							PopulateStringTableBeforeWrite(TArray<CString> &stringTable);
	bool							WriteToStream(CFileStream &stream, TArray<CString> &stringTable);
	static PSkeleton				ReadFromStream(CFileStream &stream, TArray<CString> &stringTable, ECoordinateFrame from, ECoordinateFrame to);

	CGuid							FindBone(const CString &name) const;
	TMemoryView<const SBoneRecord>	Bones() const { return m_Bones; }
	TMemoryView<const CFloat4x4>	Matrices() const;
	TMemoryView<const CFloat4x4>	InverseMatrices() const;

	static hh_u32					CurrentVersion();

	CString							BuildHierarchicalString() const;
	CDigestMD5						RecomputeHierarchicalDigest() const;
	const CDigestMD5				&HierarchicalDigest() const { return m_HierarchicalDigest; }
	bool							Equals(PSkeleton other);

	void							RebuildBaseMatrices();
	void							RebuildBaseInverseMatrices();

	void							RecursiveConvertCoordinates(hh_u32 bone, ECoordinateFrame from, ECoordinateFrame to);

	CSkeletonView					View() { HH_ASSERT(m_Matrices.Count() == m_InverseMatrices.Count()); return CSkeletonView(m_Matrices.Count(), &m_Matrices[0], &m_InverseMatrices[0]); }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	//__GE_SKELETON_H__

