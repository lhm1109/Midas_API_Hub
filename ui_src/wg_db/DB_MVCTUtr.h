#ifndef __DB_MVCTUtr_DB_H__
#define __DB_MVCTUtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTUtr
{
public:
	CDB_MVCTUtr()
	{
		m_mvcttr.InitHashTable(HASHSIZEMVCTtr);
	}
	virtual ~CDB_MVCTUtr(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCTtr_UDRD_D& rData)
		{m_mvcttr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvcttr.RemoveKey(Key);}
	void DelAll()
		{m_mvcttr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCTtr_UDRD_D& rData)
		{return m_mvcttr.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCTtr_UDRD_D,T_MVCTtr_UDRD_D&>m_mvcttr;
};

#endif