#ifndef __DB_CRST_DB_H__
#define __DB_CRST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CRST
{
public:
	CDB_CRST();
	virtual ~CDB_CRST();
	CDBDoc* m_pDoc;

public:
	void Add(T_CRST_K Key,T_CRST_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CRST_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CRST_K Key,T_CRST_D& rData);
		//{return m_crst.Lookup(Key,rData);}
	int GetCount();
		//{return m_crst.GetCount();}
	POSITION GetStart();
		//{return m_crst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CRST_K& rKey,T_CRST_D& rData);
		//{m_crst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CRST_K,T_CRST_K,T_CRST_D,T_CRST_D&>m_crst;
};

#endif

