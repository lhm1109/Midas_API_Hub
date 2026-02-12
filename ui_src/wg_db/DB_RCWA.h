#ifndef __DB_RCWA_DB_H__
#define __DB_RCWA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RCWA
{
public:
	CDB_RCWA();
	virtual ~CDB_RCWA();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCWA_K Key,T_RCWA_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RCWA_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RCWA_K Key,T_RCWA_D& rData);
		//{return m_rcwa.Lookup(Key,rData);}
	int GetCount();
		//{return m_rcwa.GetCount();}
	POSITION GetStart();
		//{return m_rcwa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RCWA_K& rKey,T_RCWA_D& rData);
		//{m_rcwa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RCWA_K,T_RCWA_K,T_RCWA_D,T_RCWA_D&>m_rcwa;
};

#endif

