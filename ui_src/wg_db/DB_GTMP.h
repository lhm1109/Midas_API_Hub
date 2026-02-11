#ifndef __DB_GTMP_DB_H__
#define __DB_GTMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_GTMP
{
public:
	CDB_GTMP();
	virtual ~CDB_GTMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_GTMP_K Key,T_GTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_GTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_GTMP_K Key,T_GTMP_D& rData)
		{return m_gtmp.Lookup(Key.keymap,rData);}

	BOOL Get(T_GTMP_K Key,T_GTMP_D& rData);
		//{return m_gtmp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_gtmp.GetCount();}
	POSITION GetStart();
		//{return m_gtmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GTMP_K& rKey,T_GTMP_D& rData);
		//{m_gtmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_GTMP_KEY,T_GTMP_KEY,T_GTMP_D,T_GTMP_D&>m_gtmp;
};

#endif