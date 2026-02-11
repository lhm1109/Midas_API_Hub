#ifndef __DB_AHTR_DB_H__
#define __DB_AHTR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_AHTR
{
public:
	CDB_AHTR();
	virtual ~CDB_AHTR();
	CDBDoc* m_pDoc;

public:
	void Add(T_AHTR_K Key,T_AHTR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_AHTR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_AHTR_K Key,T_AHTR_D& rData);
		//{return m_leng.Lookup(Key,rData);}
	int GetCount();
		//{return m_leng.GetCount();}
	POSITION GetStart();
		//{return m_leng.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_AHTR_K& rKey,T_AHTR_D& rData);
		//{m_leng.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_AHTR_K,T_AHTR_K,T_AHTR_D,T_AHTR_D&>m_ahtr;
};

#endif

