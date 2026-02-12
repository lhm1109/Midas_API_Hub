// DB_SURS.h: interface for the CDB_SURS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_SURS_DB_H__)
#define __DB_SURS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SURS  
{
public:
	CDB_SURS();
	virtual ~CDB_SURS();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_SURS_D& rData);
	BOOL Del();
	
public:
	BOOL Get(T_SURS_D& rData);
	int GetCount();
	
protected:
	CMap<T_SURS_K,T_SURS_K,T_SURS_D,T_SURS_D&> m_surs;
};

#endif // !defined(__DB_SURS_DB_H__)
