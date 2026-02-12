#ifndef __DB_IPFOU_DB_H__
#define __DB_IPFOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IPFOU
{
public:
	CDB_IPFOU()
	{
		m_ipfo.InitHashTable(HASHSIZEIPFO);
	}
	virtual ~CDB_IPFOU(){};

public:
	void Add(T_UDRD_KEY Key,T_IPFO_UDRD_D& rData)
		{m_ipfo.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ipfo.RemoveKey(Key);}
	void DelAll()
		{m_ipfo.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IPFO_UDRD_D& rData)
		{return m_ipfo.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IPFO_UDRD_D,T_IPFO_UDRD_D&>m_ipfo;
};

#endif