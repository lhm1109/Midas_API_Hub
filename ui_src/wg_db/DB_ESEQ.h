#ifndef __DB_ESEQ_DB_H__
#define __DB_ESEQ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_STOR;
class CDB_MATL;

class CDB_ESEQ
{
public:
	CDB_ESEQ();
	virtual ~CDB_ESEQ();
	CDBDoc* m_pDoc;

public:
	void Add(T_ESEQ_D& rData,CDB_STLD* pStld);
	BOOL Del(CDB_STLD* pStld);

public:
	BOOL GetFromBaseDB(T_ESEQ_D& rData)
		{return m_eseq.Lookup(1,rData);}
	BOOL Get(T_ESEQ_D& rData);
		//{return m_eseq.Lookup(1,rData);}
	int GetCount();
		//{return m_eseq.GetCount();}

protected:
	CMap<T_ESEQ_K,T_ESEQ_K,T_ESEQ_D,T_ESEQ_D&>m_eseq;
};

#endif