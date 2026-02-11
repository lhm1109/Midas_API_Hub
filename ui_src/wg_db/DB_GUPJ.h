#ifndef __DB_GUPJ_DB_H__
#define __DB_GUPJ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PJCF;
class CDB_GUPJ
{
public:
	CDB_GUPJ();
	virtual ~CDB_GUPJ();

public:
	T_GUPJ_K m_nStartNum;
	T_GUPJ_K m_nLastNum;

public:
	void Add(T_GUPJ_K Key, T_GUPJ_D& rData, CDB_PJCF* pPjcf);
	BOOL Del(T_GUPJ_K Key, CDB_PJCF* pPjcf);

public:
	BOOL Get(T_GUPJ_K Key, T_GUPJ_D& rData)
	{return m_gupj.Lookup(Key,rData);}
	int GetCount()
	{return m_gupj.GetCount();}
	POSITION GetStart()
	{return m_gupj.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUPJ_K& rKey, T_GUPJ_D& rData)
	{m_gupj.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUPJ_K,T_GUPJ_K,T_GUPJ_D,T_GUPJ_D&>m_gupj;
};

#endif