#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/23 10:32 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_MATRIX_H__
#define __PK_MATHS_MATRIX_H__

#ifndef __PK_MATHS_H__
#	error	hh_maths_matrix.h cannot be included without hh_maths.h
#endif

#include "hh_maths/include/hh_maths_vector.h"

#ifndef	HH_MATHS_MATRIX_EXPLICIT_INSTANTIATION
#	if	defined(HH_COMPILER_GCC) && !defined(HH_ORBIS)/* && defined(_DEBUG)*//* || defined(HH_COMPILER_GHS)*/
#		define	HH_MATHS_MATRIX_EXPLICIT_INSTANTIATION	1
#	else
#		define	HH_MATHS_MATRIX_EXPLICIT_INSTANTIATION	0
#	endif
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// forward-declares

template<typename _Type, hh_u32 _Dimension>								class	TVector;
template<typename _Type>												class	TQuaternion;
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount> class	TMatrix;
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount> class	TMatrixImpl;
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>	struct	TMatrixInternalTools;

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
class	TMatrix
{
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
class	TMatrix<_Type, 2, _AxisDimensionCount> : public TMatrixImpl<_Type, 2, _AxisDimensionCount>
{
protected:
	typedef TMatrixImpl<_Type, 2, _AxisDimensionCount>	_M;
	typedef TMatrix<_Type, 2, _AxisDimensionCount>		SelfType;
	typedef typename _M::SelfAxisType					SelfAxisType;

public:
	TMatrix();
	TMatrix(const _M &other) : _M(other) {}

	explicit TMatrix(_Type value);

	explicit TMatrix(const SelfAxisType &axis);

	TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis);

	TMatrix(_Type m00, _Type m01,
			_Type m10, _Type m11);

	//	Identity: | 1  0 |
	//	          | 0  1 |

	HH_ALIGN(0x10) static const SelfType	IDENTITY;

	//	Zero:     | 0  0 |
	//	          | 0  0 |

	HH_ALIGN(0x10) static const SelfType	ZERO;

	//	One:      | 1  1 |
	//	          | 1  1 |

	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
class	TMatrix<_Type, 3, _AxisDimensionCount> : public TMatrixImpl<_Type, 3, _AxisDimensionCount>
{
protected:
	typedef TMatrixImpl<_Type, 3, _AxisDimensionCount>	_M;
	typedef TMatrix<_Type, 3, _AxisDimensionCount>		SelfType;
	typedef typename _M::SelfAxisType					SelfAxisType;

public:
	TMatrix();
	TMatrix(const _M &other) : _M(other) {}

	explicit TMatrix(_Type value);

	explicit TMatrix(const SelfAxisType &axis);

	TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis, const SelfAxisType &zAxis);

	TMatrix(_Type m00, _Type m01,
			_Type m10, _Type m11,
			_Type m20, _Type m21);

	TMatrix(_Type m00, _Type m01, _Type m02,
			_Type m10, _Type m11, _Type m12,
			_Type m20, _Type m21, _Type m22);

	TMatrix(_Type m00, _Type m01, _Type m02, _Type m03,
			_Type m10, _Type m11, _Type m12, _Type m13,
			_Type m20, _Type m21, _Type m22, _Type m23);

	//	Identity: | 1  0  0 |
	//	          | 0  1  0 |
	//	          | 0  0  1 |

	HH_ALIGN(0x10) static const SelfType	IDENTITY;

	//	Zero:     | 0  0  0 |
	//	          | 0  0  0 |
	//	          | 0  0  0 |

	HH_ALIGN(0x10) static const SelfType	ZERO;

	//	One:      | 1  1  1 |
	//	          | 1  1  1 |
	//	          | 1  1  1 |

	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisDimensionCount>
class	TMatrix<_Type, 4, _AxisDimensionCount> : public TMatrixImpl<_Type, 4, _AxisDimensionCount>
{
protected:
	typedef TMatrixImpl<_Type, 4, _AxisDimensionCount>	_M;
	typedef TMatrix<_Type, 4, _AxisDimensionCount>		SelfType;
	typedef typename _M::SelfAxisType					SelfAxisType;

public:
	TMatrix();
	TMatrix(const _M &other) : _M(other) {}

	explicit TMatrix(_Type value);

	explicit TMatrix(const SelfAxisType &axis);

