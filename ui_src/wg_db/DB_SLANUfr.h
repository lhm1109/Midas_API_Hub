#ifndef __DB_SLANUfr_DB_H__
#define __DB_SLANUfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLANUfr
{
public:
	CDB_SLANUfr()
	{
		m_slanfr.InitHashTable(HASHSIZESLANfr);
	}
	virtual ~CDB_SLANUfr(){};

public:
	void Add(T_UDRD_KEY Key,T_SLANfr_UDRD_D& rData)
		{m_slanfr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slanfr.RemoveKey(Key);}
	void DelAll()
		{m_slanfr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLANfr_UDRD_D& rData)
		{return m_slanfr.Lookup(Key,rData);}
	int GetCount()
		{return m_slanfr.GetCount();}
	POSITION GetStart()
		{return m_slanfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLANfr_UDRD_D& rData)
		{m_slanfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLANfr_UDRD_D,T_SLANfr_UDRD_D&>m_slanfr;
};

#endif
