#ifndef __DB_PSLLU_DB_H__
#define __DB_PSLLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSLLU
{
public:
	CDB_PSLLU()
	{
		m_psll.InitHashTable(HASHSIZEPSLL);
	}
	virtual ~CDB_PSLLU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSLL_UDRD_D& rData)
		{m_psll.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_psll.RemoveKey(Key);}
	void DelAll()
		{m_psll.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSLL_UDRD_D& rData)
		{return m_psll.Lookup(Key,rData);}
	int GetCount()
		{return m_psll.GetCount();}
	POSITION GetStart()
		{return m_psll.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSLL_UDRD_D& rData)
		{m_psll.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSLL_UDRD_D,T_PSLL_UDRD_D&>m_psll;
};

#endif