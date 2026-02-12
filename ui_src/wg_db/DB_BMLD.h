#ifndef __DB_BMLD_DB_H__
#define __DB_BMLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_BMLD
{
public:
	CDB_BMLD();
	virtual ~CDB_BMLD();
	CDBDoc* m_pDoc;

public:
	void Add(T_BMLD_K Key,T_BMLD_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_BMLD_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_BMLD_K Key,T_BMLD_D& rData)
		{return m_bmld.Lookup(Key.keymap,rData);}

	BOOL Get(T_BMLD_K Key,T_BMLD_D& rData);
		//{return m_bmld.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_bmld.GetCount();}
	POSITION GetStart();
		//{return m_bmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BMLD_K& rKey,T_BMLD_D& rData);
		//{m_bmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_BMLD_KEY,T_BMLD_KEY,T_BMLD_D,T_BMLD_D&>m_bmld;
};

#endif