#ifndef __DB_STDG_DB_H__
#define __DB_STDG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STOR;
class CDB_BNGR;
class CDBDoc;

class CDB_STDG
{
public:
	CDB_STDG();
	virtual ~CDB_STDG();
	CDBDoc* m_pDoc;

public:
	void Add(T_STDG_K Key,T_STDG_D& rData,CDB_STOR* pStor,CDB_BNGR* pBngr);
	BOOL Del(T_STDG_K Key,CDB_STOR* pStor,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_STDG_K Key,T_STDG_D& rData){return m_stdg.Lookup(Key,rData);}

	BOOL Get(T_STDG_K Key,T_STDG_D& rData);
	//  {return m_stdg.Lookup(Key,rData);}
	int GetCount();
	//  {return m_stdg.GetCount();}
	POSITION GetStart();
	//  {return m_stdg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STDG_K& rKey,T_STDG_D& rData);
	//  {m_stdg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STDG_K,T_STDG_K,T_STDG_D,T_STDG_D&>m_stdg;
};

#endif