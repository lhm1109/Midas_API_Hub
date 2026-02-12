#ifndef __DB_SDDPU_DB_H__
#define __DB_SDDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDDPU
{
public:
	CDB_SDDPU()
	{
		m_sddp.InitHashTable(HASHSIZESDDP);
	}
	virtual ~CDB_SDDPU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDDP_UDRD_D& rData)
		{m_sddp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sddp.RemoveKey(Key);}
	void DelAll()
		{m_sddp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDDP_UDRD_D& rData)
		{return m_sddp.Lookup(Key,rData);}
	int GetCount()
		{return m_sddp.GetCount();}
	POSITION GetStart()
		{return m_sddp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDDP_UDRD_D& rData)
		{m_sddp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDDP_UDRD_D,T_SDDP_UDRD_D&>m_sddp;
};

#endif