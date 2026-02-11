#ifndef __DB_MLSP_DB_H__
#define __DB_MLSP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MLSP
{
public:
	CDB_MLSP();
	virtual ~CDB_MLSP();
	CDBDoc* m_pDoc;

public:
	void Add(T_MLSP_K Key,T_MLSP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MLSP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MLSP_K Key,T_MLSP_D& rData);
		//{return m_mlsp.Lookup(Key,rData);}
	int GetCount();
		//{return m_mlsp.GetCount();}
	POSITION GetStart();
		//{return m_mlsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MLSP_K& rKey,T_MLSP_D& rData);
		//{m_mlsp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MLSP_KEY,T_MLSP_KEY,T_MLSP_D,T_MLSP_D&>m_mlsp;
};

#endif