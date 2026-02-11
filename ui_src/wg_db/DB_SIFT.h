// DB_SIFT.h: interface for the CDB_SIFT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_SIFT_DB_H__)
#define __DB_SIFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SIFT
{
public:
	CDB_SIFT();
	virtual ~CDB_SIFT();
	CDBDoc* m_pDoc;

public:
	void Add(T_SIFT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SIFT_D& rData);
	int GetCount();

protected:
	CMap<T_SIFT_K, T_SIFT_K, T_SIFT_D, T_SIFT_D&> m_sift;
};

#endif // !defined(__DB_SIFT_DB_H__)
