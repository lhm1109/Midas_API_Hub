#ifndef __DB_TMLD_DB_H__
#define __DB_TMLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_LDGR;

class CDB_TMLD
{
public:
	CDB_TMLD();
	virtual ~CDB_TMLD();
	CDBDoc* m_pDoc;

public:
	void Add(T_TMLD_K Key,T_TMLD_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_TMLD_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_TMLD_K Key,T_TMLD_D& rData)
		{return m_tmld.Lookup(Key.keymap,rData);}

	BOOL Get(T_TMLD_K Key,T_TMLD_D& rData);
		//{return m_tmld.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_tmld.GetCount();}
	POSITION GetStart();
		//{return m_tmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TMLD_K& rKey,T_TMLD_D& rData);
		//{m_tmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_TMLD_KEY,T_TMLD_KEY,T_TMLD_D,T_TMLD_D&>m_tmld;
};

#endif