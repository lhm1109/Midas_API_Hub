#ifndef __DB_SLANUop_DB_H__
#define __DB_SLANUop_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLANUop
{
public:
	CDB_SLANUop()
	{
		m_slanop.InitHashTable(HASHSIZESLANop);
	}
	virtual ~CDB_SLANUop(){};

public:
	void Add(T_UDRD_KEY Key,T_SLANop_UDRD_D& rData)
		{m_slanop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slanop.RemoveKey(Key);}
	void DelAll()
		{m_slanop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLANop_UDRD_D& rData)
		{return m_slanop.Lookup(Key,rData);}
	int GetCount()
		{return m_slanop.GetCount();}
	POSITION GetStart()
		{return m_slanop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLANop_UDRD_D& rData)
		{m_slanop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLANop_UDRD_D,T_SLANop_UDRD_D&>m_slanop;
};

#endif
