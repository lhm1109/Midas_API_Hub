#ifndef __DB_TDNT_DB_H__
#define __DB_TDNT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDNT
{
public:
	CDB_TDNT();
	virtual ~CDB_TDNT();

public:
	T_TDNT_K m_nStartNum;
	T_TDNT_K m_nLastNum;

public:
	void Add(T_TDNT_K Key,T_TDNT_D& rData);
	BOOL Del(T_TDNT_K Key);

public:
	BOOL Get(T_TDNT_K Key,T_TDNT_D& rData)
		{return m_tdnt.Lookup(Key,rData);}
	int GetCount()
		{return m_tdnt.GetCount();}
	POSITION GetStart()
		{return m_tdnt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDNT_K& rKey,T_TDNT_D& rData)
		{m_tdnt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDNT_K,T_TDNT_K,T_TDNT_D,T_TDNT_D&>m_tdnt;
};

#endif
