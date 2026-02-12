#ifndef __DB_APHI_DB_H__
#define __DB_APHI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_APHI
{
public:
	CDB_APHI();
	virtual ~CDB_APHI();
	CDBDoc* m_pDoc;

public:
	void Add(T_APHI_K Key,T_APHI_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_APHI_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_APHI_K Key,T_APHI_D& rData);
		//{return m_aphi.Lookup(Key,rData);}
	int GetCount();
		//{return m_aphi.GetCount();}
	POSITION GetStart();
		//{return m_aphi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_APHI_K& rKey,T_APHI_D& rData);
		//{m_aphi.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_APHI_K,T_APHI_K,T_APHI_D,T_APHI_D&>m_aphi;
};

#endif

