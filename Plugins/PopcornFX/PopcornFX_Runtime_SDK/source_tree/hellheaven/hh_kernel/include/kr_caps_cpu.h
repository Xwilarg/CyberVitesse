#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2008/09/28 18:52 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CAPS_CPU_H__
#define __KR_CAPS_CPU_H__

#include "hh_kernel/include/kr_caps.h"
#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_callbacks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	CPU CAPS
//
//----------------------------------------------------------------------------

class	CCapsCPUExtensions
{
public:
	enum	ECapability
	{
		VirtualMode = 0,	// Virtual Mode Extension : The processor supports extensions to virtual-8086 mode.
		Debugging,			// Debugging Extension : The processor supports I/O breakpoints, including the CR4. DE bit for enabling debug extensions and optional trapping of access to the DR4 and DR5 registers.
		PageSize,			// Page Size Extension : The processor supports 4-Mbyte pages.
		RDTSC,				// Time-Stamp Counter : The RDTSC instruction is supported including the CR4.TSD bit for access/privilege control.
		MSR,				// Model Specific Registers : Model Specific Registers are implemented with the RDMSR, WRMSR instructions
		PAE,				// Physical Address Extension
		MCE,				// Machine Check Exception
		CXCHG8,				// CMPXCHG8 Instruction
		APIC,				// On-chip APIC Hardware
		SEP,				// Fast System Call
		MTRR,				// Memory Type Range Registers
		PGE,				// Page Global Enable
		MCA,				// Machine Check
		CMOV,				// Conditional Move Instruction
		PAT,				// Page Attribute Table
		PSE36,				// 36-bit Page Size Extension
		CLFlush,			// CLFLUSH Instruction supported
		DebugStore,			// Debug Store
		ACPI,				// Thermal Monitor and Software Controlled Clock Facilities supported
		FXSR,				// Fast floating point save and restore
		SelfSnoop,			// Self-Snoop
		HyperThreading,		// HyperThreading
		ThermalMonitor,		// Thermal Monitor
		ThermalMonitor2,	// Thermal Monitor 2
		IA64,				// IA64 Capabilities
		IA64Extensions,		// IA64 Extensions
		PBE,				// Pending Break Enable

		ExecutionDisable,	// The processor supports the XD Bit when PAE mode paging is enabled.

		DTES64,				// 64-Bit Debug Store
		MONITOR,			// MONITOR/MWAIT
		DS_CPL,				// CPL Qualified Debug Store
		VMEXT,				// Virtual Machine Extensions
		SMEXT,				// Safer Mode Extensions
		ESpeedStep,			// Enhanced Intel ESpeedStep Technology
		CID,				// Context ID
		CXCHG16B,			// CMPXCHG16B
		XTPR,				// Send Task Priority Messages
		PDCM,				// Perfmon and Debug Capability
		DCA,				// Direct Cache Access
		x2APIC,				// extended APIC ID
		XSave,				// XSAVE/XSTOR States
		OsXSave,			// OS-Enabled Extended State Management
		InvariantTSC,		// ISC is invariant across states

		AMD_AA64,
		AMD_3DNow,
		AMD_3DNowExt,
		AMD_3DNowPrefetch,
		AMD_IBSampling,
		AMD_MisalignedSSE,
		AMD_SSE4A,
		AMD_ABM,
		AMD_LAHF_SAHF,
		AMD_RDTSCP,
		AMD_EMMX,

		MOVBE,				// MOVBE instruction
		POPCNT,				// POPCNT instruction
		CLMUL,				// CLMUL instruction
		LZCNT,				// LZCNT instruction
		BMI1,				// BMI1 instructions
		BMI2,				// BMI2 instructions
		F16C,				// F16C instructions
		AES,				// AES instructions
		FMA3,				// FMA3 instructions
		FMA4,				// FMA4 instructions
		RDRAND,				// rdrand instruction

		MMX,				// Intel Architecture MMX technology supported
		SSE,				// Streaming SIMD Extensions supported
		SSE2,				// Streaming SIMD Extensions 2 supported
		SSE3,				// Streaming SIMD Extensions 3
		SSSE3,				// Supplemental Streaming SIMD Extensions 3
		SSE4_1,				// Streaming SIMD Extensions 4.1
		SSE4_2,				// Streaming SIMD Extensions 4.2
		AVX,				// Advanced Vector Extensions
		AVX2,				// Advanced Vector Extensions 2
		AVX512,				// Advanced Vector Extensions 512-bits

		ARM_NEON,

		__ExtensionsCount
	};

private:
	typedef hh_u32			FlagArrayElementType;
	static const hh_u32		FlagsArraySize = (__ExtensionsCount - 1) / 32 + 1;
	static const hh_u32		FlagsArrayShift = TIntegerTemplateTools::Log2<sizeof(FlagArrayElementType) << 3>::Value;
	static const hh_u32		FlagsArrayMask = (1 << FlagsArrayShift) - 1;

