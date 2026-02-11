#ifndef __DB_MVLDUjp_DB_H__
#define __DB_MVLDUjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUjp
{
public:
	CDB_MVLDUjp()
	{
		m_mvldjp.InitHashTable(HASHSIZEMVLDjp);
	}
	virtual ~CDB_MVLDUjp(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDjp_UDRD_D& rData)
		{m_mvldjp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldjp.RemoveKey(Key);}
	void DelAll()
		{m_mvldjp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDjp_UDRD_D& rData)
		{return m_mvldjp.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldjp.GetCount();}
	POSITION GetStart()
		{return m_mvldjp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDjp_UDRD_D& rData)
		{m_mvldjp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDjp_UDRD_D,T_MVLDjp_UDRD_D&>m_mvldjp;
};

#endif