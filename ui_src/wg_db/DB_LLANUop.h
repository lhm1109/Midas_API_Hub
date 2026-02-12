#ifndef __DB_LLANUop_DB_H__
#define __DB_LLANUop_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUop
{
public:
	CDB_LLANUop()
	{
		m_llanop.InitHashTable(HASHSIZELLANop);
	}
	virtual ~CDB_LLANUop(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANop_UDRD_D& rData)
		{m_llanop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llanop.RemoveKey(Key);}
	void DelAll()
		{m_llanop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANop_UDRD_D& rData)
		{return m_llanop.Lookup(Key,rData);}
	int GetCount()
		{return m_llanop.GetCount();}
	POSITION GetStart()
		{return m_llanop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLANop_UDRD_D& rData)
		{m_llanop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANop_UDRD_D,T_LLANop_UDRD_D&>m_llanop;
};

#endif
