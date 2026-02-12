#ifndef __DB_ANEV_DB_H__
#define __DB_ANEV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ANEV
{
public:
	CDB_ANEV();
	virtual ~CDB_ANEV();
	CDBDoc* m_pDoc;

public:
	T_ANEV_K m_nStartNum;
	T_ANEV_K m_nLastNum;

public:
	void Add(T_ANEV_K Key,T_ANEV_D& rData);
	BOOL Del(T_ANEV_K Key);

public:
	BOOL Get(T_ANEV_K Key,T_ANEV_D& rData);
	//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
	//{return m_ssps.GetCount();}
	POSITION GetStart();
	//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ANEV_K& rKey,T_ANEV_D& rData);
	//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_ANEV_K,T_ANEV_K,T_ANEV_D,T_ANEV_D&>m_anev;
};

#endif
