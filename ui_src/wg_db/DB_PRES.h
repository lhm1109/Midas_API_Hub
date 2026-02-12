#ifndef __DB_PRES_DB_H__
#define __DB_PRES_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;
class CDB_PSLT;

class CDB_PRES
{
public:
	CDB_PRES();
	virtual ~CDB_PRES();
	CDBDoc* m_pDoc;

public:
	void Add(T_PRES_K Key,T_PRES_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,CDB_PSLT* pPslt=NULL);
	BOOL Del(T_PRES_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,CDB_PSLT* pPslt=NULL);

public:
	BOOL GetFromBaseDB(T_PRES_K Key,T_PRES_D& rData)
		{return m_pres.Lookup(Key.keymap,rData);}

	BOOL Get(T_PRES_K Key,T_PRES_D& rData);
		//{return m_pres.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_pres.GetCount();}
	POSITION GetStart();
		//{return m_pres.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PRES_K& rKey,T_PRES_D& rData);
		//{m_pres.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PRES_KEY,T_PRES_KEY,T_PRES_D,T_PRES_D&>m_pres;
};

#endif