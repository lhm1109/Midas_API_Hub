#ifndef __DB_PFCM_DB_H__
#define __DB_PFCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PFCM
{
public:
	CDB_PFCM();
	virtual ~CDB_PFCM();
	CDBDoc* m_pDoc;

public:
	void Add(T_PFCM_K Key,T_PFCM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PFCM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PFCM_K Key,T_PFCM_D& rData);
		//{return m_PFCM.Lookup(Key,rData);}
	int GetCount();
		//{return m_PFCM.GetCount();}
	POSITION GetStart();
		//{return m_PFCM.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFCM_K& rKey,T_PFCM_D& rData);
		//{m_PFCM.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PFCM_K,T_PFCM_K,T_PFCM_D,T_PFCM_D&>m_PFCM;
};

#endif

