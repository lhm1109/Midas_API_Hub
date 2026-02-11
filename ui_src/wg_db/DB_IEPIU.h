#ifndef __DB_IEPIU_DB_H__
#define __DB_IEPIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_IEPIU
{
public:
	CDB_IEPIU()
	{
		m_iepi.InitHashTable(HASHSIZEIEPI);
	}
	virtual ~CDB_IEPIU(){};

public:   
	void Add(T_UDRD_KEY Key,T_IEPI_UDRD_D& rData)
		{m_iepi.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_iepi.RemoveKey(Key);}
	void DelAll()
		{m_iepi.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IEPI_UDRD_D& rData)
		{return m_iepi.Lookup(Key,rData);}
	int GetCount()
		{return m_iepi.GetCount();}
	POSITION GetStart()
		{return m_iepi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IEPI_UDRD_D& rData)
		{m_iepi.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IEPI_UDRD_D,T_IEPI_UDRD_D&>m_iepi;
};

#endif
