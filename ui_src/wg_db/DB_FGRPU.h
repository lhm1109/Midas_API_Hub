#ifndef __DB_FGRPU_DB_H__
#define __DB_FGRPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FGRPU
{
public:
	CDB_FGRPU()
	{
		m_fgrp.InitHashTable(HASHSIZEFGRP);
	}
	virtual ~CDB_FGRPU(){};
	
public:
	void Add(T_UDRD_KEY Key,T_FGRP_UDRD_D& rData)
	{m_fgrp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_fgrp.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_FGRP_UDRD_D& rData)
	{return m_fgrp.Lookup(Key,rData);}
	int GetCount()
	{return m_fgrp.GetCount();}
	POSITION GetStart()
	{return m_fgrp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FGRP_UDRD_D& rData)
	{m_fgrp.GetNextAssoc(rNextPosition,rKey,rData);}
	
	void DelAll()
	{
		m_fgrp.RemoveAll();
	}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_FGRP_UDRD_D,T_FGRP_UDRD_D&>m_fgrp;
};

#endif
