#ifndef __DB_PSSF_DB_H__
#define __DB_PSSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_PSSF
{
public:
	CDB_PSSF();
	virtual ~CDB_PSSF();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSSF_K Key,T_PSSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_PSSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_PSSF_K Key,T_PSSF_D& rData)
		{return m_pssf.Lookup(Key.keymap,rData);}
	
	BOOL Get(T_PSSF_K Key,T_PSSF_D& rData);
		//{return m_pssf.Lookup(Key,rData);}
	int GetCount();
		//{return m_pssf.GetCount();}
	POSITION GetStart();
		//{return m_pssf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSSF_K& rKey,T_PSSF_D& rData);
		//{m_pssf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PSSF_KEY,T_PSSF_KEY,T_PSSF_D,T_PSSF_D&>m_pssf;
};

#endif