	TMatrix(const SelfAxisType &xAxis, const SelfAxisType &yAxis, const SelfAxisType &zAxis, const SelfAxisType &wAxis);

	TMatrix(_Type m00, _Type m01, _Type m02,
			_Type m10, _Type m11, _Type m12,
			_Type m20, _Type m21, _Type m22,
			_Type m30, _Type m31, _Type m32);

	TMatrix(_Type m00, _Type m01, _Type m02, _Type m03,
			_Type m10, _Type m11, _Type m12, _Type m13,
			_Type m20, _Type m21, _Type m22, _Type m23,
			_Type m30, _Type m31, _Type m32, _Type m33);

	//	Identity: | 1  0  0  0 |
	//	          | 0  1  0  0 |
	//	          | 0  0  1  0 |
	//	          | 0  0  0  1 |

	HH_ALIGN(0x10) static const SelfType	IDENTITY;

	//	Zero:     | 0  0  0  0 |
	//	          | 0  0  0  0 |
	//	          | 0  0  0  0 |
	//	          | 0  0  0  0 |

	HH_ALIGN(0x10) static const SelfType	ZERO;

	//	One:      | 1  1  1  1 |
	//	          | 1  1  1  1 |
	//	          | 1  1  1  1 |
	//	          | 1  1  1  1 |

	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
class	TMatrixImpl
{
public:
	typedef TMatrixImpl<_Type, _AxisCount, _AxisDimensionCount>	SelfType;
	typedef TMatrix<_Type, _AxisCount, _AxisDimensionCount>		SelfFullType;
	typedef TVector<_Type, _AxisDimensionCount>					SelfAxisType;
	typedef _Type												ScalarType;
	static const hh_u32											AxisCount = _AxisCount;
	static const hh_u32											AxisDimensionCount = _AxisDimensionCount;
	static const hh_u32											DiagonalSize = (_AxisCount <= _AxisDimensionCount) ? _AxisCount : _AxisDimensionCount;

private:
	friend struct	TMatrixInternalTools<_Type, _AxisCount, _AxisDimensionCount>;

protected:
	SelfAxisType	m_Axes[_AxisCount];

	HH_INLINE bool	_InBounds(hh_u32 axis, hh_u32 field) const { return axis < _AxisCount && field < _AxisDimensionCount; }

public:
	//TMatrixImpl() {}

	//----------------------------------------------------------------------------
	// Accessors

	HH_FORCEINLINE SelfAxisType				&Axis(hh_u32 which) { HH_ASSERT(which < _AxisCount); return m_Axes[which]; }
	HH_FORCEINLINE const SelfAxisType		&Axis(hh_u32 which) const { HH_ASSERT(which < _AxisCount); return m_Axes[which]; }

	HH_FORCEINLINE SelfAxisType				&XAxis() { return m_Axes[0]; }
	HH_FORCEINLINE SelfAxisType				&YAxis() { return m_Axes[1]; }
	HH_FORCEINLINE SelfAxisType				&ZAxis() { HH_STATIC_ASSERT(_AxisCount > 2); return m_Axes[2]; }
	HH_FORCEINLINE SelfAxisType				&WAxis() { HH_STATIC_ASSERT(_AxisCount > 3); return m_Axes[3]; }
	HH_FORCEINLINE SelfAxisType				&Translations() { HH_STATIC_ASSERT(_AxisCount > 3); return m_Axes[3]; }

	HH_FORCEINLINE const SelfAxisType		&XAxis() const { return m_Axes[0]; }
	HH_FORCEINLINE const SelfAxisType		&YAxis() const { return m_Axes[1]; }
	HH_FORCEINLINE const SelfAxisType		&ZAxis() const { HH_STATIC_ASSERT(_AxisCount > 2); return m_Axes[2]; }
	HH_FORCEINLINE const SelfAxisType		&WAxis() const { HH_STATIC_ASSERT(_AxisCount > 3); return m_Axes[3]; }
	HH_FORCEINLINE const SelfAxisType		&Translations() const { HH_STATIC_ASSERT(_AxisCount > 3); return m_Axes[3]; }

	//----------------------------------------------------------------------------

