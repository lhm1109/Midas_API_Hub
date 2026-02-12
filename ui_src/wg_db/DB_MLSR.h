#ifndef __DB_MLSR_DB_H__
#define __DB_MLSR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_MLSR
{
public:
	CDB_MLSR();
	virtual ~CDB_MLSR();
	CDBDoc* m_pDoc;

public:
	void Add(T_MLSR_K Key,T_MLSR_D& rData,CDB_NODE* pNode);
	BOOL Del(T_MLSR_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_MLSR_K Key,T_MLSR_D& rData);
		//{return m_mlsr.Lookup(Key,rData);}
	int GetCount();
		//{return m_mlsr.GetCount();}
	POSITION GetStart();
		//{return m_mlsr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MLSR_K& rKey,T_MLSR_D& rData);
		//{m_mlsr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MLSR_K,T_MLSR_K,T_MLSR_D,T_MLSR_D&>m_mlsr;
};

#endif