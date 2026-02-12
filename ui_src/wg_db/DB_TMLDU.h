#ifndef __DB_TMLDU_DB_H__
#define __DB_TMLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_TMLDU
{
public:
	CDB_TMLDU()
	{
		m_tmld.InitHashTable(HASHSIZETMLD);
	}
	virtual ~CDB_TMLDU(){};

public:   
	void Add(T_UDRD_KEY Key,T_TMLD_UDRD_D& rData)
		{m_tmld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tmld.RemoveKey(Key);}
	void DelAll()
		{m_tmld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TMLD_UDRD_D& rData)
		{return m_tmld.Lookup(Key,rData);}
	int GetCount()
		{return m_tmld.GetCount();}
	POSITION GetStart()
		{return m_tmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TMLD_UDRD_D& rData)
		{m_tmld.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TMLD_UDRD_D,T_TMLD_UDRD_D&>m_tmld;
};

#endif
