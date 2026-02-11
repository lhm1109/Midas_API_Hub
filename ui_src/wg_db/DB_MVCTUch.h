#ifndef __DB_MVCTUch_DB_H__
#define __DB_MVCTUch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTUch
{
public:
	CDB_MVCTUch()
	{
		m_mvctch.InitHashTable(HASHSIZEMVCTch);
	}
	virtual ~CDB_MVCTUch(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCTch_UDRD_D& rData)
		{m_mvctch.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvctch.RemoveKey(Key);}
	void DelAll()
		{m_mvctch.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCTch_UDRD_D& rData)
		{return m_mvctch.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCTch_UDRD_D,T_MVCTch_UDRD_D&>m_mvctch;
};

#endif