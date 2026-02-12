#ifndef __DB_PCDCU_DB_H__
#define __DB_PCDCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PCDCU
{
public:
	CDB_PCDCU()
	{
		m_pcdc.InitHashTable(HASHSIZEPCDC);
	}
	virtual ~CDB_PCDCU(){};

public:
	void Add(T_UDRD_KEY Key,T_PCDC_UDRD_D& rData)
		{m_pcdc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pcdc.RemoveKey(Key);}
	void DelAll()
		{m_pcdc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PCDC_UDRD_D& rData)
		{return m_pcdc.Lookup(Key,rData);}
	int GetCount()
		{return m_pcdc.GetCount();}
	POSITION GetStart()
		{return m_pcdc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PCDC_UDRD_D& rData)
		{m_pcdc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PCDC_UDRD_D,T_PCDC_UDRD_D&>m_pcdc;
};

#endif