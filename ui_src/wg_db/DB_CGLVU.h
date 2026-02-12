#ifndef __DB_CGLVU_DB_H__
#define __DB_CGLVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGLVU
{
public:
	CDB_CGLVU()
	{
		m_cglv.InitHashTable(HASHSIZECGLV);
	}
	virtual ~CDB_CGLVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGLV_UDRD_D& rData)
		{m_cglv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cglv.RemoveKey(Key);}
	void DelAll()
		{m_cglv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGLV_UDRD_D& rData)
		{return m_cglv.Lookup(Key,rData);}
	int GetCount()
		{return m_cglv.GetCount();}
	POSITION GetStart()
		{return m_cglv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGLV_UDRD_D& rData)
		{m_cglv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGLV_UDRD_D,T_CGLV_UDRD_D&>m_cglv;
};

#endif