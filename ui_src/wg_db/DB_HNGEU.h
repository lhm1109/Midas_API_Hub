#ifndef __DB_HNGEU_DB_H__
#define __DB_HNGEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_HNGEU
{
public:
	CDB_HNGEU()
	{
		m_hnge.InitHashTable(HASHSIZEHNGE);
	}
	virtual ~CDB_HNGEU(){};

public:   
	void Add(T_UDRD_KEY Key,T_HNGE_UDRD_D& rData)
		{m_hnge.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hnge.RemoveKey(Key);}
	void DelAll()
		{m_hnge.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HNGE_UDRD_D& rData)
		{return m_hnge.Lookup(Key,rData);}
	int GetCount()
		{return m_hnge.GetCount();}
	POSITION GetStart()
		{return m_hnge.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HNGE_UDRD_D& rData)
		{m_hnge.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HNGE_UDRD_D,T_HNGE_UDRD_D&>m_hnge;
};

#endif
