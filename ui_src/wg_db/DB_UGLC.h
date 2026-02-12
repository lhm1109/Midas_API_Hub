// DB_UGLC.h: interface for the CDB_UGLC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_UGLC_DB_H__)
#define __DB_UGLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_UGLC
{
public:
	CDB_UGLC();
	virtual ~CDB_UGLC();
	CDBDoc* m_pDoc;

public:
	void Add(T_UGLC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_UGLC_D& rData);
	int GetCount();

protected:
	CMap<T_UGLC_K, T_UGLC_K, T_UGLC_D, T_UGLC_D&> m_uglc;
};

#endif // !defined(__DB_UGLC_DB_H__)