	HH_FORCEINLINE TVector<_Type, 3>		&StrippedXAxis() { HH_STATIC_ASSERT(_AxisDimensionCount >= 3); return m_Axes[0].xyz(); }
	HH_FORCEINLINE TVector<_Type, 3>		&StrippedYAxis() { HH_STATIC_ASSERT(_AxisDimensionCount >= 3); return m_Axes[1].xyz(); }
	HH_FORCEINLINE TVector<_Type, 3>		&StrippedZAxis() { HH_STATIC_ASSERT(_AxisCount >= 3 && _AxisDimensionCount >= 3); return m_Axes[2].xyz(); }
	HH_FORCEINLINE TVector<_Type, 3>		&StrippedWAxis() { HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 3); return m_Axes[3].xyz(); }
	HH_FORCEINLINE TVector<_Type, 3>		&StrippedTranslations() { HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 3); return m_Axes[3].xyz(); }

	HH_FORCEINLINE const TVector<_Type, 3>	&StrippedXAxis() const { HH_STATIC_ASSERT(_AxisDimensionCount >= 3); return m_Axes[0].xyz(); }
	HH_FORCEINLINE const TVector<_Type, 3>	&StrippedYAxis() const { HH_STATIC_ASSERT(_AxisDimensionCount >= 3); return m_Axes[1].xyz(); }
	HH_FORCEINLINE const TVector<_Type, 3>	&StrippedZAxis() const { HH_STATIC_ASSERT(_AxisCount >= 3 && _AxisDimensionCount >= 3); return m_Axes[2].xyz(); }
	HH_FORCEINLINE const TVector<_Type, 3>	&StrippedWAxis() const { HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 3); return m_Axes[3].xyz(); }
	HH_FORCEINLINE const TVector<_Type, 3>	&StrippedTranslations() const { HH_STATIC_ASSERT(_AxisCount >= 4 && _AxisDimensionCount >= 3); return m_Axes[3].xyz(); }

	//----------------------------------------------------------------------------
	//	ops
	//	ex: operator +
	//
	//	| m00 m01 m02 m03 |   | n00 n01 n02 n03 |   | (m00 + n00) (m01 + n01) (m02 + n02) (m03 + n03) |
	//	| m10 m11 m12 m13 | + | n10 n11 n12 n13 | = | (m10 + n10) (m11 + n11) (m12 + n12) (m13 + n13) |
	//	| m20 m21 m22 m23 |   | n20 n21 n22 n23 |   | (m20 + n20) (m21 + n21) (m22 + n22) (m23 + n23) |
	//	| m30 m31 m32 m33 |   | n30 n31 n32 n33 |   | (m30 + n30) (m31 + n31) (m32 + n32) (m33 + n33) |

#define	HH_MATHS_MATRIX_BUILD_OP(__which) \
	SelfType	operator __which (const SelfType &other) const \
	{ \
		SelfType	result; \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				result.m_Axes[i].Axis(j) = m_Axes[i].Axis(j) __which other.m_Axes[i].Axis(j); \
		return result; \
	} \
	SelfType	operator __which (const _Type &scalar) const \
	{ \
		SelfType	result; \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				result.m_Axes[i].Axis(j) = m_Axes[i].Axis(j) __which scalar; \
		return result; \
	}

#define	HH_MATHS_MATRIX_BUILD_AOP(__which) \
	void	operator __which (const SelfType &other) \
	{ \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				m_Axes[i].Axis(j) __which other.m_Axes[i].Axis(j); \
	} \
	void	operator __which (const _Type &scalar) \
	{ \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				m_Axes[i].Axis(j) __which scalar; \
	}

