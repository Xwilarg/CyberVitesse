//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXRefPtrWrap.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_refptr.h>
#include <hh_kernel/include/kr_mem_utils.h>

//----------------------------------------------------------------------------

void	CPopcornFXRefPtrWrap_Base::Set(PopcornFX::CRefCountedObject *rawPtr)
{
	Clear();
	if (rawPtr != null)
	{
		//m_RefTrackingSlot = const_cast<_WriteableType*>(m_Ptr)->AddReference(ONLY_IN_UBER_REFPTR_DEBUG(true));
		HH_ASSERT(HH_MEM_LOOKS_VALID(rawPtr));
		m_Ptr = rawPtr;
		const_cast<PopcornFX::CRefCountedObject*>(m_Ptr)->AddReference();
	}
}

//----------------------------------------------------------------------------

void	CPopcornFXRefPtrWrap_Base::Clear()
{
	if (m_Ptr != null)
	{
		HH_ASSERT(HH_MEM_LOOKS_VALID(m_Ptr));
		//const_cast<_WriteableType*>(m_Ptr)->RemoveReference(m_Ptr ONLY_IN_UBER_REFPTR_DEBUG(COMMA m_RefTrackingSlot));
		//ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot = hh_u32(-1));
		const_cast<PopcornFX::CRefCountedObject*>(m_Ptr)->RemoveReference(m_Ptr);
		m_Ptr = null;
	}
}

//----------------------------------------------------------------------------
