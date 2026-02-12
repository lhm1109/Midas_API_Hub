#ifndef __DB_SECPU_DB_H__
#define __DB_SECPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECPU
{
public:
	CDB_SECPU()
	{
		m_secp.InitHashTable(HASHSIZESECP);
	}
	virtual ~CDB_SECPU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECP_UDRD_D& rData)
		{m_secp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_secp.RemoveKey(Key);}
	void DelAll()
		{m_secp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECP_UDRD_D& rData)
		{return m_secp.Lookup(Key,rData);}
	int GetCount()
		{return m_secp.GetCount();}
	POSITION GetStart()
		{return m_secp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECP_UDRD_D& rData)
		{m_secp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECP_UDRD_D,T_SECP_UDRD_D&>m_secp;
};

#endif