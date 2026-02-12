#ifndef __DB_IPCRU_DB_H__
#define __DB_IPCRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_STLD.h"

class CDB_IPCRU
{
public:
	CDB_IPCRU()
	{
		m_ipcr.InitHashTable(HASHSIZEIPCR);
	}
	virtual ~CDB_IPCRU(){};

public:
	void Add(T_UDRD_KEY Key,T_IPCR_UDRD_D& rData)
	{m_ipcr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_ipcr.RemoveKey(Key);}
	void DelAll()
	{m_ipcr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IPCR_UDRD_D& rData)
	{return m_ipcr.Lookup(Key,rData);}
	int GetCount()
	{return m_ipcr.GetCount();}
	POSITION GetStart()
	{return m_ipcr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IPCR_UDRD_D& rData)
	{m_ipcr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_IPCR_UDRD_D,T_IPCR_UDRD_D&>m_ipcr;
};

#endif