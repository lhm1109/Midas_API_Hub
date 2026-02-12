#ifndef __DB_SPHI_DB_H__
#define __DB_SPHI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPHI
{
public:
	CDB_SPHI();
	virtual ~CDB_SPHI();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPHI_K Key,T_SPHI_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SPHI_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SPHI_K Key,T_SPHI_D& rData);
		//{return m_SPHI.Lookup(Key,rData);}
	int GetCount();
		//{return m_SPHI.GetCount();}
	POSITION GetStart();
		//{return m_SPHI.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPHI_K& rKey,T_SPHI_D& rData);
		//{m_SPHI.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPHI_K,T_SPHI_K,T_SPHI_D,T_SPHI_D&>m_SPHI;
};

#endif

