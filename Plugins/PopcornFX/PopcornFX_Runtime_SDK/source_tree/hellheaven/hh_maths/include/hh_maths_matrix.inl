#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/02/10 19:26 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_MATRIX_INL__
#define __PK_MATHS_MATRIX_INL__

#ifndef	__PK_MATHS_MATRIX_H__
#	error	hh_maths_matrix.inl must not be included manually, include hh_maths_matrix.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Ctors	2 x N
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 2, _AxisDimensionCount>::TMatrix()
{
	HH_ONLY_IN_DEBUG(
		for (hh_u32 i = 0; i < 2; i++)
		{
			TNumericTraits<SelfAxisType>::SetInvalidDebug(_M::m_Axes[i]);
		}
	);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 2, _AxisDimensionCount>::TMatrix(_Type value)
{
	for (hh_u32 i = 0; i < 2; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			_M::m_Axes[i].Axis(j) = value;
		}
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 2, _AxisDimensionCount>::TMatrix(const SelfAxisType &axis)
{
	for (hh_u32 i = 0; i < 2; i++)
	{
		_M::m_Axes[i] = axis;
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 2, _AxisDimensionCount>::TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis)
{
	_M::m_Axes[0] = xAxis;
	_M::m_Axes[1] = yAxis;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 2, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01,
																_Type m10, _Type m11)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 2);
	_M::m_Axes[0] = TVector<_Type, 2>(m00, m01);
	_M::m_Axes[1] = TVector<_Type, 2>(m10, m11);
}

//----------------------------------------------------------------------------
//
//	Ctors	3 x N
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix()
{
	HH_ONLY_IN_DEBUG(
		for (hh_u32 i = 0; i < 3; i++)
		{
			TNumericTraits<SelfAxisType>::SetInvalidDebug(_M::m_Axes[i]);
		}
	);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(_Type value)
{
	for (hh_u32 i = 0; i < 3; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			_M::m_Axes[i].Axis(j) = value;
		}
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(const SelfAxisType &axis)
{
	for (hh_u32 i = 0; i < 3; i++)
	{
		_M::m_Axes[i] = axis;
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis, const SelfAxisType &zAxis)
{
	_M::m_Axes[0] = xAxis;
	_M::m_Axes[1] = yAxis;
	_M::m_Axes[2] = zAxis;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01,
																_Type m10, _Type m11,
																_Type m20, _Type m21)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 2);
	_M::m_Axes[0] = TVector<_Type, 2>(m00, m01);
	_M::m_Axes[1] = TVector<_Type, 2>(m10, m11);
	_M::m_Axes[2] = TVector<_Type, 2>(m20, m21);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01, _Type m02,
																_Type m10, _Type m11, _Type m12,
																_Type m20, _Type m21, _Type m22)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 3);
	_M::m_Axes[0] = TVector<_Type, 3>(m00, m01, m02);
	_M::m_Axes[1] = TVector<_Type, 3>(m10, m11, m12);
	_M::m_Axes[2] = TVector<_Type, 3>(m20, m21, m22);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 3, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01, _Type m02, _Type m03,
																_Type m10, _Type m11, _Type m12, _Type m13,
																_Type m20, _Type m21, _Type m22, _Type m23)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 4);
	_M::m_Axes[0] = TVector<_Type, 4>(m00, m01, m02, m03);
	_M::m_Axes[1] = TVector<_Type, 4>(m10, m11, m12, m13);
	_M::m_Axes[2] = TVector<_Type, 4>(m20, m21, m22, m23);
}

