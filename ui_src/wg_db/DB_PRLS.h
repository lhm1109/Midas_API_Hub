#ifndef __DB_PRLS_DB_H__
#define __DB_PRLS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_PRLS
{
public:
	CDB_PRLS();
	virtual ~CDB_PRLS();
	CDBDoc* m_pDoc;

public:
	void Add(T_PRLS_K Key,T_PRLS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_PRLS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_PRLS_K Key,T_PRLS_D& rData)
		{return m_prls.Lookup(Key.keymap,rData);}

	BOOL Get(T_PRLS_K Key,T_PRLS_D& rData);
		//{return m_prls.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_prls.GetCount();}
	POSITION GetStart();
		//{return m_prls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PRLS_K& rKey,T_PRLS_D& rData);
		//{m_prls.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PRLS_KEY,T_PRLS_KEY,T_PRLS_D,T_PRLS_D&>m_prls;
};

#endif