#ifndef __DB_DGMLU_DB_H__
#define __DB_DGMLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DGMLU
{
public:
	CDB_DGMLU()
	{
		m_dgml.InitHashTable(HASHSIZEDGML);
	}
	virtual ~CDB_DGMLU(){};

public:
	void Add(T_UDRD_KEY Key,T_DGML_UDRD_D& rData)
		{m_dgml.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dgml.RemoveKey(Key);}
	void DelAll()
		{m_dgml.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DGML_UDRD_D& rData)
		{return m_dgml.Lookup(Key,rData);}
	int GetCount()
		{return m_dgml.GetCount();}
	POSITION GetStart()
		{return m_dgml.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DGML_UDRD_D& rData)
		{m_dgml.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DGML_UDRD_D,T_DGML_UDRD_D&>m_dgml;
};

#endif
