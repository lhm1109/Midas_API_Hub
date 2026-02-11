#ifndef __DB_STDGU_DB_H__
#define __DB_STDGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_STDGU
{
public:
	CDB_STDGU()
	{
		m_stdg.InitHashTable(HASHSIZESTDG);
	}
	virtual ~CDB_STDGU(){};

public:   
	void Add(T_UDRD_KEY Key,T_STDG_UDRD_D& rData)
		{m_stdg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stdg.RemoveKey(Key);}
	void DelAll()
		{m_stdg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STDG_UDRD_D& rData)
		{return m_stdg.Lookup(Key,rData);}
	int GetCount()
		{return m_stdg.GetCount();}
	POSITION GetStart()
		{return m_stdg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STDG_UDRD_D& rData)
		{m_stdg.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STDG_UDRD_D,T_STDG_UDRD_D&>m_stdg;
};

#endif
