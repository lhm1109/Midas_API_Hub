#ifndef __DB_MEMB_DB_H__
#define __DB_MEMB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MEMB
{
public:
	CDB_MEMB();
	virtual ~CDB_MEMB();
	CDBDoc* m_pDoc;

public:
	T_MEMB_K m_nStartNum;
	T_MEMB_K m_nLastNum;

public:
	void Add(T_MEMB_K Key,T_MEMB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MEMB_K Key,CDB_ELEM* pElem);
	T_MEMB_K FindByName(LPCTSTR sName) const;
public:
	BOOL Get(T_MEMB_K Key,T_MEMB_D& rData);
		//{return m_memb.Lookup(Key,rData);}
	int GetCount();
		//{return m_memb.GetCount();}
	POSITION GetStart();
		//{return m_memb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MEMB_K& rKey,T_MEMB_D& rData);
		//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetMembAssigned(T_ELEM_K KeyElem, T_MEMB_K& rKeyMemb);   // element가 속한 member의 key를 찾아준다.
		//{return m_elemtomemb.Lookup(KeyElem, rKeyMemb);}
protected:
	CMap<T_MEMB_K,T_MEMB_K,T_MEMB_D,T_MEMB_D&> m_memb;
	CMap<T_ELEM_K,T_ELEM_K,T_MEMB_K,T_MEMB_K>  m_elemtomemb;
	CMap<CString, LPCTSTR, T_MEMB_K, T_MEMB_K> m_name2memb;
};

#endif

