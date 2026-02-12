#ifndef __DB_CGDPU_DB_H__
#define __DB_CGDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGDPU
{
public:
	CDB_CGDPU()
	{
		m_cgdp.InitHashTable(HASHSIZECGDP);
	}
	virtual ~CDB_CGDPU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGDP_UDRD_D& rData)
		{m_cgdp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgdp.RemoveKey(Key);}
	void DelAll()
		{m_cgdp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGDP_UDRD_D& rData)
		{return m_cgdp.Lookup(Key,rData);}
	int GetCount()
		{return m_cgdp.GetCount();}
	POSITION GetStart()
		{return m_cgdp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGDP_UDRD_D& rData)
		{m_cgdp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGDP_UDRD_D,T_CGDP_UDRD_D&>m_cgdp;
};

#endif