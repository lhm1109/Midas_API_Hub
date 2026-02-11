#ifndef __DB_BURB_DB_H__
#define __DB_BURB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BURB
{
public:
	CDB_BURB();
	virtual ~CDB_BURB();
	CDBDoc* m_pDoc;

public:
	T_BURB_K m_nStartNum;
	T_BURB_K m_nLastNum;

public:
	void Add(T_BURB_K Key,T_BURB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_BURB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_BURB_K Key,T_BURB_D& rData);
		//{return m_burb.Lookup(Key,rData);}
	int GetCount();
		//{return m_burb.GetCount();}
	POSITION GetStart();
		//{return m_burb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BURB_K& rKey,T_BURB_D& rData);
		//{m_burb.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetRcplAssigned(T_ELEM_K KeyElem, T_BURB_K& rKeyRcpl);   // element가 속한 Rcpl 의 key를 찾아준다.
		//{return m_elemtoburb.Lookup(KeyElem, rKeyRcpl);}
protected:
	CMap<T_BURB_K,T_BURB_K,T_BURB_D,T_BURB_D&>m_burb;
	CMap<T_ELEM_K,T_ELEM_K,T_BURB_K,T_BURB_K>  m_elemtoburb;
};

#endif

