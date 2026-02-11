#ifndef __DB_IEHGU_DB_H__
#define __DB_IEHGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_IEHGU
{
public:
	CDB_IEHGU()
	{
		m_iehg.InitHashTable(HASHSIZEIEHG);
	}
	virtual ~CDB_IEHGU(){};

public:   
	void Add(T_UDRD_KEY Key,T_IEHG_UDRD_D& rData)
		{m_iehg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_iehg.RemoveKey(Key);}
	void DelAll()
		{m_iehg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IEHG_UDRD_D& rData)
		{return m_iehg.Lookup(Key,rData);}
	int GetCount()
		{return m_iehg.GetCount();}
	POSITION GetStart()
		{return m_iehg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IEHG_UDRD_D& rData)
		{m_iehg.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IEHG_UDRD_D,T_IEHG_UDRD_D&>m_iehg;
};

#endif
