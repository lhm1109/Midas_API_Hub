#ifndef __DB_FRLS_DB_H__
#define __DB_FRLS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_FRLS
{
public:
	CDB_FRLS();
	virtual ~CDB_FRLS();
	CDBDoc* m_pDoc;

public:
	void Add(T_FRLS_K Key,T_FRLS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_FRLS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_FRLS_K Key,T_FRLS_D& rData)
		{return m_frls.Lookup(Key.keymap,rData);}

	BOOL Get(T_FRLS_K Key,T_FRLS_D& rData);
		//{return m_frls.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_frls.GetCount();}
	POSITION GetStart();
		//{return m_frls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FRLS_K& rKey,T_FRLS_D& rData);
		//{m_frls.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_FRLS_KEY,T_FRLS_KEY,T_FRLS_D,T_FRLS_D&>m_frls;
};

#endif