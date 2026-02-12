#ifndef __DB_RSDM_DB_H__
#define __DB_RSDM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_ELNK;
class CDB_NLNK;

class CDB_RSDM
{
public:
	CDB_RSDM();
	virtual ~CDB_RSDM();
	CDBDoc* m_pDoc;

public:
	void Add(T_RSDM_K Key,T_RSDM_D& rData, CDB_ELEM* pElem, CDB_ELNK* pElnk, CDB_NLNK* pNlnk);
	BOOL Del(T_RSDM_K Key, CDB_ELEM* pElem, CDB_ELNK* pElnk, CDB_NLNK* pNlnk);

public:
	BOOL Get(T_RSDM_K Key,T_RSDM_D& rData);
		//{return m_rsdm.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_rsdm.GetCount();}
	POSITION GetStart();
		//{return m_rsdm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RSDM_K& rKey,T_RSDM_D& rData);
		//{m_rsdm.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_RSDM_KEY,T_RSDM_KEY,T_RSDM_D,T_RSDM_D&>m_rsdm;
};

#endif

