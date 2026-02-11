#ifndef __DB_CGLP_DB_H__
#define __DB_CGLP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NLLP;
class CDB_BNGR;

class CDB_CGLP
{
public:
	CDB_CGLP();
	virtual ~CDB_CGLP();
	CDBDoc* m_pDoc;

public:
	T_CGLP_K m_nStartNum;
	T_CGLP_K m_nLastNum;

public:
	void Add(T_CGLP_K Key,T_CGLP_D& rData,CDB_NLLP* pNllp, CDB_BNGR* pBngr);
	BOOL Del(T_CGLP_K Key,CDB_NLLP* pNllp, CDB_BNGR* pBngr);

public:
	BOOL Get(T_CGLP_K Key,T_CGLP_D& rData);
		//{return m_cglp.Lookup(Key,rData);}
	int GetCount();
		//{return m_cglp.GetCount();}
	POSITION GetStart();
		//{return m_cglp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGLP_K& rKey,T_CGLP_D& rData);
		//{m_cglp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGLP_K,T_CGLP_K,T_CGLP_D,T_CGLP_D&>m_cglp;
};

#endif