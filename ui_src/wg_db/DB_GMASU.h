#ifndef __DB_GMASU_DB_H__
#define __DB_GMASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_GMASU
{
public:
	CDB_GMASU()
	{
		m_gmas.InitHashTable(HASHSIZEGMAS);
	}
	virtual ~CDB_GMASU(){};

public:   
	void Add(T_UDRD_KEY Key,T_GMAS_UDRD_D& rData)
		{m_gmas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gmas.RemoveKey(Key);}
	void DelAll()
		{m_gmas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GMAS_UDRD_D& rData)
		{return m_gmas.Lookup(Key,rData);}
	int GetCount()
		{return m_gmas.GetCount();}
	POSITION GetStart()
		{return m_gmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GMAS_UDRD_D& rData)
		{m_gmas.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GMAS_UDRD_D,T_GMAS_UDRD_D&>m_gmas;
};

#endif
