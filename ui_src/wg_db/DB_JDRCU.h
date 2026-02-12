#ifndef __DB_JDRCU_DB_H__
#define __DB_JDRCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDRCU
{
public:
	CDB_JDRCU()
	{
		m_JDRC.InitHashTable(HASHSIZEJDRC);
	}
	virtual ~CDB_JDRCU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDRC_UDRD_D& rData)
		{m_JDRC.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDRC.RemoveKey(Key);}
	void DelAll()
		{m_JDRC.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDRC_UDRD_D& rData)
		{return m_JDRC.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDRC_UDRD_D,T_JDRC_UDRD_D&> m_JDRC;
};

#endif