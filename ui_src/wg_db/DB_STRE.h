#ifndef __DB_STRE_DB_H__
#define __DB_STRE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STRE
{
public:
	CDB_STRE();
	virtual ~CDB_STRE();
	CDBDoc* m_pDoc;

public:
	void Add(T_STRE_K Key,T_STRE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_STRE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_STRE_K Key,T_STRE_D& rData);
		//{return m_stre.Lookup(Key,rData);}
	int GetCount();
		//{return m_stre.GetCount();}
	POSITION GetStart();
		//{return m_stre.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STRE_K& rKey,T_STRE_D& rData);
		//{m_stre.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STRE_K,T_STRE_K,T_STRE_D,T_STRE_D&>m_stre;
};

#endif

