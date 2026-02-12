#ifndef __DB_JDLS_DB_H__
#define __DB_JDLS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDLS
{
public:
	CDB_JDLS();
	virtual ~CDB_JDLS();

public:
	void Add(T_JDLS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDLS_D& rData)
		{return m_JDLS.Lookup(1,rData);}
	int GetCount()
		{return m_JDLS.GetCount();}

protected:
	CMap<T_JDLS_K,T_JDLS_K,T_JDLS_D,T_JDLS_D&> m_JDLS;
};

#endif