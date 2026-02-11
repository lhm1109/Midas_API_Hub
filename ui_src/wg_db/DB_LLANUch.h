#ifndef __DB_LLANUch_DB_H__
#define __DB_LLANUch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUch
{
public:
	CDB_LLANUch()
	{
		m_llanch.InitHashTable(HASHSIZELLANch);
	}
	virtual ~CDB_LLANUch(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANch_UDRD_D& rData)
		{m_llanch.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llanch.RemoveKey(Key);}
	void DelAll()
		{m_llanch.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANch_UDRD_D& rData)
		{return m_llanch.Lookup(Key,rData);}
	int GetCount()
		{return m_llanch.GetCount();}
	POSITION GetStart()
		{return m_llanch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLANch_UDRD_D& rData)
		{m_llanch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANch_UDRD_D,T_LLANch_UDRD_D&>m_llanch;
};

#endif
