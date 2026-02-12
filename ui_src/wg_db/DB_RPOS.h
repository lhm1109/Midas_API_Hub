#ifndef __DB_RPOS_DB_H__
#define __DB_RPOS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;

class CDB_RPOS
{
public:
	CDB_RPOS();
	virtual ~CDB_RPOS();

public:
	T_RPOS_K m_nStartNum;
	T_RPOS_K m_nLastNum;

public:
	void Add(T_RPOS_K Key,T_RPOS_D& rData,CDB_NODE* m_pNode);
	BOOL Del(T_RPOS_K Key,CDB_NODE* m_pNode);

public:
	BOOL Get(T_RPOS_K Key,T_RPOS_D& rData)
		{return m_rpos.Lookup(Key,rData);}
	int GetCount()
		{return m_rpos.GetCount();}
	POSITION GetStart()
		{return m_rpos.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPOS_K& rKey,T_RPOS_D& rData)
		{m_rpos.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPOS_K,T_RPOS_K,T_RPOS_D,T_RPOS_D&>m_rpos;
};

#endif
