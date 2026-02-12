#ifndef __DB_PMDM_DB_H__
#define __DB_PMDM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PMDM
{
public:
	CDB_PMDM();
	virtual ~CDB_PMDM();
	CDBDoc* m_pDoc;

public:
	void Add(T_PMDM_K Key,T_PMDM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PMDM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PMDM_K Key,T_PMDM_D& rData);
		//{return m_pmdm.Lookup(Key,rData);}
	int GetCount();
		//{return m_pmdm.GetCount();}
	POSITION GetStart();
		//{return m_pmdm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PMDM_K& rKey,T_PMDM_D& rData);
		//{m_pmdm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PMDM_K,T_PMDM_K,T_PMDM_D,T_PMDM_D&> m_pmdm;
};

#endif

