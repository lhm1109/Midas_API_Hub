#ifndef __DB_RISSU_DB_H__
#define __DB_RISSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CDB_RISSU
{
public:
	CDB_RISSU()
	{
		m_RISS.InitHashTable(HASHSIZERISS);
	}
	virtual ~CDB_RISSU(){};

public:
	void Add(T_UDRD_KEY Key,T_RISS_UDRD_D& rData)
	{m_RISS.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_RISS.RemoveKey(Key);}
	void DelAll()
	{m_RISS.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RISS_UDRD_D& rData)
	{return m_RISS.Lookup(Key,rData);}
	int GetCount()
	{return m_RISS.GetCount();}
	POSITION GetStart()
	{return m_RISS.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RISS_UDRD_D& rData)
	{m_RISS.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RISS_UDRD_D,T_RISS_UDRD_D&>m_RISS;
};

#endif
