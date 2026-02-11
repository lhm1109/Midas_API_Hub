#ifndef __DB_JDLSU_DB_H__
#define __DB_JDLSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDLSU
{
public:
	CDB_JDLSU()
	{
		m_JDLS.InitHashTable(HASHSIZEJDLS);
	}
	virtual ~CDB_JDLSU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDLS_UDRD_D& rData)
		{m_JDLS.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDLS.RemoveKey(Key);}
	void DelAll()
		{m_JDLS.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDLS_UDRD_D& rData)
		{return m_JDLS.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDLS_UDRD_D,T_JDLS_UDRD_D&> m_JDLS;
};

#endif