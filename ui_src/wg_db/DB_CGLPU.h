#ifndef __DB_CGLPU_DB_H__
#define __DB_CGLPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGLPU
{
public:
	CDB_CGLPU()
	{
		m_cglp.InitHashTable(HASHSIZECGLP);
	}
	virtual ~CDB_CGLPU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGLP_UDRD_D& rData)
		{m_cglp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cglp.RemoveKey(Key);}
	void DelAll()
		{m_cglp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGLP_UDRD_D& rData)
		{return m_cglp.Lookup(Key,rData);}
	int GetCount()
		{return m_cglp.GetCount();}
	POSITION GetStart()
		{return m_cglp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGLP_UDRD_D& rData)
		{m_cglp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGLP_UDRD_D,T_CGLP_UDRD_D&>m_cglp;
};

#endif