#ifndef __DB_CCPT_DB_H__
#define __DB_CCPT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_CCPT
{
public:
	CDB_CCPT();
	virtual ~CDB_CCPT();

public:
	T_CCPT_K m_nStartNum;
	T_CCPT_K m_nLastNum;

public:
	void Add(T_CCPT_K Key, T_CCPT_D& rData);
	BOOL Del(T_CCPT_K Key);

public:
	BOOL Get(T_CCPT_K Key,T_CCPT_D& rData)
		{return m_ccpt.Lookup(Key,rData);}
	int GetCount()
		{return m_ccpt.GetCount();}
	POSITION GetStart()
		{return m_ccpt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CCPT_K& rKey,T_CCPT_D& rData)
		{m_ccpt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CCPT_K,T_CCPT_K,T_CCPT_D,T_CCPT_D&>m_ccpt;
};

#endif