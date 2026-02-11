#ifndef __DB_RCCO_DB_H__
#define __DB_RCCO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RCCO
{
public:
	CDB_RCCO();
	virtual ~CDB_RCCO();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCCO_K Key,T_RCCO_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RCCO_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RCCO_K Key,T_RCCO_D& rData);
		//{return m_rcco.Lookup(Key,rData);}
	int GetCount();
		//{return m_rcco.GetCount();}
	POSITION GetStart();
		//{return m_rcco.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RCCO_K& rKey,T_RCCO_D& rData);
		//{m_rcco.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RCCO_K,T_RCCO_K,T_RCCO_D,T_RCCO_D&>m_rcco;
};

#endif