#define	HH_MATHS_MATRIX_BUILD_COP(__which) \
	bool	operator __which (const SelfType &other) const \
	{ \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				if (!(m_Axes[i].Axis(j) __which other.m_Axes[i].Axis(j))) \
					return false; \
		return true; \
	} \
	bool	operator __which (const _Type &scalar) const \
	{ \
		for (hh_u32 i = 0; i < _AxisCount; i++) \
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++) \
				if (!(m_Axes[i].Axis(j) __which scalar)) \
					return false; \
		return true; \
	}

	//----------------------------------------------------------------------------

	HH_MATHS_MATRIX_BUILD_OP(+);
	HH_MATHS_MATRIX_BUILD_OP(-);

	SelfType	operator - () const;

	HH_MATHS_MATRIX_BUILD_AOP(+=);
	HH_MATHS_MATRIX_BUILD_AOP(-=);

	HH_MATHS_MATRIX_BUILD_COP(==);
	bool	operator != (const SelfType &other) const { return !(*this == other); }
	bool	operator != (const _Type &scalar) const { return !(*this == scalar); }

	//----------------------------------------------------------------------------
	//
	// | m00 m01 m02 m03 |   | n00 n01 n02 n03 |
	// | m10 m11 m12 m13 | . | n10 n11 n12 n13 |
	// | m20 m21 m22 m23 |   | n20 n21 n22 n23 |
	// | m30 m31 m32 m33 |   | n30 n31 n32 n33 |

	template<typename _Type2, hh_u32 _InputAxisCount, hh_u32 _InputAxisDimensionCount>
	TMatrixImpl<typename TNumericPromoter<_Type, _Type2>::PromotedType, _AxisCount, _InputAxisDimensionCount>	operator * (const TMatrixImpl<_Type2, _InputAxisCount, _InputAxisDimensionCount> &multiplyWith) const
	{
		HH_STATIC_ASSERT(_AxisDimensionCount == _InputAxisCount);
		typedef	typename TNumericPromoter<_Type, _Type2>::PromotedType	ResultType;

		TMatrixImpl<ResultType, _AxisCount, _InputAxisDimensionCount>	out;
		for (hh_u32 i = 0; i < _AxisCount; i++)	// left.rows
		{
			for (hh_u32 j = 0; j < _InputAxisDimensionCount; j++)	// right.cols
			{
				ResultType	accum = m_Axes[i].Axis(0) * multiplyWith.m_Axes[0].Axis(j);
				for (hh_u32 k = 1; k < _InputAxisCount; k++)	// right.rows
				{
					accum += m_Axes[i].Axis(k) * multiplyWith.m_Axes[k].Axis(j);
				}
				out.m_Axes[i].Axis(j) = accum;
			}
		}
		return out;
	}

	// scalars
	SelfType	operator * (_Type scalar) const
	{
		SelfType	out;
		for (hh_u32 i = 0; i < _AxisCount; i++)	// left.rows
		{
			for (hh_u32 j = 0; j < _AxisDimensionCount; j++)	// right.cols
			{
				out.m_Axes[i].Axis(j) = m_Axes[i].Axis(j) * scalar;
			}
		}
		return out;
	}

	HH_FORCEINLINE void	operator *= (const SelfType &multiplyWith) { *this = this->operator * (multiplyWith); }
	HH_FORCEINLINE void	operator *= (_Type scalar) { *this = this->operator * (scalar); }

	//----------------------------------------------------------------------------
	// inversions

	SelfType				Inverse() const;
	void					Invert();
	void					Invert(SelfType &output) const;

	// inverts a matrix containing only an unscaled rotational part.
	// the function works only on input matrices of the form:
	// 
	// | m00 m01 m02  0  |
	// | m10 m11 m12  0  |
	// | m20 m21 m22  0  |
	// |  0   0   0   1  |
	// 
	//                    -1
	// | M00 m01 m02 m03 |     | M00 m10 m20 m03 |
	// | m10 M11 m12 m13 |  =  | m01 M11 m21 m13 |
	// | m20 m21 M22 m23 |     | m02 m12 M22 m23 |
	// | m30 m31 m32 m33 |     | m30 m31 m32 m33 |

	SelfType				InversePureRotations() const;
	void					InvertPureRotations();
	void					InvertPureRotations(SelfType &output) const;

	// inverts a matrix containing both unscaled rotations and translations:
	// the function works only on input matrices of the form:
	// 
	// | m00 m01 m02  0  |
	// | m10 m11 m12  0  |
	// | m20 m21 m22  0  |
	// | m30 m31 m32  1  |
	// 
	// (basically a 4*3 matrix extended to 4x4)
	// 
	//                    -1
	// | m00 m01 m02 m03 |     |              M00                             m10                             m20                             m03              |
	// | m10 m11 m12 m13 |  =  |              m01                             M11                             m21                             m13              |
	// | m20 m21 m22 m23 |     |              m02                             m12                             M22                             m23              |
	// | m30 m31 m32 m33 |     |-(m30.m00 + m31.m01 + m32.m02)  -(m30.m10 + m31.m11 + m32.m12)  -(m30.m20 + m31.m21 + m32.m22)                m33              |

	SelfType				InversePureTransforms() const;
	void					InvertPureTransforms();
	void					InvertPureTransforms(SelfType &output) const;

	// | m00 * s   m01 * s   m02 * s   m03 * s |
	// | m10 * s   m11 * s   m12 * s   m13 * s |
	// | m20 * s   m21 * s   m22 * s   m23 * s |
	// | m30 * s   m31 * s   m32 * s   m33 * s |

	void					Scale(_Type scale);

	// | m00 m01 m02 m03 |   | sx |   |  m00 * sx   m01 * sy   m02 * sz   m03 * sw  |
	// | m10 m11 m12 m13 | . | sy | = |  m10 * sx   m11 * sy   m12 * sz   m13 * sw  |
	// | m20 m21 m22 m23 |   | sz |   |  m20 * sx   m21 * sy   m22 * sz   m23 * sw  |
	// | m30 m31 m32 m33 |   | sw |   |  m30 * sx   m31 * sy   m32 * sz   m33 * sw  |

	void					Scale(const TVector<_Type, _AxisDimensionCount> &scale);

	// | m00 m01 m02 m03 |   | x |
	// | m10 m11 m12 m13 | . | y |
	// | m20 m21 m22 m23 |   | z |
	// | m30 m31 m32 m33 |   | 1 |

	TVector<_Type, 3>		TransformVector(const TVector<_Type, 3> &vec) const;

	// | m00 m01 m02 m03 |   | x |
	// | m10 m11 m12 m13 | . | y |
	// | m20 m21 m22 m23 |   | z |
	// | m30 m31 m32 m33 |   | w |

	TVector<_Type, 4>		TransformVector(const TVector<_Type, 4> &vec) const;

	// | m00 m01 m02 m03 |   | x |
	// | m10 m11 m12 m13 | . | y |
	// | m20 m21 m22 m23 |   | z |
	// | m30 m31 m32 m33 |   | 0 |

	TVector<_Type, 3>		RotateVector(const TVector<_Type, 3> &vec) const;
	TVector<_Type, 4>		RotateVector(const TVector<_Type, 4> &vec) const;


	// matrix transpose (swap the matrix along its diagonal)
	// 
	// | M00 m01 m02 m03 |     | M00 m10 m20 m30 |
	// | m10 M11 m12 m13 | --> | m01 M11 m21 m31 |
	// | m20 m21 M22 m23 | --> | m02 m12 M22 m23 |
	// | m30 m31 m32 M33 |     | m03 m13 m23 M33 |

	TMatrixImpl<_Type, _AxisDimensionCount, _AxisCount>	Transposed() const;
	void												Transpose();

	//----------------------------------------------------------------------------
	//	crop helpers: we want, whenever possible, to return const refs if the sub-rects span a contiguous memory area:
	//----------------------------------------------------------------------------

	template<hh_u32 _SubRectAxisStart, hh_u32 _NewAxisCount, hh_u32 _SubRectAxisDimensionStart, hh_u32 _NewAxisDimensionCount> struct	TCropHelper;
