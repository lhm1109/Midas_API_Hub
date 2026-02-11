#ifndef __DB_RESC_DB_H__
#define __DB_RESC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RESC
{
public:
	CDB_RESC();
	virtual ~CDB_RESC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RESC_K Key,T_RESC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RESC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RESC_K Key,T_RESC_D& rData);
		//{return m_resc.Lookup(Key,rData);}
	int GetCount();
		//{return m_resc.GetCount();}
	POSITION GetStart();
		//{return m_resc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RESC_K& rKey,T_RESC_D& rData);
		//{m_resc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RESC_K,T_RESC_K,T_RESC_D,T_RESC_D&>m_resc;
};

#endif

