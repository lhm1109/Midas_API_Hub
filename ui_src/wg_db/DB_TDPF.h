#ifndef __DB_TDPF_DB_H__
#define __DB_TDPF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_TDPF
{
public:
	CDB_TDPF();
	virtual ~CDB_TDPF();

public:
	T_TDPF_K m_nStartNum;
	T_TDPF_K m_nLastNum;

public:
	void Add(T_TDPF_K Key, T_TDPF_D& rData);
	BOOL Del(T_TDPF_K Key);

public:
	BOOL Get(T_TDPF_K Key, T_TDPF_D& rData)
		{return m_tdpf.Lookup(Key,rData);}
	int GetCount()
		{return m_tdpf.GetCount();}
	POSITION GetStart()
		{return m_tdpf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_TDPF_K& rKey, T_TDPF_D& rData)
		{m_tdpf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDPF_K, T_TDPF_K, T_TDPF_D, T_TDPF_D&> m_tdpf;
};

#endif