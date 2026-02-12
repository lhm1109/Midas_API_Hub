#ifndef __DB_TSGR_DB_H__
#define __DB_TSGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_TSGR
{
public:
	CDB_TSGR();
	virtual ~CDB_TSGR();
	CDBDoc* m_pDoc;

public:
	T_TSGR_K m_nStartNum;
	T_TSGR_K m_nLastNum;

public:
	void Add(T_TSGR_K Key,T_TSGR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_TSGR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_TSGR_K Key,T_TSGR_D& rData);
		//{return m_tsgr.Lookup(Key,rData);}
	int GetCount();
		//{return m_tsgr.GetCount();}
	POSITION GetStart();
		//{return m_tsgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TSGR_K& rKey,T_TSGR_D& rData);
		//{m_tsgr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TSGR_K,T_TSGR_K,T_TSGR_D,T_TSGR_D&>m_tsgr;
};

#endif
