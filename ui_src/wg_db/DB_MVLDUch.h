#ifndef __DB_MVLDUch_DB_H__
#define __DB_MVLDUch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUch
{
public:
	CDB_MVLDUch()
	{
		m_mvldch.InitHashTable(HASHSIZEMVLDch);
	}
	virtual ~CDB_MVLDUch(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDch_UDRD_D& rData)
		{m_mvldch.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldch.RemoveKey(Key);}
	void DelAll()
		{m_mvldch.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDch_UDRD_D& rData)
		{return m_mvldch.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldch.GetCount();}
	POSITION GetStart()
		{return m_mvldch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDch_UDRD_D& rData)
		{m_mvldch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDch_UDRD_D,T_MVLDch_UDRD_D&>m_mvldch;
};

#endif