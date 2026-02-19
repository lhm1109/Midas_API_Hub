// Dgn_DLLModuleBase.h: DLL wrapper base class. This should actually be moved to a library for use everywhere
// See Dgn_PSCReportModule.h for how to use. 	
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_DLLMODULEBASE_H__)
#define __DGN_DLLMODULEBASE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDLLModuleBase
{
public:
	CDLLModuleBase() : m_hDLL(NULL) { VERIFY(SUCCEEDED(OleInitialize(NULL))); }	  // If this fails there is not much we can do.
	virtual ~CDLLModuleBase(){ if (m_hDLL)	::FreeLibrary(m_hDLL);  OleUninitialize( ) ; }	// So that Ole uninitializes gracefully.
	
	virtual BOOL Init(LPCTSTR szDll)=0;

protected:
	HMODULE m_hDLL;
};

//////////////////////////////////////////////////////////////////////
// Macros to implement the Init function
#define DECLARE_DLL_FUNCTION(ret, func, params) \
	ret	(* func)params;

#define BEGIN_DLL_INIT() \
	BOOL Init(LPCTSTR szDll) \
	{ \
		if (m_hDLL) \
			::FreeLibrary(m_hDLL); \
		m_hDLL = ::LoadLibrary(szDll); \
		BOOL	bOk = TRUE;

#define INIT_DLL_FUNCTION(ret, func, params, origin) \
		if (m_hDLL) \
		{ \
			func = (ret (* )params)GetProcAddress(m_hDLL, origin); \
		} \
		else \
			func = NULL; \
		if (!func) \
			bOk = FALSE;

#define END_DLL_INIT() \
		return bOk; \
	} 

#endif // !defined(__DGN_DLLMODULEBASE_H__)
