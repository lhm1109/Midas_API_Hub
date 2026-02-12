#ifndef __DB_DCBR_DB_H__
#define __DB_DCBR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DCBR
{
public:
	CDB_DCBR();
	virtual ~CDB_DCBR();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCBR_K Key,T_DCBR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DCBR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DCBR_K Key,T_DCBR_D& rData);
		//{return m_dcbr.Lookup(Key,rData);}
	int GetCount();
		//{return m_dcbr.GetCount();}
	POSITION GetStart();
		//{return m_dcbr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DCBR_K& rKey,T_DCBR_D& rData);
		//{m_dcbr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DCBR_K,T_DCBR_K,T_DCBR_D,T_DCBR_D&>m_dcbr;
};

#endif

