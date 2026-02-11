#ifndef __DB_JDMSU_DB_H__
#define __DB_JDMSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDMSU
{
public:
	CDB_JDMSU()
	{
		m_JDMS.InitHashTable(HASHSIZEJDMS);
	}
	virtual ~CDB_JDMSU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDMS_UDRD_D& rData)
		{m_JDMS.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDMS.RemoveKey(Key);}
	void DelAll()
		{m_JDMS.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDMS_UDRD_D& rData)
		{return m_JDMS.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDMS_UDRD_D,T_JDMS_UDRD_D&> m_JDMS;
};

#endif