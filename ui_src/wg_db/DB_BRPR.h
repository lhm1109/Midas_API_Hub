#ifndef __DB_BRPR_DB_H__
#define __DB_BRPR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_BRPR
{
public:
	CDB_BRPR();
	virtual ~CDB_BRPR();
	CDBDoc* m_pDoc;

public:
	T_BRPR_K m_nStartNum;
	T_BRPR_K m_nLastNum;

public:
	void Add(T_BRPR_K Key,T_BRPR_D& rData);
	BOOL Del(T_BRPR_K Key);

public:
	BOOL Get(T_BRPR_K Key,T_BRPR_D& rData);
	//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
	//{return m_ssps.GetCount();}
	POSITION GetStart();
	//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BRPR_K& rKey,T_BRPR_D& rData);
	//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_BRPR_K,T_BRPR_K,T_BRPR_D,T_BRPR_D&>m_brpr;
};

#endif
