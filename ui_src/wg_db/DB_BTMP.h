#ifndef __DB_BTMP_DB_H__
#define __DB_BTMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_BTMP
{
public:
	CDB_BTMP();
	virtual ~CDB_BTMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_BTMP_K Key,T_BTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_BTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_BTMP_K Key,T_BTMP_D& rData)
		{return m_btmp.Lookup(Key.keymap,rData);}

	BOOL Get(T_BTMP_K Key,T_BTMP_D& rData);
		//{return m_btmp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_btmp.GetCount();}
	POSITION GetStart();
		//{return m_btmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BTMP_K& rKey,T_BTMP_D& rData);
		//{m_btmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_BTMP_KEY,T_BTMP_KEY,T_BTMP_D,T_BTMP_D&>m_btmp;
};

#endif