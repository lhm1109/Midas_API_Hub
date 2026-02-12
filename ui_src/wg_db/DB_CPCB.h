#ifndef __DB_CPCB_DB_H__
#define __DB_CPCB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPCB
{
public:
	CDB_CPCB();
	virtual ~CDB_CPCB();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPCB_K Key,T_CPCB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPCB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPCB_K Key,T_CPCB_D& rData);
		//{return m_cpcb.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpcb.GetCount();}
	POSITION GetStart();
		//{return m_cpcb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPCB_K& rKey,T_CPCB_D& rData);
		//{m_cpcb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPCB_K,T_CPCB_K,T_CPCB_D,T_CPCB_D&>m_cpcb;
};

#endif

