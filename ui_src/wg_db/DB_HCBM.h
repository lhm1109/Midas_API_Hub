#ifndef __DB_HCBM_DB_H__
#define __DB_HCBM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_HCBM
{
public:
	CDB_HCBM();
	virtual ~CDB_HCBM();
	CDBDoc* m_pDoc;

public:
	T_HCBM_K m_nStartNum;
	T_HCBM_K m_nLastNum;

public:
	void Add(T_HCBM_K Key,T_HCBM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_HCBM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_HCBM_K Key,T_HCBM_D& rData);
		//{return m_hcbm.Lookup(Key,rData);}
	int GetCount();
		//{return m_hcbm.GetCount();}
	POSITION GetStart();
		//{return m_hcbm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HCBM_K& rKey,T_HCBM_D& rData);
		//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetHcbmAssigned(T_ELEM_K KeyElem, T_HCBM_K& rKeyHcbm);   // element가 속한 Haunched Beam의 key를 찾아준다.
		//{return m_elemtohcbm.Lookup(KeyElem, rKeyHcbm);}
protected:
	CMap<T_HCBM_K,T_HCBM_K,T_HCBM_D,T_HCBM_D&> m_hcbm;
	CMap<T_ELEM_K,T_ELEM_K,T_HCBM_K,T_HCBM_K>  m_elemtohcbm;
};

#endif