	FlagArrayElementType	m_Extensions[FlagsArraySize];

public:
	CCapsCPUExtensions() { Clear(); }

	static const char		*Name(ECapability cap);
	bool					Supported(ECapability cap) const { return (m_Extensions[(hh_u32)cap >> FlagsArrayShift] & (1 << ((hh_u32)cap & FlagsArrayMask))) != 0; }
	void					SetCap(ECapability cap) { m_Extensions[(hh_u32)cap >> FlagsArrayShift] |= (1 << ((hh_u32)cap & FlagsArrayMask)); }
	void					ClearCap(ECapability cap) { m_Extensions[(hh_u32)cap >> FlagsArrayShift] &= ~(1 << ((hh_u32)cap & FlagsArrayMask)); }
	hh_u32					CapSetCount() const;

	void					Clear();
	void					Merge_AND(const CCapsCPUExtensions &other);
	void					Merge_OR(const CCapsCPUExtensions &other);
};

//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT SCapsCPUCacheDescriptor
{
	enum	EType
	{
		CacheType_Invalid = 0,
		CacheType_Data = 1,
		CacheType_Instruction = 2,
		CacheType_Unified = 3,

		__MaxCacheTypes
	};

	hh_u32	m_SizeInKb;
	hh_u32	m_LineSizeInBytes;
	hh_u8	m_Associativity;
	hh_u8	m_Level;	// starts at 1->L1, 2->L2, etc.. 0->invalid
	EType	m_Type;

	hh_u32	m_SelectionMask;

	SCapsCPUCacheDescriptor() : m_SizeInKb(0), m_LineSizeInBytes(0), m_Associativity(0), m_Level(0), m_Type(CacheType_Invalid), m_SelectionMask(0) {}

	bool	FullyAssociative() const { return m_Associativity == 0xFF; }
	bool	DirectMapped() const { return m_Associativity == 1; }

	bool	Valid() const { return m_Level != 0 && m_Type != CacheType_Invalid && m_SizeInKb != 0 && m_LineSizeInBytes != 0 && m_Associativity != 0; }
};

//----------------------------------------------------------------------------

struct			SCapsCPUCore
{
	TSemiDynamicArray<CGuid, 2>	m_HWThreads;
};

//----------------------------------------------------------------------------

struct			HH_KERNEL_EXPORT SCapsCPUPackage
{
	typedef TSemiDynamicArray<SCapsCPUCacheDescriptor, 3>	CacheDescriptorsList;
	typedef TSemiDynamicArray<SCapsCPUCore, 2>				CoresList;

	enum	EVendor
	{
		Vendor_Unknown = 0,
		Vendor_Intel,
		Vendor_AMD,
		Vendor_IBM,
		Vendor_Sony_IBM,
		Vendor_ARM,
	};

	hh_u32					m_PackageID;
	EVendor					m_Vendor;
	char					m_Name[48];
	hh_u32					m_Model;

	float					m_Frequency;	// Hz
	float					m_CyclesToSeconds;

	hh_u32					m_MaxCPUIDLeaf;
	hh_u32					m_MaxCPUIDLeafExt;

	CCapsCPUExtensions		m_Extensions;
	CoresList				m_Cores;

	CacheDescriptorsList	m_CacheDescriptors;

	SCapsCPUPackage();

	bool					Initialized() const { return m_PackageID != 0xFFFFFFFF; }

	HH_FORCEINLINE bool		Supports(CCapsCPUExtensions::ECapability cap) const { return m_Extensions.Supported(cap); }

	// Processor clock frequency in Hz
	float					Frequency() const;	// rounds to the nearest 10 MHz
	float					MeasuredFrequency() const { return m_Frequency; }
};

//----------------------------------------------------------------------------

struct			SCapsCPULogicalProcessor
{
	hh_u32		m_APICID;		// contains either the full x2APICID or the legacy APICID

	hh_u32		m_PackageID;
	hh_u32		m_CoreID;
	hh_u32		m_HWThreadID;

	TSemiDynamicArray<hh_u8, 4>	m_CacheIDs;	// for each cache level, index into the cache instance list of the parent processor package. range: [0,0xFE]. by default, we have space for L1D, L1I, L2, and L3 before allocating...

	static HH_FORCEINLINE hh_u8	BUildCacheID(hh_u32 idInCacheDescriptor, hh_u32 cachePhysicalIndexAtLevel)
	{
		return (idInCacheDescriptor << 4) | (cachePhysicalIndexAtLevel & 0xF);
	}

	SCapsCPULogicalProcessor() : m_APICID(0), m_PackageID(0), m_CoreID(0), m_HWThreadID(0) {}
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#if	(KR_CAPS_DETECT_CPU != 0)
struct	SCPUDetectionToolkit;
#endif

namespace CPU
{

