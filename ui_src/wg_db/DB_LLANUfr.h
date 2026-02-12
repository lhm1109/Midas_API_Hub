#ifndef __DB_LLANUfr_DB_H__
#define __DB_LLANUfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUfr
{
public:
	CDB_LLANUfr()
	{
		m_llanfr.InitHashTable(HASHSIZELLANfr);
	}
	virtual ~CDB_LLANUfr(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANfr_UDRD_D& rData)
		{m_llanfr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llanfr.RemoveKey(Key);}
	void DelAll()
		{m_llanfr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANfr_UDRD_D& rData)
		{return m_llanfr.Lookup(Key,rData);}
	int GetCount()
		{return m_llanfr.GetCount();}
	POSITION GetStart()
		{return m_llanfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLANfr_UDRD_D& rData)
		{m_llanfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANfr_UDRD_D,T_LLANfr_UDRD_D&>m_llanfr;
};

#endif
