#ifndef __DB_DRLSU_DB_H__
#define __DB_DRLSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_DRLSU
{
public:
	CDB_DRLSU()
	{
		m_drls.InitHashTable(HASHSIZEDRLS);
	}
	virtual ~CDB_DRLSU(){};

public:   
	void Add(T_UDRD_KEY Key,T_DRLS_UDRD_D& rData)
		{m_drls.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_drls.RemoveKey(Key);}
	void DelAll()
		{m_drls.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DRLS_UDRD_D& rData)
		{return m_drls.Lookup(Key,rData);}
	int GetCount()
		{return m_drls.GetCount();}
	POSITION GetStart()
		{return m_drls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DRLS_UDRD_D& rData)
		{m_drls.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DRLS_UDRD_D,T_DRLS_UDRD_D&>m_drls;
};

#endif
