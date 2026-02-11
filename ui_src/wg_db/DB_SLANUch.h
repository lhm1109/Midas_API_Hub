#ifndef __DB_SLANUch_DB_H__
#define __DB_SLANUch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLANUch
{
public:
	CDB_SLANUch()
	{
		m_slanch.InitHashTable(HASHSIZESLANch);
	}
	virtual ~CDB_SLANUch(){};

public:
	void Add(T_UDRD_KEY Key,T_SLANch_UDRD_D& rData)
		{m_slanch.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slanch.RemoveKey(Key);}
	void DelAll()
		{m_slanch.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLANch_UDRD_D& rData)
		{return m_slanch.Lookup(Key,rData);}
	int GetCount()
		{return m_slanch.GetCount();}
	POSITION GetStart()
		{return m_slanch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLANch_UDRD_D& rData)
		{m_slanch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLANch_UDRD_D,T_SLANch_UDRD_D&>m_slanch;
};

#endif
