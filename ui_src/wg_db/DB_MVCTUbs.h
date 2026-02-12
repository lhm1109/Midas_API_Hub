#ifndef __DB_MVCTUbs_DB_H__
#define __DB_MVCTUbs_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTUbs
{
public:
	CDB_MVCTUbs()
	{
		m_mvctbs.InitHashTable(HASHSIZEMVCTbs);
	}
	virtual ~CDB_MVCTUbs(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCTbs_UDRD_D& rData)
		{m_mvctbs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvctbs.RemoveKey(Key);}
	void DelAll()
		{m_mvctbs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCTbs_UDRD_D& rData)
		{return m_mvctbs.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCTbs_UDRD_D,T_MVCTbs_UDRD_D&>m_mvctbs;
};

#endif