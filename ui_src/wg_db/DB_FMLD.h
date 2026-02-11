#ifndef __DB_FMLD_DB_H__
#define __DB_FMLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_FMLD
{
public:
	CDB_FMLD();
	virtual ~CDB_FMLD();
	CDBDoc* m_pDoc;

public:
	void Add(T_FMLD_K Key,T_FMLD_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_FMLD_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_FMLD_K Key,T_FMLD_D& rData)
		{return m_fmld.Lookup(Key.keymap,rData);}

	BOOL Get(T_FMLD_K Key,T_FMLD_D& rData);
		//{return m_fmld.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_fmld.GetCount();}
	POSITION GetStart();
		//{return m_fmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FMLD_K& rKey,T_FMLD_D& rData);
		//{m_fmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_FMLD_KEY,T_FMLD_KEY,T_FMLD_D,T_FMLD_D&>m_fmld;
};

#endif