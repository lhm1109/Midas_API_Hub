#ifndef __DB_TDMF_DB_H__
#define __DB_TDMF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDMF
{
public:
	CDB_TDMF();
	CDB_TDMF(CDB_TDMF& src);
	CDB_TDMF& operator=(CDB_TDMF& src);
	virtual ~CDB_TDMF();

public:
	T_TDMF_K m_nStartNum;
	T_TDMF_K m_nLastNum;

public:
	void Add(T_TDMF_K Key,T_TDMF_D& rData);
	BOOL Del(T_TDMF_K Key);

public:
	BOOL Get(T_TDMF_K Key,T_TDMF_D& rData)
		{return m_tdmf.Lookup(Key,rData);}
	int GetCount()
		{return m_tdmf.GetCount();}
	POSITION GetStart()
		{return m_tdmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDMF_K& rKey,T_TDMF_D& rData)
		{m_tdmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDMF_K,T_TDMF_K,T_TDMF_D,T_TDMF_D&>m_tdmf;
};

#endif
