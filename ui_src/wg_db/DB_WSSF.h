#ifndef __DB_WSSF_DB_H__
#define __DB_WSSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_WSSF
{
public:
	CDB_WSSF();
	virtual ~CDB_WSSF();
	CDBDoc* m_pDoc;

public:
	void Add(T_WSSF_K Key,T_WSSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_WSSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_WSSF_K Key,T_WSSF_D& rData)
		{return m_wssf.Lookup(Key.keymap,rData);}
	
	BOOL Get(T_WSSF_K Key,T_WSSF_D& rData);
		//{return m_wssf.Lookup(Key,rData);}
	int GetCount();
		//{return m_wssf.GetCount();}
	POSITION GetStart();
		//{return m_wssf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WSSF_K& rKey,T_WSSF_D& rData);
		//{m_wssf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_WSSF_KEY,T_WSSF_KEY,T_WSSF_D,T_WSSF_D&>m_wssf;
};

#endif