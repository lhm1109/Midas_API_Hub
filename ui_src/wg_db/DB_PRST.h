#ifndef __DB_PRST_DB_H__
#define __DB_PRST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_PRST
{
public:
	CDB_PRST();
	virtual ~CDB_PRST();
	CDBDoc* m_pDoc;

public:
	void Add(T_PRST_K Key,T_PRST_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_PRST_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_PRST_K Key,T_PRST_D& rData)
		{return m_prst.Lookup(Key.keymap,rData);}

	BOOL Get(T_PRST_K Key,T_PRST_D& rData);
		//{return m_prst.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_prst.GetCount();}
	POSITION GetStart();
		//{return m_prst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PRST_K& rKey,T_PRST_D& rData);
		//{m_prst.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PRST_KEY,T_PRST_KEY,T_PRST_D,T_PRST_D&>m_prst;
};

#endif