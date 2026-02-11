#ifndef __DB_ASGBU_DB_H__
#define __DB_ASGBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ASGBU
{
public:
	CDB_ASGBU()
	{
		m_nllp.InitHashTable(HASHSIZEASGB);
	}
	virtual ~CDB_ASGBU(){};

public:
	void Add(T_UDRD_KEY Key,T_ASGB_UDRD_D& rData)
		{m_nllp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nllp.RemoveKey(Key);}
	void DelAll()
		{m_nllp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ASGB_UDRD_D& rData)
		{return m_nllp.Lookup(Key,rData);}
	int GetCount()
		{return m_nllp.GetCount();}
	POSITION GetStart()
		{return m_nllp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ASGB_UDRD_D& rData)
		{m_nllp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ASGB_UDRD_D,T_ASGB_UDRD_D&>m_nllp;
};

#endif