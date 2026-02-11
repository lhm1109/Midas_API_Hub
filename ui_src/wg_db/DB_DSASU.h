#ifndef __DB_DSASU_DB_H__
#define __DB_DSASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DSASU
{
public:
	CDB_DSASU()
	{
		m_dsas.InitHashTable(HASHSIZEDSAS);
	}
	virtual ~CDB_DSASU(){};

public:
	void Add(T_UDRD_KEY Key,T_DSAS_UDRD_D& rData)
		{m_dsas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dsas.RemoveKey(Key);}
	void DelAll()
		{m_dsas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DSAS_UDRD_D& rData)
		{return m_dsas.Lookup(Key,rData);}
	int GetCount()
		{return m_dsas.GetCount();}
	POSITION GetStart()
		{return m_dsas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DSAS_UDRD_D& rData)
		{m_dsas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DSAS_UDRD_D,T_DSAS_UDRD_D&>m_dsas;
};

#endif