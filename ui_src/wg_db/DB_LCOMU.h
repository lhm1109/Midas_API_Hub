#ifndef __DB_LCOMU_DB_H__
#define __DB_LCOMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCOMU
{
public:
	CDB_LCOMU()
	{
		m_lcom.InitHashTable(HASHSIZELCOM);
	}
	virtual ~CDB_LCOMU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCOM_UDRD_D& rData)
		{m_lcom.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lcom.RemoveKey(Key);}
	void DelAll()
		{m_lcom.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCOM_UDRD_D& rData)
		{return m_lcom.Lookup(Key,rData);}
	int GetCount()
		{return m_lcom.GetCount();}
	POSITION GetStart()
		{return m_lcom.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LCOM_UDRD_D& rData)
		{m_lcom.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCOM_UDRD_D,T_LCOM_UDRD_D&>m_lcom;
};

#endif