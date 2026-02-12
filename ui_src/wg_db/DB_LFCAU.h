#ifndef __DB_LFCAU_DB_H__
#define __DB_LFCAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LFCAU
{
public:
	CDB_LFCAU()
	{
		m_lfca.InitHashTable(HASHSIZELFCA);
	}
	virtual ~CDB_LFCAU(){};

public:
	void Add(T_UDRD_KEY Key,T_LFCA_UDRD_D& rData)
		{m_lfca.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lfca.RemoveKey(Key);}
	void DelAll()
		{m_lfca.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LFCA_UDRD_D& rData)
		{return m_lfca.Lookup(Key,rData);}
	int GetCount()
		{return m_lfca.GetCount();}
	POSITION GetStart()
		{return m_lfca.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LFCA_UDRD_D& rData)
		{m_lfca.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LFCA_UDRD_D,T_LFCA_UDRD_D&>m_lfca;
};

#endif