#ifndef __DB_MVLDUfr_DB_H__
#define __DB_MVLDUfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUfr
{
public:
	CDB_MVLDUfr()
	{
		m_mvldfr.InitHashTable(HASHSIZEMVLDfr);
	}
	virtual ~CDB_MVLDUfr(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDfr_UDRD_D& rData)
		{m_mvldfr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldfr.RemoveKey(Key);}
	void DelAll()
		{m_mvldfr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDfr_UDRD_D& rData)
		{return m_mvldfr.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldfr.GetCount();}
	POSITION GetStart()
		{return m_mvldfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDfr_UDRD_D& rData)
		{m_mvldfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDfr_UDRD_D,T_MVLDfr_UDRD_D&>m_mvldfr;
};

#endif