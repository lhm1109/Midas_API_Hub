#ifndef __DB_CPTF_DB_H__
#define __DB_CPTF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPTF
{
public:
	CDB_CPTF();
	virtual ~CDB_CPTF();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPTF_K Key,T_CPTF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPTF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPTF_K Key,T_CPTF_D& rData);
		//{return m_cptf.Lookup(Key,rData);}
	int GetCount();
		//{return m_cptf.GetCount();}
	POSITION GetStart();
		//{return m_cptf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPTF_K& rKey,T_CPTF_D& rData);
		//{m_cptf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPTF_K,T_CPTF_K,T_CPTF_D,T_CPTF_D&>m_cptf;
};

#endif

