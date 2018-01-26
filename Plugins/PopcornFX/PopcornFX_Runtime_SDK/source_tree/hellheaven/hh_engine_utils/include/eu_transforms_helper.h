#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/10/20 15:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_TRANSFORMS_HELPER_H__
#define __KR_TRANSFORMS_HELPER_H__

#include "hh_kernel/include/kr_base_types.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT CBasicTransformsStorageHelper
{
private:
	CQuaternion			m_Orientation;
	CFloat4x4			m_Transforms;

protected:
	CFloat3				m_Position;
	CFloat3				m_EulerOrientation;

	bool				OnPositionChanged();			// Rebuild m_Transforms
	bool				OnEulerOrientationChanged();	// Rebuild m_Orientation and m_Transforms

	virtual void		RebuildTransforms(CFloat4x4 &outMatrix);

public:
	CBasicTransformsStorageHelper();
	virtual ~CBasicTransformsStorageHelper() {}

	// main transforms accessors:
	const CFloat4x4		&Transforms() const { return m_Transforms; }
	void				SetTransforms(const CFloat4x4 &newTransforms);

	// auxilliary transforms accessors (will internally call Transforms() / SetTransforms())
	const CQuaternion	&Orientation() const { return m_Orientation; }
	void				SetOrientation(const CQuaternion &newOrientation);
};

//----------------------------------------------------------------------------
// instance helper:

class	HH_ENGINEUTILS_EXPORT CBasicTransformsInstanceHelper
{
protected:
	CBasicTransformsInstanceHelper();
	virtual ~CBasicTransformsInstanceHelper() {}

	CFloat4x4						m_Transforms;

	virtual bool					_VirtualSetTransforms(const CFloat4x4 &/*newPos*/) { return false; };

public:
	// direct transformations accessors:

	HH_FORCEINLINE const CFloat4x4	&Transforms() const { return m_Transforms; }
	void							SetTransforms(const CFloat4x4 &newTransforms);

	// indirect accessors:
	HH_FORCEINLINE const CFloat3	&Position() const { return m_Transforms.StrippedTranslations(); }
	void							SetPosition(const CFloat3 &newPos);
	CQuaternion						Orientation() const;
	void							SetOrientation(const CQuaternion &newOrientation);

	void							GetEulerOrientation(CFloat3 &angles) const;
	void							SetEulerOrientation(const CFloat3 &newAngles);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_TRANSFORMS_HELPER_H__
