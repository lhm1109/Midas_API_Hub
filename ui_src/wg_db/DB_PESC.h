#ifndef __DB_PESC_DB_H__
#define __DB_PESC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PESC
{
public:
	CDB_PESC();
	virtual ~CDB_PESC();
	CDBDoc* m_pDoc;

public:
	void Add(T_PESC_K Key,T_PESC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PESC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PESC_K Key,T_PESC_D& rData);
		//{return m_pesc.Lookup(Key,rData);}
	int GetCount();
		//{return m_pesc.GetCount();}
	POSITION GetStart();
		//{return m_pesc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PESC_K& rKey,T_PESC_D& rData);
		//{m_pesc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PESC_K,T_PESC_K,T_PESC_D,T_PESC_D&>m_pesc;
};

#endif

