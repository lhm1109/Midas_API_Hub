#ifndef __DB_MVLDtrUtr_DB_H__
#define __DB_MVLDtrUtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUtr
{
public:
	CDB_MVLDUtr()
	{
		m_mvldtr.InitHashTable(HASHSIZEMVLDtr);
	}
	virtual ~CDB_MVLDUtr(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDtr_UDRD_D& rData)
		{m_mvldtr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldtr.RemoveKey(Key);}
	void DelAll()
		{m_mvldtr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDtr_UDRD_D& rData)
		{return m_mvldtr.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldtr.GetCount();}
	POSITION GetStart()
		{return m_mvldtr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDtr_UDRD_D& rData)
		{m_mvldtr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDtr_UDRD_D,T_MVLDtr_UDRD_D&>m_mvldtr;
};

#endif