#ifndef	HH_COMPILER_CLANG
	template<hh_u32 _SubRectAxisStart, hh_u32 _NewAxisCount> struct	TCropHelper<_SubRectAxisStart, _NewAxisCount, 0, _AxisDimensionCount>;	// ORBIS
#endif

	//	TMatrixImpl<4,4>::Crop<3,2>();
	//
	//	| m00 m01 m02 m03 |    | m00 m01 |
	//	| m10 m11 m12 m13 | -> | m10 m11 |
	//	| m20 m21 m22 m23 |    | m20 m21 |
	//	| m30 m31 m32 m33 |

	template<hh_u32 _NewAxisCount, hh_u32 _NewAxisDimensionCount>
	typename TCropHelper<0, _NewAxisCount, 0, _NewAxisDimensionCount>::_ReturnType	Crop() const { return TCropHelper<0, _NewAxisCount, 0, _NewAxisDimensionCount>::Crop(*this); }

	//	TMatrixImpl<3,2>::Extent<4,4>(v);
	//
	// | m00 m01 |    | m00 m01  v   v  |
	// | m10 m11 | -> | m10 m11  v   v  |
	// | m20 m21 |    | m20 m21  v   v  |
	//                |  v   v   v   v  |

	template<hh_u32 _NewAxisCount, hh_u32 _NewAxisDimensionCount>
	TMatrixImpl<_Type, _NewAxisCount, _NewAxisDimensionCount>			Extend(_Type fillWith = TNumericTraits<_Type>::Zero()) const;

	HH_FORCEINLINE _Type												Determinant() const;

	//----------------------------------------------------------------------------

	bool							Homogeneous(float tolerance = 1.0e-5f) const;
	bool							Orthogonal(float tolerance = 1.0e-5f) const;
	bool							Orthonormal(float tolerance = 1.0e-5f) const;
	bool							Scaled(_Type tolerance = 1.0e-5f) const;
	bool							Equals(const SelfType &other, _Type tolerance = 1.0e-5f) const;
	bool							IsIdentity(_Type tolerance = 1.0e-5f) const;
	bool							IsLeftHanded() const;

	TVector<_Type, 3>				AbsScalingFactors() const;	// WARNING: does not handle mirroring, the magnitudes will always be > 0... fix this? see what it would cost, and possibly make another method for when we care about negative scales
	TVector<_Type, 3>				AbsScalingFactorsSquared() const;
	TVector<_Type, 3>				ScalingFactors() const;
	_Type							UniformScalingFactor() const;
	void							RemoveScale();
	void							RemoveAbsScale();
	void							Explode(TVector<_Type, 3> &translations, TMatrixImpl<_Type, 3, 3> &rotations, TVector<_Type, 3> &scale) const;
	void							Explode(TVector<_Type, 3> &translations, TMatrixImpl<_Type, 3, 3> &rotations, TVector<_Type, 3> &scale, TVector<_Type, 3> &shear) const;

	//	v = diagonalValue			| v   0   0   0 |
	//								| 0   v   0   0 |
	//								| 0   0   v   0 |
	//								| 0   0   0   v |

	static SelfType					BuildDiagonalMatrix(_Type diagonalValue);
};

