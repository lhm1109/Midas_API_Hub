#ifndef __DB_MVLDUbs_DB_H__
#define __DB_MVLDUbs_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDUbs
{
public:
	CDB_MVLDUbs()
	{
		m_mvldbs.InitHashTable(HASHSIZEMVLDbs);
	}
	virtual ~CDB_MVLDUbs(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLDbs_UDRD_D& rData)
		{m_mvldbs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvldbs.RemoveKey(Key);}
	void DelAll()
		{m_mvldbs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLDbs_UDRD_D& rData)
		{return m_mvldbs.Lookup(Key,rData);}
	int GetCount()
		{return m_mvldbs.GetCount();}
	POSITION GetStart()
		{return m_mvldbs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLDbs_UDRD_D& rData)
		{m_mvldbs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLDbs_UDRD_D,T_MVLDbs_UDRD_D&>m_mvldbs;
};

#endif