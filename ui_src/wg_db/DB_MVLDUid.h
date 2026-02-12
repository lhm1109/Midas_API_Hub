#ifndef __DB_MVLDUid_DB_H__
#define __DB_MVLDUid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUid
{
public:
	CDB_MVLDUid()
	{
		m_mvldid.InitHashTable(HASHSIZEMVLDid);
	}
	virtual ~CDB_MVLDUid(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDid_UDRD_D& rData)
		{m_mvldid.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldid.RemoveKey(Key);}
	void DelAll()
		{m_mvldid.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDid_UDRD_D& rData)
		{return m_mvldid.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldid.GetCount();}
	POSITION GetStart()
		{return m_mvldid.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDid_UDRD_D& rData)
		{m_mvldid.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDid_UDRD_D,T_MVLDid_UDRD_D&>m_mvldid;
};

#endif