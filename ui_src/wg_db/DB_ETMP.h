#ifndef __DB_ETMP_DB_H__
#define __DB_ETMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;
class CDB_FLUE;
class CDB_STTF;
class CDB_FIPA;
class CDB_FSSF;
class CDB_MATL;


class CDB_ETMP
{
public:
	CDB_ETMP();
	virtual ~CDB_ETMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_ETMP_K Key,T_ETMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,
		CDB_FLUE* pFlue,CDB_STTF* pSttf,CDB_FIPA* pFipa,CDB_FSSF* pFssf,CDB_MATL* pMatl);
	BOOL Del(T_ETMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,
		CDB_FLUE* pFlue,CDB_STTF* pSttf,CDB_FIPA* pFipa,CDB_FSSF* pFssf,CDB_MATL* pMatl);
		
public:
	BOOL GetFromBaseDB(T_ETMP_K Key,T_ETMP_D& rData)
		{return m_etmp.Lookup(Key.keymap,rData);}

	BOOL Get(T_ETMP_K Key,T_ETMP_D& rData);
		//{return m_etmp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_etmp.GetCount();}
	POSITION GetStart();
		//{return m_etmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ETMP_K& rKey,T_ETMP_D& rData);
		//{m_etmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_ETMP_KEY,T_ETMP_KEY,T_ETMP_D,T_ETMP_D&>m_etmp;
};

#endif