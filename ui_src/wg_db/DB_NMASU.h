#ifndef __DB_NMASU_DB_H__
#define __DB_NMASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_NMASU
{
public:
	CDB_NMASU()
	{
		m_nmas.InitHashTable(HASHSIZENMAS);
	}
	virtual ~CDB_NMASU(){};

public:   
	void Add(T_UDRD_KEY Key,T_NMAS_UDRD_D& rData)
		{m_nmas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nmas.RemoveKey(Key);}
	void DelAll()
		{m_nmas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NMAS_UDRD_D& rData)
		{return m_nmas.Lookup(Key,rData);}
	int GetCount()
		{return m_nmas.GetCount();}
	POSITION GetStart()
		{return m_nmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NMAS_UDRD_D& rData)
		{m_nmas.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NMAS_UDRD_D,T_NMAS_UDRD_D&>m_nmas;
};

#endif
