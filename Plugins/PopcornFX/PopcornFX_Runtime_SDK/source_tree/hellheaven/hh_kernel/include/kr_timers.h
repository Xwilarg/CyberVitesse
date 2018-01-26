#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 2:52 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_TIMERS_H__
#define	__KR_TIMERS_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type> class	TBaseTimer;

//----------------------------------------------------------------------------

template<typename _Type>
class	TTimer : public TBaseTimer<_Type>
{
private:
	_Type	m_Accumulator;

protected:
public:
	TTimer() : m_Accumulator(0) {}
	~TTimer() {}

	/*HH_INLINE*/ void			Start() { m_Accumulator = 0; TBaseTimer<_Type>::_InternalStart(); }
	HH_FORCEINLINE _Type		Read() const { return m_Accumulator + TBaseTimer<_Type>::_InternalRead(); }	// undefined results if the timer is stopped
	/*HH_INLINE*/ _Type			Pause() { _Type duration = Stop(); m_Accumulator = duration; return duration; }
	/*HH_INLINE*/ _Type			Resume() { TBaseTimer<_Type>::_InternalStart(); return m_Accumulator; }
	/*HH_INLINE*/ void			Reset() { Start(); }
	/*HH_INLINE*/ _Type			Stop() { _Type duration = m_Accumulator + TBaseTimer<_Type>::_InternalStop(); m_Accumulator = 0; return duration; }
};

//----------------------------------------------------------------------------
//
//	CPU Cycles timer
//	typical overhead on a P4: ~63 cycles
//
//----------------------------------------------------------------------------

template<typename _TCycles>
class	TTimerCPUCycles
{
private:
	typedef typename TNumericTraits<_TCycles>::SignedType	_TSignedCycles;

	_TCycles			m_CyclesStart;
	_TCycles			m_CyclesEnd;

public:
	TTimerCPUCycles();

	void				Start(bool serializeCPU = false);
	_TCycles			Stop(bool serializeCPU = false);						// returns real cycles
	_TCycles			Read(bool serializeCPU = false) const;					// returns real cycles
	static _TCycles		RawCPUCycles(bool serializeCPU = false);				// returns raw cycles
	static _TCycles		RawProcessorTimerOverhead(bool serializeCPU = false);	// returns raw cycles
	static _TCycles		RawCPUCyclesToRealCycles(_TCycles cycles);				// converts raw cycles to real cycles
};

//----------------------------------------------------------------------------
//
//	CPU Cycles timer
//	full 64 bits precision, larger object
//	typical overhead on a P4: ~63 cycles
//
//----------------------------------------------------------------------------
// 16 bytes

typedef TTimerCPUCycles<hh_u64>	CTimerCPUCycles;

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_timers.inl"

__PK_API_BEGIN
//----------------------------------------------------------------------------

typedef TTimer<double>	CTimer;
//typedef TTimer<float>	CTimer;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_TIMERS_H__
