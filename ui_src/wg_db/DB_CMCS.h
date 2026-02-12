#ifndef __DB_CMCS_DB_H__
#define __DB_CMCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CMCS
{
public:
	CDB_CMCS();
	virtual ~CDB_CMCS();
	CDBDoc* m_pDoc;

public:
	void Add(T_CMCS_K Key,T_CMCS_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CMCS_K Key,CDB_NODE* pNode);

public:
	BOOL GetFromBaseDB(T_CMCS_K Key,T_CMCS_D& rData)
		{return m_cmcs.Lookup(Key,rData);}

	BOOL Get(T_CMCS_K Key,T_CMCS_D& rData);
		//{return m_cmcs.Lookup(Key,rData);}
	int GetCount();
		//{return m_cmcs.GetCount();}
	POSITION GetStart();
		//{return m_cmcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CMCS_K& rKey,T_CMCS_D& rData);
		//{m_cmcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CMCS_K,T_CMCS_K,T_CMCS_D,T_CMCS_D&>m_cmcs;
};

#endif