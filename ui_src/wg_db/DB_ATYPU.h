#ifndef __DB_ATYPU_DB_H__
#define __DB_ATYPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ATYPU
{
public:
	CDB_ATYPU()
	{
		m_atyp.InitHashTable(HASHSIZEATYP);
	}
	virtual ~CDB_ATYPU(){};

public:
	void Add(T_UDRD_KEY Key,T_ATYP_UDRD_D& rData)
		{m_atyp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_atyp.RemoveKey(Key);}
	void DelAll()
		{m_atyp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ATYP_UDRD_D& rData)
		{return m_atyp.Lookup(Key,rData);}
	int GetCount()
		{return m_atyp.GetCount();}
	POSITION GetStart()
		{return m_atyp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ATYP_UDRD_D& rData)
		{m_atyp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ATYP_UDRD_D,T_ATYP_UDRD_D&>m_atyp;
};

#endif