//----------------------------------------------------------------------------
//
//	Ctors	4 x N
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix()
{
	HH_ONLY_IN_DEBUG(
		for (hh_u32 i = 0; i < 4; i++)
		{
			TNumericTraits<SelfAxisType>::SetInvalidDebug(_M::m_Axes[i]);
		}
	);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix(_Type value)
{
	for (hh_u32 i = 0; i < 4; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			_M::m_Axes[i].Axis(j) = value;
		}
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix(const SelfAxisType &axis)
{
	for (hh_u32 i = 0; i < 4; i++)
	{
		_M::m_Axes[i] = axis;
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis, const SelfAxisType &zAxis, const SelfAxisType &wAxis)
{
	_M::m_Axes[0] = xAxis;
	_M::m_Axes[1] = yAxis;
	_M::m_Axes[2] = zAxis;
	_M::m_Axes[3] = wAxis;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01, _Type m02,
																_Type m10, _Type m11, _Type m12,
																_Type m20, _Type m21, _Type m22,
																_Type m30, _Type m31, _Type m32)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 3);
	_M::m_Axes[0] = TVector<_Type, 3>(m00, m01, m02);
	_M::m_Axes[1] = TVector<_Type, 3>(m10, m11, m12);
	_M::m_Axes[2] = TVector<_Type, 3>(m20, m21, m22);
	_M::m_Axes[3] = TVector<_Type, 3>(m30, m31, m32);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE TMatrix<_Type, 4, _AxisDimensionCount>::TMatrix(	_Type m00, _Type m01, _Type m02, _Type m03,
																_Type m10, _Type m11, _Type m12, _Type m13,
																_Type m20, _Type m21, _Type m22, _Type m23,
																_Type m30, _Type m31, _Type m32, _Type m33)
{
	HH_STATIC_ASSERT(_AxisDimensionCount == 4);
	_M::m_Axes[0] = TVector<_Type, 4>(m00, m01, m02, m03);
	_M::m_Axes[1] = TVector<_Type, 4>(m10, m11, m12, m13);
	_M::m_Axes[2] = TVector<_Type, 4>(m20, m21, m22, m23);
	_M::m_Axes[3] = TVector<_Type, 4>(m30, m31, m32, m33);
}

//----------------------------------------------------------------------------
//
//	Matrix/Vector ops
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 3>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::TransformVector(const TVector<_Type, 3> &vec) const
{
	HH_STATIC_ASSERT_CUSTOM(_AxisDimensionCount >= 3 && _AxisCount >= 3, "Matrix_Too_Small_To_Transform_TVector3___You_Need_At_Least_a_TMatrix3x3");
	TVector<_Type, 3>	out = (_AxisDimensionCount > 3) ? m_Axes[3].xyz() : TVector<_Type, 3>(0);
	for (hh_u32 i = 0; i < 3; i++)
	{
		out += m_Axes[i].xyz() * vec.Axis(i);
	}
	return out;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 4>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::TransformVector(const TVector<_Type, 4> &vec) const
{
	HH_STATIC_ASSERT_CUSTOM(_AxisDimensionCount >= 4 && _AxisCount >= 4, "Matrix_Too_Small_To_Transform_TVector4___You_Need_At_Least_a_TMatrix4x4");
	TVector<_Type, 4>	out = m_Axes[0] * vec.Axis(0);
	for (hh_u32 i = 1; i < 4; i++)
	{
		out += m_Axes[i] * vec.Axis(i);
	}
	return out;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 3>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::RotateVector(const TVector<_Type, 3> &vec) const
{
	HH_STATIC_ASSERT_CUSTOM(_AxisDimensionCount >= 3 && _AxisCount >= 3, "Matrix_Too_Small_To_Rotate_TVector3___You_Need_At_Least_a_TMatrix3x3");
	TVector<_Type, 3>	out = TVector<_Type, 3>(0);
	for (hh_u32 i = 0; i < 3; i++)
	{
		out += m_Axes[i].xyz() * vec.Axis(i);
	}
	return out;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 4>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::RotateVector(const TVector<_Type, 4> &vec) const
{
	HH_STATIC_ASSERT_CUSTOM(_AxisDimensionCount >= 3 && _AxisCount >= 3, "Matrix_Too_Small_To_Rotate_TVector4___You_Need_At_Least_a_TMatrix3x3");
	TVector<_Type, 4>	out = m_Axes[0] * vec.Axis(0);
	for (hh_u32 i = 1; i < 3; i++)
	{
		out += m_Axes[i] * vec.Axis(i);
	}
	out.w() = vec.w();
	return out;
}

//----------------------------------------------------------------------------
//
//	Scaling
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Scale(_Type scale)
{
	for (hh_u32 i = 0; i < _AxisCount; i++)
		m_Axes[i] *= scale;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Scale(const TVector<_Type, _AxisDimensionCount> &scale)
{
	for (hh_u32 i = 0; i < _AxisCount; i++)
		m_Axes[i] *= scale;
}

//----------------------------------------------------------------------------
//
//	Extension
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
template<hh_u32 _NewAxisCount, hh_u32 _NewAxisDimensionCount>
TMatrixImpl<_Type, _NewAxisCount, _NewAxisDimensionCount>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Extend(_Type fillWith /*= TNumericTraits<_Type>::Zero()*/) const
{
	HH_STATIC_ASSERT_CUSTOM(_NewAxisCount >= _AxisCount && _NewAxisDimensionCount >= _AxisDimensionCount, "Cannot_Extend_A_Matrix_To_A_Size_Smaller_Than_Itself");
	TMatrixImpl<_Type, _NewAxisCount, _NewAxisDimensionCount>	extended;
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		TVector<_Type, _NewAxisDimensionCount>	&dst = extended.Axis(i);
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			dst.Axis(j) = m_Axes[i].Axis(j);
		}
		for (hh_u32 j = _AxisDimensionCount; j < _NewAxisDimensionCount; j++)
		{
			dst.Axis(j) = fillWith;
		}
	}

	for (hh_u32 i = _AxisCount; i < _NewAxisCount; i++)
	{
		TVector<_Type, _NewAxisDimensionCount>	&dst = extended.Axis(i);
		for (hh_u32 j = 0; j < _NewAxisDimensionCount; j++)
		{
			dst.Axis(j) = fillWith;
		}
	}
	return extended;
}

//----------------------------------------------------------------------------
//
//	Matrix inversions
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount> struct	TMatrixInternalTools
{
	typedef	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>	SelfType;

	_Type	SubDeterminant() const { HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("Determinant not supported for non square matrices"); }
};

//----------------------------------------------------------------------------
//
//	Matrix inversions (3x3 specialization)
//
//----------------------------------------------------------------------------

template<typename _Type>
struct	TMatrixInternalTools<_Type, 3, 3>
{
	typedef	TMatrixImpl<_Type, 3, 3>		SelfType;

	static _Type	SubDeterminant(const SelfType &matrix)
	{
		return (matrix.m_Axes[0].Axis(0) * (matrix.m_Axes[1].Axis(1) * matrix.m_Axes[2].Axis(2) - matrix.m_Axes[1].Axis(2) * matrix.m_Axes[2].Axis(1)) +
				matrix.m_Axes[0].Axis(1) * (matrix.m_Axes[1].Axis(2) * matrix.m_Axes[2].Axis(0) - matrix.m_Axes[1].Axis(0) * matrix.m_Axes[2].Axis(2)) +
				matrix.m_Axes[0].Axis(2) * (matrix.m_Axes[1].Axis(0) * matrix.m_Axes[2].Axis(1) - matrix.m_Axes[1].Axis(1) * matrix.m_Axes[2].Axis(0)));
	}

	static void	Invert(const SelfType &matrix, SelfType &out)
	{
		HH_ASSERT(&matrix != &out);
		const _Type	m_00 = matrix.m_Axes[1].Axis(1) * matrix.m_Axes[2].Axis(2) - matrix.m_Axes[1].Axis(2) * matrix.m_Axes[2].Axis(1);
		const _Type	m_01 = matrix.m_Axes[1].Axis(2) * matrix.m_Axes[2].Axis(0) - matrix.m_Axes[1].Axis(0) * matrix.m_Axes[2].Axis(2);
		const _Type	m_02 = matrix.m_Axes[1].Axis(0) * matrix.m_Axes[2].Axis(1) - matrix.m_Axes[1].Axis(1) * matrix.m_Axes[2].Axis(0);

		const _Type	m_10 = matrix.m_Axes[0].Axis(2) * matrix.m_Axes[2].Axis(1) - matrix.m_Axes[0].Axis(1) * matrix.m_Axes[2].Axis(2);
		const _Type	m_11 = matrix.m_Axes[0].Axis(0) * matrix.m_Axes[2].Axis(2) - matrix.m_Axes[0].Axis(2) * matrix.m_Axes[2].Axis(0);
		const _Type	m_12 = matrix.m_Axes[0].Axis(1) * matrix.m_Axes[2].Axis(0) - matrix.m_Axes[0].Axis(0) * matrix.m_Axes[2].Axis(1);

		const _Type	m_20 = matrix.m_Axes[0].Axis(1) * matrix.m_Axes[1].Axis(2) - matrix.m_Axes[0].Axis(2) * matrix.m_Axes[1].Axis(1);
		const _Type	m_21 = matrix.m_Axes[0].Axis(2) * matrix.m_Axes[1].Axis(0) - matrix.m_Axes[0].Axis(0) * matrix.m_Axes[1].Axis(2);
		const _Type	m_22 = matrix.m_Axes[0].Axis(0) * matrix.m_Axes[1].Axis(1) - matrix.m_Axes[0].Axis(1) * matrix.m_Axes[1].Axis(0);

		const _Type	det = _Type(1) / (matrix.m_Axes[0].Axis(0) * m_00 + matrix.m_Axes[0].Axis(1) * m_01 + matrix.m_Axes[0].Axis(2) * m_02);

		out = TMatrix<_Type, 3, 3>(	m_00 * det, m_10 * det, m_20 * det,
									m_01 * det, m_11 * det, m_21 * det,
									m_02 * det, m_12 * det, m_22 * det);
	}

	template<bool _InPlace>
	static void	InvertPureRotations(const SelfType &matrix, SelfType &out)
	{
		if (_InPlace)
		{
			HH_ASSERT(&matrix == &out);
			out.Transpose();
		}
		else
		{
			HH_ASSERT(&matrix != &out);
			out = matrix.Transposed();
		}
	}

	template<bool _InPlace>
	static HH_FORCEINLINE void	InvertPureTransforms(const SelfType &matrix, SelfType &out)
	{
		InvertPureRotations<_InPlace>(matrix, out);
	}
};

//----------------------------------------------------------------------------
//
//	Matrix inversions (4x4 specialization)
//
//----------------------------------------------------------------------------

template<typename _Type>
struct	TMatrixInternalTools<_Type, 4, 4>
{
	typedef	TMatrixImpl<_Type, 4, 4>		SelfType;

	static _Type	SubDeterminant(const SelfType &matrix)
	{
		const _Type	m_22_33_23_32 = matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(3) - matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(2);
		const _Type	m_23_30_20_33 = matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(0) - matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(3);
		const _Type	m_20_31_21_30 = matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(1) - matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(0);
		const _Type	m_21_32_22_31 = matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(2) - matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(1);
		const _Type	m_23_31_21_33 = matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(1) - matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(3);
		const _Type	m_20_32_22_30 = matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(2) - matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(0);

		const _Type	d00 = matrix.m_Axes[0].Axis(0) * (matrix.m_Axes[1].Axis(1) * m_22_33_23_32 + matrix.m_Axes[1].Axis(2) * m_23_31_21_33 + matrix.m_Axes[1].Axis(3) * m_21_32_22_31);
		const _Type	d01 = matrix.m_Axes[0].Axis(1) * (matrix.m_Axes[1].Axis(0) * m_22_33_23_32 + matrix.m_Axes[1].Axis(2) * m_23_30_20_33 + matrix.m_Axes[1].Axis(3) * m_20_32_22_30);
		const _Type	d02 = matrix.m_Axes[0].Axis(2) * (matrix.m_Axes[1].Axis(0) *-m_23_31_21_33 + matrix.m_Axes[1].Axis(1) * m_23_30_20_33 + matrix.m_Axes[1].Axis(3) * m_20_31_21_30);
		const _Type	d03 = matrix.m_Axes[0].Axis(3) * (matrix.m_Axes[1].Axis(0) * m_21_32_22_31 + matrix.m_Axes[1].Axis(1) *-m_20_32_22_30 + matrix.m_Axes[1].Axis(2) * m_20_31_21_30);

		return d00 - d01 + d02 - d03;
	}

	static void	Invert(const SelfType &matrix, SelfType &out)
	{
		HH_ASSERT(&matrix != &out);
		const _Type	m_22_33_23_32 = matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(3) - matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(2);
		const _Type	m_23_30_20_33 = matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(0) - matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(3);
		const _Type	m_20_31_21_30 = matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(1) - matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(0);
		const _Type	m_21_32_22_31 = matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(2) - matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(1);
		const _Type	m_23_31_21_33 = matrix.m_Axes[2].Axis(3) * matrix.m_Axes[3].Axis(1) - matrix.m_Axes[2].Axis(1) * matrix.m_Axes[3].Axis(3);
		const _Type	m_20_32_22_30 = matrix.m_Axes[2].Axis(0) * matrix.m_Axes[3].Axis(2) - matrix.m_Axes[2].Axis(2) * matrix.m_Axes[3].Axis(0);

		const _Type	d00 = matrix.m_Axes[1].Axis(1) * m_22_33_23_32 + matrix.m_Axes[1].Axis(2) * m_23_31_21_33 + matrix.m_Axes[1].Axis(3) * m_21_32_22_31;
		const _Type	d01 = matrix.m_Axes[1].Axis(0) * m_22_33_23_32 + matrix.m_Axes[1].Axis(2) * m_23_30_20_33 + matrix.m_Axes[1].Axis(3) * m_20_32_22_30;
		const _Type	d02 = matrix.m_Axes[1].Axis(0) *-m_23_31_21_33 + matrix.m_Axes[1].Axis(1) * m_23_30_20_33 + matrix.m_Axes[1].Axis(3) * m_20_31_21_30;
		const _Type	d03 = matrix.m_Axes[1].Axis(0) * m_21_32_22_31 + matrix.m_Axes[1].Axis(1) *-m_20_32_22_30 + matrix.m_Axes[1].Axis(2) * m_20_31_21_30;

		const _Type	d10 = matrix.m_Axes[0].Axis(1) * m_22_33_23_32 + matrix.m_Axes[0].Axis(2) * m_23_31_21_33 + matrix.m_Axes[0].Axis(3) * m_21_32_22_31;
		const _Type	d11 = matrix.m_Axes[0].Axis(0) * m_22_33_23_32 + matrix.m_Axes[0].Axis(2) * m_23_30_20_33 + matrix.m_Axes[0].Axis(3) * m_20_32_22_30;
		const _Type	d12 = matrix.m_Axes[0].Axis(0) *-m_23_31_21_33 + matrix.m_Axes[0].Axis(1) * m_23_30_20_33 + matrix.m_Axes[0].Axis(3) * m_20_31_21_30;
		const _Type	d13 = matrix.m_Axes[0].Axis(0) * m_21_32_22_31 + matrix.m_Axes[0].Axis(1) *-m_20_32_22_30 + matrix.m_Axes[0].Axis(2) * m_20_31_21_30;

		const _Type	m_02_13_03_12 = matrix.m_Axes[0].Axis(2) * matrix.m_Axes[1].Axis(3) - matrix.m_Axes[0].Axis(3) * matrix.m_Axes[1].Axis(2);
		const _Type	m_03_10_00_13 = matrix.m_Axes[0].Axis(3) * matrix.m_Axes[1].Axis(0) - matrix.m_Axes[0].Axis(0) * matrix.m_Axes[1].Axis(3);
		const _Type	m_00_11_01_10 = matrix.m_Axes[0].Axis(0) * matrix.m_Axes[1].Axis(1) - matrix.m_Axes[0].Axis(1) * matrix.m_Axes[1].Axis(0);
		const _Type	m_01_12_02_11 = matrix.m_Axes[0].Axis(1) * matrix.m_Axes[1].Axis(2) - matrix.m_Axes[0].Axis(2) * matrix.m_Axes[1].Axis(1);
		const _Type	m_03_11_01_13 = matrix.m_Axes[0].Axis(3) * matrix.m_Axes[1].Axis(1) - matrix.m_Axes[0].Axis(1) * matrix.m_Axes[1].Axis(3);
		const _Type	m_00_12_02_10 = matrix.m_Axes[0].Axis(0) * matrix.m_Axes[1].Axis(2) - matrix.m_Axes[0].Axis(2) * matrix.m_Axes[1].Axis(0);

		const _Type	d20 = matrix.m_Axes[3].Axis(1) * m_02_13_03_12 + matrix.m_Axes[3].Axis(2) * m_03_11_01_13 + matrix.m_Axes[3].Axis(3) * m_01_12_02_11;
		const _Type	d21 = matrix.m_Axes[3].Axis(0) * m_02_13_03_12 + matrix.m_Axes[3].Axis(2) * m_03_10_00_13 + matrix.m_Axes[3].Axis(3) * m_00_12_02_10;
		const _Type	d22 = matrix.m_Axes[3].Axis(0) *-m_03_11_01_13 + matrix.m_Axes[3].Axis(1) * m_03_10_00_13 + matrix.m_Axes[3].Axis(3) * m_00_11_01_10;
		const _Type	d23 = matrix.m_Axes[3].Axis(0) * m_01_12_02_11 + matrix.m_Axes[3].Axis(1) *-m_00_12_02_10 + matrix.m_Axes[3].Axis(2) * m_00_11_01_10;

		const _Type	d30 = matrix.m_Axes[2].Axis(1) * m_02_13_03_12 + matrix.m_Axes[2].Axis(2) * m_03_11_01_13 + matrix.m_Axes[2].Axis(3) * m_01_12_02_11;
		const _Type	d31 = matrix.m_Axes[2].Axis(0) * m_02_13_03_12 + matrix.m_Axes[2].Axis(2) * m_03_10_00_13 + matrix.m_Axes[2].Axis(3) * m_00_12_02_10;
		const _Type	d32 = matrix.m_Axes[2].Axis(0) *-m_03_11_01_13 + matrix.m_Axes[2].Axis(1) * m_03_10_00_13 + matrix.m_Axes[2].Axis(3) * m_00_11_01_10;
		const _Type	d33 = matrix.m_Axes[2].Axis(0) * m_01_12_02_11 + matrix.m_Axes[2].Axis(1) *-m_00_12_02_10 + matrix.m_Axes[2].Axis(2) * m_00_11_01_10;

		const _Type	det = _Type(1) / (matrix.m_Axes[0].Axis(0) * d00 - matrix.m_Axes[0].Axis(1) * d01 + matrix.m_Axes[0].Axis(2) * d02 - matrix.m_Axes[0].Axis(3) * d03);

		out = TMatrix<_Type, 4, 4>(	 d00 * det, -d10 * det,  d20 * det, -d30 * det,
									-d01 * det,  d11 * det, -d21 * det,  d31 * det,
									 d02 * det, -d12 * det,  d22 * det, -d32 * det,
									-d03 * det,  d13 * det, -d23 * det,  d33 * det);
	}

	template<bool _InPlace>
	static void	InvertPureRotations(const SelfType &matrix, SelfType &out)
	{
		HH_ASSERT((!_InPlace && &matrix != &out) || (_InPlace && &matrix == &out));

		out.m_Axes[0].Axis(0) = matrix.m_Axes[0].Axis(0);
		out.m_Axes[0].Axis(1) = matrix.m_Axes[1].Axis(0);
		out.m_Axes[0].Axis(2) = matrix.m_Axes[2].Axis(0);
		if (!_InPlace)
			out.Axis(0).Axis(3) = matrix.Axis(0).Axis(3);

		out.m_Axes[1].Axis(0) = matrix.m_Axes[0].Axis(1);
		out.m_Axes[1].Axis(1) = matrix.m_Axes[1].Axis(1);
		out.m_Axes[1].Axis(2) = matrix.m_Axes[2].Axis(1);
		if (!_InPlace)
			out.Axis(1).Axis(3) = matrix.Axis(1).Axis(3);

		out.m_Axes[2].Axis(0) = matrix.m_Axes[0].Axis(2);
		out.m_Axes[2].Axis(1) = matrix.m_Axes[1].Axis(2);
		out.m_Axes[2].Axis(2) = matrix.m_Axes[2].Axis(2);

		if (!_InPlace)
		{
			out.m_Axes[2].Axis(3) = matrix.m_Axes[2].Axis(3);
			out.m_Axes[3] = matrix.m_Axes[3];
		}
	}

	template<bool _InPlace>
	static void	InvertPureTransforms(const SelfType &matrix, SelfType &out)
	{
		if (_InPlace)
		{
			HH_ASSERT(&matrix == &out);
			const _Type	v01 = matrix.m_Axes[0].Axis(1);
			const _Type	v02 = matrix.m_Axes[0].Axis(2);
			const _Type	v12 = matrix.m_Axes[1].Axis(2);

			out.m_Axes[0].Axis(1) = matrix.m_Axes[1].Axis(0);
			out.m_Axes[0].Axis(2) = matrix.m_Axes[2].Axis(0);

			out.m_Axes[1].Axis(0) = v01;
			out.m_Axes[1].Axis(2) = matrix.m_Axes[2].Axis(1);

			out.m_Axes[2].Axis(0) = v02;
			out.m_Axes[2].Axis(1) = v12;

			const TVector<_Type, 3>	tr = matrix.m_Axes[3].xyz();
			out.m_Axes[3].x() = -(tr.x() * matrix.m_Axes[0].x() + tr.y() * matrix.m_Axes[1].x() + tr.z() * matrix.m_Axes[2].x());
			out.m_Axes[3].y() = -(tr.x() * matrix.m_Axes[0].y() + tr.y() * matrix.m_Axes[1].y() + tr.z() * matrix.m_Axes[2].y());
			out.m_Axes[3].z() = -(tr.x() * matrix.m_Axes[0].z() + tr.y() * matrix.m_Axes[1].z() + tr.z() * matrix.m_Axes[2].z());
		}
		else
		{
			out.m_Axes[0].Axis(0) = matrix.m_Axes[0].Axis(0);
			out.m_Axes[0].Axis(1) = matrix.m_Axes[1].Axis(0);
			out.m_Axes[0].Axis(2) = matrix.m_Axes[2].Axis(0);
			out.m_Axes[0].Axis(3) = matrix.m_Axes[0].Axis(3);

			out.m_Axes[1].Axis(0) = matrix.m_Axes[0].Axis(1);
			out.m_Axes[1].Axis(1) = matrix.m_Axes[1].Axis(1);
			out.m_Axes[1].Axis(2) = matrix.m_Axes[2].Axis(1);
			out.m_Axes[1].Axis(3) = matrix.m_Axes[1].Axis(3);

			out.m_Axes[2].Axis(0) = matrix.m_Axes[0].Axis(2);
			out.m_Axes[2].Axis(1) = matrix.m_Axes[1].Axis(2);
			out.m_Axes[2].Axis(2) = matrix.m_Axes[2].Axis(2);
			out.m_Axes[2].Axis(3) = matrix.m_Axes[2].Axis(3);

			const TVector<_Type, 4>	&tr = matrix.m_Axes[3];
			out.m_Axes[3].x() = -(tr.x() * matrix.m_Axes[0].x() + tr.y() * matrix.m_Axes[0].y() + tr.z() * matrix.m_Axes[0].z());
			out.m_Axes[3].y() = -(tr.x() * matrix.m_Axes[1].x() + tr.y() * matrix.m_Axes[1].y() + tr.z() * matrix.m_Axes[1].z());
			out.m_Axes[3].z() = -(tr.x() * matrix.m_Axes[2].x() + tr.y() * matrix.m_Axes[2].y() + tr.z() * matrix.m_Axes[2].z());
			out.m_Axes[3].w() = tr.w();
		}
	}
};

//----------------------------------------------------------------------------

#ifdef	__HHSELFTYPE
#	error	__HHSELFTYPE already defined
#endif

#define	__HHSELFTYPE	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
__HHSELFTYPE	__HHSELFTYPE::operator - () const
{
	__HHSELFTYPE	result;
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			result.m_Axes[i].Axis(j) = -m_Axes[i].Axis(j);
		}
	}
	return result;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE __HHSELFTYPE	__HHSELFTYPE::Inverse() const
{
	__HHSELFTYPE tmp;
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::Invert(*this, tmp);
	return tmp;
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::Invert()
{
	__HHSELFTYPE tmp;
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::Invert(*this, tmp);
	*this = tmp;
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::Invert(__HHSELFTYPE &output) const
{
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::Invert(*this, output);
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE __HHSELFTYPE	__HHSELFTYPE::InversePureRotations() const
{
	__HHSELFTYPE tmp;
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureRotations<false>(*this, tmp);
	return tmp;
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::InvertPureRotations()
{
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureRotations<true>(*this, *this);
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::InvertPureRotations(__HHSELFTYPE &output) const
{
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureRotations<false>(*this, output);
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE __HHSELFTYPE	__HHSELFTYPE::InversePureTransforms() const
{
	__HHSELFTYPE tmp;
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureTransforms<false>(*this, tmp);
	return tmp;
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::InvertPureTransforms()
{
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureTransforms<true>(*this, *this);
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE void			__HHSELFTYPE::InvertPureTransforms(__HHSELFTYPE &output) const
{
	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::template InvertPureTransforms<false>(*this, output);
}


template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_FORCEINLINE _Type	__HHSELFTYPE::Determinant() const
{
	HH_STATIC_ASSERT(_AxisCount == _AxisDimensionCount);
	return TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>::SubDeterminant(*this);
}

#undef	__HHSELFTYPE

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
TMatrixImpl<_Type, _AxisDimensionCount, _AxisCount>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Transposed() const
{
	HH_STATIC_ASSERT_CUSTOM(_AxisCount == _AxisDimensionCount, "Generic_Transpose_Only_Works_On_Square_Matrices___Use_The_Transposed_Method_Instead");
	TMatrixImpl<_Type, _AxisDimensionCount, _AxisCount>	output;
	for (hh_u32 j = 0; j < _AxisCount; j++)
	{
		for (hh_u32 i = 0; i < _AxisDimensionCount; i++)
		{
			output.Axis(j).Axis(i) = m_Axes[i].Axis(j);
		}
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Transpose()
{
	HH_STATIC_ASSERT_CUSTOM(_AxisCount == _AxisDimensionCount, "Generic_Transpose_Only_Works_On_Square_Matrices___Use_The_Transposed_Method_Instead");
	for (hh_u32 j = 0; j < _AxisCount - 1; j++)
	{
		for (hh_u32 i = j + 1; i < _AxisDimensionCount; i++)
		{
			HHSwap(m_Axes[j].Axis(i), m_Axes[i].Axis(j));
		}
	}
}

//----------------------------------------------------------------------------
//
//	Crop
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
template<hh_u32 _SubRectAxisStart, hh_u32 _NewAxisCount, hh_u32 _SubRectAxisDimensionStart, hh_u32 _NewAxisDimensionCount>
struct	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::TCropHelper
{
	typedef TMatrixImpl<ScalarType, _NewAxisCount, _NewAxisDimensionCount>	_ReturnType;

	static _ReturnType	Crop(const SelfType &matrix)
	{
		HH_STATIC_ASSERT_CUSTOM(_SubRectAxisStart + _NewAxisCount <= AxisCount && _SubRectAxisDimensionStart + _NewAxisDimensionCount <= AxisDimensionCount, "Cannot_Crop_A_Matrix_To_A_Size_Larger_Than_Itself");
		_ReturnType	cropped;
		for (hh_u32 i = _SubRectAxisStart; i < _SubRectAxisStart + _NewAxisCount; i++)
		{
			for (hh_u32 j = 0; j < _NewAxisDimensionCount; j++)
				cropped.Axis(i).Axis(j) = matrix.m_Axes[i].Axis(j);
		}
		return cropped;
	}
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
template<hh_u32 _SubRectAxisStart, hh_u32 _NewAxisCount>
struct	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::TCropHelper<_SubRectAxisStart, _NewAxisCount, 0, _AxisDimensionCount>
{
	typedef TMatrixImpl<ScalarType, _NewAxisCount, _AxisDimensionCount>	_CroppedType;
	typedef const _CroppedType										&_ReturnType;

	static _ReturnType	Crop(const SelfType &matrix)
	{
		HH_STATIC_ASSERT_CUSTOM(_SubRectAxisStart + _NewAxisCount <= AxisCount, "Cannot_Crop_A_Matrix_To_A_Size_Larger_Than_Itself");
		return *reinterpret_cast<const _CroppedType*>(&matrix.m_Axes[_SubRectAxisStart]);
	}
};

//----------------------------------------------------------------------------
//
//	Matrix properties
//
//----------------------------------------------------------------------------
#if 0
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Symmetrical(float tolerance /*= 1.0e-5f*/) const
{
	if (_AxisCount != _AxisDimensionCount)
		return false;	// only meaningful for square matrices
	for (hh_u32 i = 0; i < _AxisCount - 1; i++)
	{
		for (hh_u32 j = i + 1; j < _AxisCount; j++)
		{
			if (HHAbs(m_Axes[i].Axis(j) - m_Axes[j].Axis(i)) > tolerance)
				return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Isotropic(float tolerance) const
{
	static const hh_u32	_MinSquare = TIntegerTemplateTools::Min<_AxisCount, _AxisDimensionCount>::Value;
	HH_STATIC_ASSERT(_MinSquare >= 1);
	for (hh_u32 i = 0; i < _MinSquare - 1; i++)
	{
		for (hh_u32 j = i + 1; j < _MinSquare; j++)
		{
			if (HHAbs(m_Axes[i].Axis(j)) > tolerance ||
				HHAbs(m_Axes[j].Axis(i)) > tolerance)
				return false;
		}
	}
	const _Type	diag = m_Axes[0].x();
	for (hh_u32 i = 1; i < _MinSquare; i++)
	{
		if (HHAbs(m_Axes[i].Axis(i) - diag) > tolerance)
			return false;
	}
	return true;
}
#endif
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Homogeneous(float tolerance /*= 1.0e-5f*/) const
{
	if (_AxisDimensionCount == 4)
	{
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		_Type	delta = m_Axes[i].Axis(3);
		if (i == 3)
			delta -= 1.0f;
		if (delta > tolerance || delta < -tolerance)
			return false;
	}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Orthogonal(float tolerance /*= 1.0e-5f*/) const
{
//	const hh_u32	kAxisCount = (_AxisCount > 3) ? 3 : _AxisCount;
	for (hh_u32 i = 0; i < 3; i++)
	{
		for (hh_u32 j = i + 1; j < 3; j++)
		{
			if (HHAbs(m_Axes[i].Dot(m_Axes[j])) > tolerance)
				return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Orthonormal(float tolerance /*= 1.0e-5f*/) const
{
//	const hh_u32	kAxisCount = (_AxisCount > 3) ? 3 : _AxisCount;
	for (hh_u32 i = 0; i < 3; i++)
	{
		if (HHAbs(m_Axes[i].LengthSquared() - 1.0f) > tolerance)
			return false;
	}
	return Orthogonal(tolerance);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Scaled(_Type tolerance /*= 1.0e-5f*/) const
{
#if 1
	const CFloat3	asfsq = AbsScalingFactorsSquared();
	const float		tsq = tolerance * tolerance;
	for (hh_u32 i = 0; i < 3; i++)
	{
		if (asfsq.Axis(i) > tsq)
			return false;
	}
	return true;
#else
	return All(AbsScalingFactorsSquared() <= tolerance * tolerance);
#endif
}

//----------------------------------------------------------------------------
#if 0
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::DiagonallyDominant() const
{
	for (hh_u32 i = 0; i < TIntegerTemplateTools::Min<_AxisCount, _AxisDimensionCount>::Value; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			if (i == j)
				continue;
			if (m_Axes[i].Axis(i) <= m_Axes[i].Axis(j))
				return false;
		}
	}
	return true;
}
#endif
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Equals(const SelfType &other, _Type tolerance /*= 1.0e-5f*/) const
{
	_Type	delta = 0;
	_Type	mean = 0;
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			delta += HHSquared(this->Axis(i).Axis(j) - other.Axis(i).Axis(j));
			mean += HHSquared(this->Axis(i).Axis(j) + other.Axis(i).Axis(j));
		}
	}
	if (mean != 0)
		return (delta * 2 / mean) <= tolerance * tolerance;
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::IsIdentity(_Type tolerance /*= 1.0e-5f*/) const
{
	return Equals(TMatrix<_Type, _AxisCount, _AxisDimensionCount>::IDENTITY, tolerance);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
bool	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::IsLeftHanded() const
{
	return Determinant() < 0;
	//return StrippedXAxis().Cross(StrippedYAxis()).Dot(StrippedZAxis()) < 0;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 3>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::AbsScalingFactors() const	// WARNING: does not handle mirroring, the magnitudes will always be > 0... fix this? see what it would cost, and possibly make another method for when we care about negative scales
{
	return TVector<_Type, 3>(StrippedXAxis().Length(), StrippedYAxis().Length(), StrippedZAxis().Length());
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 3>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::AbsScalingFactorsSquared() const
{
	return TVector<_Type, 3>(StrippedXAxis().LengthSquared(), StrippedYAxis().LengthSquared(), StrippedZAxis().LengthSquared());
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
HH_INLINE TVector<_Type, 3>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::ScalingFactors() const
{
	TVector<_Type, 3>	scale = AbsScalingFactors();
#if 1
	if (Determinant() < 0)	// FIXME: we probably could get away with 'SubDeterminant<3,3>() < 0'
		scale.x() = -scale.x();
#else
	if (StrippedXAxis().Cross(StrippedYAxis()).Dot(StrippedZAxis()) < 0)
		scale.x() = -scale.x();
#endif
	return scale;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
_Type	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::UniformScalingFactor() const
{
	const TVector<_Type, 3>	out = AbsScalingFactors();
	return (out.x() + out.y() + out.z()) * 0.333333333f;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::RemoveScale()
{
	HH_ASSERT(Homogeneous());
	for (hh_u32 i = 0; i < 3; i++)
	{
		m_Axes[i].Normalize();	// won't work if the matrix isn't homogeneous
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::RemoveAbsScale()
{
	HH_ASSERT(Homogeneous());
	const TVector<_Type, 3>	scale = ScalingFactors();
	for (hh_u32 i = 0; i < 3; i++)
	{
		const float	n = 1.0f / scale.Axis(i);
		m_Axes[i] *= n;
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Explode(TVector<_Type, 3> &translations, TMatrixImpl<_Type, 3, 3> &rotations, TVector<_Type, 3> &scale) const
{
	HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 4);
	translations = StrippedTranslations();	// that was hard...

	scale = ScalingFactors();
	HH_ASSERT(HHAbs(scale) > 1.0e-5f);

	rotations.XAxis() = StrippedXAxis() * HHRcp(scale.x());
	rotations.YAxis() = StrippedYAxis() * HHRcp(scale.x());
	rotations.ZAxis() = StrippedZAxis() * HHRcp(scale.x());
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::Explode(TVector<_Type, 3> &translations, TMatrixImpl<_Type, 3, 3> &rotations, TVector<_Type, 3> &scale, TVector<_Type, 3> &shear) const
{
	HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 4);
	HH_ASSERT_NOT_REACHED(); // FIXME:  implement
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>	TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>::BuildDiagonalMatrix(_Type diagonalValue)
{
	SelfType	diagonalMatrix;
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		for (hh_u32 j = 0; j < _AxisDimensionCount; j++)
		{
			diagonalMatrix.Axis(i).Axis(j) = 0.0f;
		}
	}
	if (_AxisCount <= _AxisDimensionCount)
	{
		for (hh_u32 i = 0; i < _AxisCount; i++)
			diagonalMatrix.Axis(i).Axis(i) = diagonalValue;
	}
	else
	{
		for (hh_u32 i = 0; i < _AxisDimensionCount; i++)
			diagonalMatrix.Axis(i).Axis(i) = diagonalValue;
	}
	return diagonalMatrix;
}

//----------------------------------------------------------------------------

#if	(HH_MATHS_MATRIX_EXPLICIT_INSTANTIATION == 0)

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 2, _AxisDimensionCount>	TMatrix<_Type, 2, _AxisDimensionCount>::IDENTITY = TMatrix<_Type, 2, _AxisDimensionCount>::BuildDiagonalMatrix(1);
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 2, _AxisDimensionCount>	TMatrix<_Type, 2, _AxisDimensionCount>::ZERO = TMatrix<_Type, 2, _AxisDimensionCount>(TNumericTraits<_Type>::Zero());
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 2, _AxisDimensionCount>	TMatrix<_Type, 2, _AxisDimensionCount>::ONE = TMatrix<_Type, 2, _AxisDimensionCount>(TNumericTraits<_Type>::One());

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 3, _AxisDimensionCount>	TMatrix<_Type, 3, _AxisDimensionCount>::IDENTITY = TMatrix<_Type, 3, _AxisDimensionCount>::BuildDiagonalMatrix(1);
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 3, _AxisDimensionCount>	TMatrix<_Type, 3, _AxisDimensionCount>::ZERO = TMatrix<_Type, 3, _AxisDimensionCount>(TNumericTraits<_Type>::Zero());
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 3, _AxisDimensionCount>	TMatrix<_Type, 3, _AxisDimensionCount>::ONE = TMatrix<_Type, 3, _AxisDimensionCount>(TNumericTraits<_Type>::One());

template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 4, _AxisDimensionCount>	TMatrix<_Type, 4, _AxisDimensionCount>::IDENTITY = TMatrix<_Type, 4, _AxisDimensionCount>::BuildDiagonalMatrix(1);
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 4, _AxisDimensionCount>	TMatrix<_Type, 4, _AxisDimensionCount>::ZERO = TMatrix<_Type, 4, _AxisDimensionCount>(TNumericTraits<_Type>::Zero());
template<typename _Type, hh_u32 _AxisDimensionCount>
HH_ALIGN(0x10) const TMatrix<_Type, 4, _AxisDimensionCount>	TMatrix<_Type, 4, _AxisDimensionCount>::ONE = TMatrix<_Type, 4, _AxisDimensionCount>(TNumericTraits<_Type>::One());

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_MATRIX_INL__
