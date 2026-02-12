#ifndef __DB_PEBA_DB_H__
#define __DB_PEBA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_PEBA
{
public:
	CDB_PEBA();
	virtual ~CDB_PEBA();

public:
	T_PEBA_K m_nStartNum;
	T_PEBA_K m_nLastNum;

public:
	void Add(T_PEBA_K Key, T_PEBA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_PEBA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_PEBA_K Key,T_PEBA_D& rData)
		{return m_peba.Lookup(Key,rData);}
	int GetCount()
		{return m_peba.GetCount();}
	POSITION GetStart()
		{return m_peba.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PEBA_K& rKey,T_PEBA_D& rData)
		{m_peba.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PEBA_K,T_PEBA_K,T_PEBA_D,T_PEBA_D&>m_peba;
};

#endif