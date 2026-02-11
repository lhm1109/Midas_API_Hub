#ifndef __DB_SUFC_DB_H__
#define __DB_SUFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SUFC
{
public:
	CDB_SUFC();
	virtual ~CDB_SUFC();
	CDBDoc* m_pDoc;

public:
	void Add(T_SUFC_K Key,T_SUFC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SUFC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SUFC_K Key,T_SUFC_D& rData);
		//{return m_sufc.Lookup(Key,rData);}
	int GetCount();
		//{return m_sufc.GetCount();}
	POSITION GetStart();
		//{return m_sufc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SUFC_K& rKey,T_SUFC_D& rData);
		//{m_sufc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SUFC_K,T_SUFC_K,T_SUFC_D,T_SUFC_D&>m_sufc;
};

#endif

