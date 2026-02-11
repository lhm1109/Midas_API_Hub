#ifndef __DB_PILE_DB_H__
#define __DB_PILE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PILE
{
public:
	CDB_PILE();
	virtual ~CDB_PILE();

public:
	T_PILE_K m_nStartNum;
	T_PILE_K m_nLastNum;

public:
	void Add(T_PILE_K Key,T_PILE_D& rData);
	BOOL Del(T_PILE_K Key);

public:
	BOOL Get(T_PILE_K Key,T_PILE_D& rData)
		{return m_pile.Lookup(Key,rData);}
	int GetCount()
		{return m_pile.GetCount();}
	POSITION GetStart()
		{return m_pile.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PILE_K& rKey,T_PILE_D& rData)
		{m_pile.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PILE_K,T_PILE_K,T_PILE_D,T_PILE_D&>m_pile;
};

#endif