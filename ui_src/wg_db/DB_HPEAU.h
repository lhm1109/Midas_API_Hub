#ifndef __DB_HPEAU_DB_H__
#define __DB_HPEAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HPEAU
{
public:
	CDB_HPEAU()
	{
		m_hpea.InitHashTable(HASHSIZEHPEA);
	}
	virtual ~CDB_HPEAU(){};

public:
	void Add(T_UDRD_KEY Key,T_HPEA_UDRD_D& rData)
		{m_hpea.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hpea.RemoveKey(Key);}
	void DelAll()
		{m_hpea.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HPEA_UDRD_D& rData)
		{return m_hpea.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_HPEA_UDRD_D,T_HPEA_UDRD_D&>m_hpea;
};

#endif