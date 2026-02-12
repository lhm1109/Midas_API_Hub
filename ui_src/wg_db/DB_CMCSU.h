#ifndef __DB_CMCSU_DB_H__
#define __DB_CMCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_CMCSU
{
public:
	CDB_CMCSU()
	{
		m_cmcs.InitHashTable(HASHSIZECMCS);
	}
	virtual ~CDB_CMCSU(){};

public:   
	void Add(T_UDRD_KEY Key,T_CMCS_UDRD_D& rData)
		{m_cmcs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cmcs.RemoveKey(Key);}
	void DelAll()
		{m_cmcs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CMCS_UDRD_D& rData)
		{return m_cmcs.Lookup(Key,rData);}
	int GetCount()
		{return m_cmcs.GetCount();}
	POSITION GetStart()
		{return m_cmcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CMCS_UDRD_D& rData)
		{m_cmcs.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CMCS_UDRD_D,T_CMCS_UDRD_D&>m_cmcs;
};

#endif