	class	HH_KERNEL_EXPORT CCaps : public CBaseDigestableCaps
	{
	public:
		typedef TSemiDynamicArray<SCapsCPUPackage, 1>						ProcessorPackageList;
		typedef TSemiDynamicArray<SCapsCPULogicalProcessor, 8>				LogicalProcessorList;
	protected:
#if	(KR_CAPS_DETECT_CPU != 0)
		friend struct				PopcornFX::SCPUDetectionToolkit;
#endif

		CCapsCPUExtensions			m_CommonExtensions;
		CCapsCPUExtensions			m_CommonExtensionsOverride;

		hh_u32						m_MaxLogicalCPUsSeenByOS;
		hh_u32						m_CoreCount;		// total number of cores accumulated across all CPUs

		CGenericAffinityMask		m_ProcessAffinity;

		ProcessorPackageList		m_ProcessorPackages;
		LogicalProcessorList		m_LogicalProcessors;

#if	(KR_CAPS_DETECT_CPU != 0)
		void						BuildCPUHierarchy();
		void						BuildCacheHierarchy();
#endif
		void						SanityCheck();		// performs a sanity check on the whole CPU caps structure to make sure there aren't inconsistencies

	public:
		CCaps();
		~CCaps();

#if	(KR_CAPS_DETECT_CPU != 0)
		bool						Detect();
#endif

		HH_FORCEINLINE bool			Multicore() const { return m_CoreCount > m_ProcessorPackages.Count(); }
		HH_FORCEINLINE bool			SupportsMultiThreading() const { return m_CommonExtensions.Supported(CCapsCPUExtensions::HyperThreading); }	// HT
		HH_FORCEINLINE bool			MultiThreaded() const { HH_ASSERT(m_LogicalProcessors.Count() <= m_CoreCount || SupportsMultiThreading()); return m_LogicalProcessors.Count() > m_CoreCount; }
		HH_FORCEINLINE hh_u32		NumberOfCores() const { return m_CoreCount; }
		HH_FORCEINLINE hh_u32		NumberOfLogicalProcessors() const { return m_LogicalProcessors.Count(); }

		// Package-specific extensions (can't be overridden)
		const CCapsCPUExtensions	&Extensions(hh_u32 packageId) const { return m_ProcessorPackages[packageId].m_Extensions; }

		// HW extensions common to all CPU packages
		const CCapsCPUExtensions	&CommonExtensionsBase() const { return m_CommonExtensions; }
		const CCapsCPUExtensions	&CommonExtensionsOverride() const { return m_CommonExtensionsOverride; }
		// Override extensions. masks 'newExtensions' with 'CommonExtensionsBase', and calls the re-dispatch callbacks
		void						OverrideCommonExtensions(const CCapsCPUExtensions &newExtensions, bool redispatch = true);

		mutable TCallbackChainUnordered<void()>	m_CPUDispatch;	// Will be called whenever 'OverrideCommonExtensions(..., true)' is called

		HH_FORCEINLINE bool			Supports(CCapsCPUExtensions::ECapability cap) const { return m_CommonExtensionsOverride.Supported(cap); }

		void						Log() const;	// Dumps detailed detection report to logfile
		virtual void				FillDigestStream(CStream &stream) const override;

		const CGenericAffinityMask	&ProcessAffinity() const { return m_ProcessAffinity; }

		const ProcessorPackageList	&ProcessorPackages() const { return m_ProcessorPackages; }
		const LogicalProcessorList	&LogicalProcessors() const { return m_LogicalProcessors; }
	};

	//----------------------------------------------------------------------------

	HH_KERNEL_EXPORT const CCaps	&Caps();
	HH_KERNEL_EXPORT CCaps			&CapsForWriting();	// watchout from where you call this !
};

//----------------------------------------------------------------------------
//
//	CPU modes helpers
//
//----------------------------------------------------------------------------

namespace	CPU
{

	class	CScoped_FpuEnableFastMode : public CNonCopyable
	{
	private:
#if	(HH_CPU_ARCH == HH_CPU_X86)
		hh_u32	m_PrevMXCSR;
#endif
	public:
		CScoped_FpuEnableFastMode();
		~CScoped_FpuEnableFastMode();
	};

	class	CScoped_FpuDisableExceptions : public CNonCopyable
	{
	private:
#if	defined(HH_WINDOWS) || defined(HH_LINUX)
		hh_u32	m_PrevFPCW;
#endif
#if	defined(HH_WINDOWS)
		hh_u32	m_PrevMXCW;
#endif
	public:
		CScoped_FpuDisableExceptions();
		~CScoped_FpuDisableExceptions();
	};

}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CAPS_CPU_H__