//----------------------------------------------------------------------------

typedef TMatrix<float, 3, 3>	CFloat3x3;
typedef TMatrix<float, 3, 4>	CFloat3x4;
typedef TMatrix<float, 4, 3>	CFloat4x3;
typedef TMatrix<float, 4, 4>	CFloat4x4;

HH_STATIC_ASSERT(sizeof(CFloat3x3) == 36);
HH_STATIC_ASSERT(sizeof(CFloat4x3) == 48);
HH_STATIC_ASSERT(sizeof(CFloat3x4) == 48);
HH_STATIC_ASSERT(sizeof(CFloat4x4) == 64);

//----------------------------------------------------------------------------

//template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
//TMatrix<_Type, _AxisCount, _AxisDimensionCount>	PKSwapEndian(const TMatrix<_Type, _AxisCount, _AxisDimensionCount> &object)
//{
//	TMatrix<_Type, _AxisCount, _AxisDimensionCount>	out = object;
//	PKSwapEndianInPlace(out);
//	return out;
//}

namespace	Internal	// kr_memoryviews.h
{
	template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
	struct	TStreamEndianSwapperHelpers<TMatrix<_Type, _AxisCount, _AxisDimensionCount> >
	{
		static void	SwapInPlace(TMatrix<_Type, _AxisCount, _AxisDimensionCount> *data, hh_u32 count) { TNativeSwapper<sizeof(_Type)>::SwapInPlace(data, count * _AxisCount * _AxisDimensionCount); }
	};
}

template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
void	PKSwapEndianInPlace(TMatrix<_Type, _AxisCount, _AxisDimensionCount> &object)
{
	for (hh_u32 i = 0; i < _AxisCount; i++)
	{
		PKSwapEndianInPlace(object.Axis(i));
	}
}

//----------------------------------------------------------------------------

namespace	TTypeTraits
{
	template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>	struct	IsInteger<TMatrix<_Type, _AxisCount, _AxisDimensionCount> > : IsInteger<_Type> {};
	template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>	struct	IsFloat<TMatrix<_Type, _AxisCount, _AxisDimensionCount> > : IsFloat<_Type> {};
	template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>	struct	IsSigned<TMatrix<_Type, _AxisCount, _AxisDimensionCount> > : IsSigned<_Type> {};
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_maths_matrix.inl"

#endif // __PK_MATHS_MATRIX_H__
