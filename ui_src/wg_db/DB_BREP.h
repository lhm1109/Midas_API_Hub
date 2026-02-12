#ifndef __DB_BREP_DB_H__
#define __DB_BREP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_BREP
{
public:
	CDB_BREP();
	virtual ~CDB_BREP();
	CDBDoc* m_pDoc;

public:
	T_BREP_K m_nStartNum;
	T_BREP_K m_nLastNum;

public:
	void Add(T_BREP_K Key, T_BREP_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_BREP_K Key, CDB_ELEM* pElem);
	T_BREP_K FindByName(LPCTSTR sName) const;
public:
	BOOL Get(T_BREP_K Key, T_BREP_D& rData);
	//{return m_brep.Lookup(Key,rData);}
	int GetCount();
	//{return m_brep.GetCount();}
	POSITION GetStart();
	//{return m_brep.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_BREP_K& rKey, T_BREP_D& rData);
	//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetBrepAssigned(T_ELEM_K KeyElem, T_BREP_K& rKeyBrep);   // element가 속한 breper의 key를 찾아준다.
	//{return m_elemtobrep.Lookup(KeyElem, rKeyBrep);}
protected:
	CMap<T_BREP_K, T_BREP_K, T_BREP_D, T_BREP_D&> m_brep;
	CMap<T_ELEM_K, T_ELEM_K, T_BREP_K, T_BREP_K>  m_elemtobrep;
	CMap<CString, LPCTSTR, T_BREP_K, T_BREP_K> m_name2brep;
};

#endif