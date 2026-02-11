#ifndef __DB_SUPL_DB_H__
#define __DB_SUPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SUPL
{
public:
	CDB_SUPL();
	virtual ~CDB_SUPL();
	CDBDoc* m_pDoc;

public:
	T_SUPL_K m_nStartNum;
	T_SUPL_K m_nLastNum;

public:
	void Add(T_SUPL_K Key,T_SUPL_D& rData);
	BOOL Del(T_SUPL_K Key);

public:
	BOOL Get(T_SUPL_K Key,T_SUPL_D& rData);
	//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
	//{return m_ssps.GetCount();}
	POSITION GetStart();
	//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SUPL_K& rKey,T_SUPL_D& rData);
	//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_SUPL_K,T_SUPL_K,T_SUPL_D,T_SUPL_D&>m_supl;
};

#endif
