// DB_LLRF.h: interface for the CDB_LLRF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_LLRF_DB_H__)
#define __DB_LLRF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LLRF
{
public:
	CDB_LLRF();
	virtual ~CDB_LLRF();
	CDBDoc* m_pDoc;

public:
	void Add(T_LLRF_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LLRF_D& rData);
	int GetCount();

protected:
	CMap<T_LLRF_K, T_LLRF_K, T_LLRF_D, T_LLRF_D&> m_llrf;
};

#endif // !defined(__DB_LLRF_DB_H__)
