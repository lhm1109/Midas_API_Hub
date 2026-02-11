#ifndef __DB_LLANUid_DB_H__
#define __DB_LLANUid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUid
{
public:
	CDB_LLANUid()
	{
		m_llanid.InitHashTable(HASHSIZELLANid);
	}
	virtual ~CDB_LLANUid(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANid_UDRD_D& rData)
		{m_llanid.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llanid.RemoveKey(Key);}
	void DelAll()
		{m_llanid.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANid_UDRD_D& rData)
		{return m_llanid.Lookup(Key,rData);}
	int GetCount()
		{return m_llanid.GetCount();}
	POSITION GetStart()
		{return m_llanid.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLANid_UDRD_D& rData)
		{m_llanid.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANid_UDRD_D,T_LLANid_UDRD_D&>m_llanid;
};

#endif
