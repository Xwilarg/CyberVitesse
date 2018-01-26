#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2012/11/8 20:45 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_COORDINATE_FRAME_H__
#define __GE_COORDINATE_FRAME_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

enum	ECoordinateFrame		// side  up  fwd
{
	Frame_RightHand_Y_Up = 0,	// X Y Z	(OpenGL)
	Frame_RightHand_Z_Up,		// X Z -Y	(Max)
	Frame_LeftHand_Y_Up,		// X Y -Z
	Frame_LeftHand_Z_Up,		// X Z Y

	__MaxCoordinateFrames
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CCoordinateFrame
{
public:
	static ECoordinateFrame		GlobalFrame();

	static const CFloat4x4		&GlobalFrameTransform();
	static const CFloat4x4		&GlobalFrameInvTransform();

	static const CFloat3		&UpAxis();
	static const CFloat3		&ForwardAxis();
	static const CFloat3		&SideAxis();

	static const CFloat3		MatrixUpAxis(const CFloat4x4 &transforms);
	static const CFloat3		MatrixForwardAxis(const CFloat4x4 &transforms);
	static const CFloat3		MatrixSideAxis(const CFloat4x4 &transforms);

	static bool					IsRightHanded();
	static float				CrossSign();		// assuming your Cross is right-handed

	static const CInt3			&AxesRemapper(ECoordinateFrame frame); // signed axes, 1-based. negative if the axis is flipped. 'x' is 1, 'y' is 2, 'z' is  3. contained in the [-3,-1];[1,3] range
	static const CInt3			&AxesRemapper();
	static const CUint3			AxesIndexer() { return HHAbs(AxesRemapper()) - 1; }

	// note: bad to call during runtime. changing coordinate frames won't change the ones of the elements
	// already loaded in memory. this should only be changed at startup.
	static void					SetGlobalFrame(ECoordinateFrame frame);

	static void					TransposeFrame(ECoordinateFrame from, ECoordinateFrame to, const TStridedMemoryView<CFloat3> &stream);
	static void					TransposeFrame(ECoordinateFrame from, ECoordinateFrame to, const TStridedMemoryView<CFloat4> &stream);	// doesn't touch the 'w' component

	static void					BuildLocalFrame(ECoordinateFrame frame, CFloat4x4 &outTransform, CFloat4x4 &outInvTransform);
	static void					BuildTransitionFrame(ECoordinateFrame from, ECoordinateFrame to, CFloat4x4 &outTransform);
	//static SIMD::Float4x4		BuildTransitionFrame(ECoordinateFrame from, ECoordinateFrame to);

	static bool					IsFrameRightHanded(ECoordinateFrame frame);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GE_COORDINATE_FRAME_H__
