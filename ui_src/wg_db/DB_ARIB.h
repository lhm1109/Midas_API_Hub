#ifndef __DB_ARIB_DB_H__
#define __DB_ARIB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ARIB
{
public:
	CDB_ARIB();
	virtual ~CDB_ARIB();
	CDBDoc* m_pDoc;

public:
	void Add(T_ARIB_K Key,T_ARIB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_ARIB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_ARIB_K Key,T_ARIB_D& rData);
		//{return m_arib.Lookup(Key,rData);}
	int GetCount();
		//{return m_arib.GetCount();}
	POSITION GetStart();
		//{return m_arib.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ARIB_K& rKey,T_ARIB_D& rData);
		//{m_arib.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ARIB_K,T_ARIB_K,T_ARIB_D,T_ARIB_D&>m_arib;
};

#endif

