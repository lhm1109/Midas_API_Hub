#ifndef __DB_TDPFU_DB_H__
#define __DB_TDPFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_TDPFU
{
public:
	CDB_TDPFU()
	{
		m_tdpf.InitHashTable(HASHSIZETDPF);
	}
	virtual ~CDB_TDPFU(){};

public:
	void Add(T_UDRD_KEY Key, T_TDPF_UDRD_D& rData)
		{m_tdpf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdpf.RemoveKey(Key);}
	void DelAll()
		{m_tdpf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key, T_TDPF_UDRD_D& rData)
		{return m_tdpf.Lookup(Key,rData);}
	int GetCount()
		{return m_tdpf.GetCount();}
	POSITION GetStart()
		{return m_tdpf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_TDPF_UDRD_D& rData)
		{m_tdpf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_TDPF_UDRD_D, T_TDPF_UDRD_D&> m_tdpf;
};

#endif