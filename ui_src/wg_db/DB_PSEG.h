#ifndef __DB_PSEG_DB_H__
#define __DB_PSEG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PSEG
{
public:
	CDB_PSEG();
	virtual ~CDB_PSEG();
	CDBDoc* m_pDoc;

public:
	T_PSEG_K m_nStartNum;
	T_PSEG_K m_nLastNum;

public:
	void Add(T_PSEG_K Key,T_PSEG_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PSEG_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PSEG_K Key,T_PSEG_D& rData);
		//{return m_pseg.Lookup(Key,rData);}
	int GetCount();
		//{return m_pseg.GetCount();}
	POSITION GetStart();
		//{return m_pseg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSEG_K& rKey,T_PSEG_D& rData);
		//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetPsegAssigned(T_ELEM_K KeyElem, T_PSEG_K& rKeyPseg);   // element가 속한 pseger의 key를 찾아준다.
		//{return m_elemtopseg.Lookup(KeyElem, rKeyPseg);}
protected:
	CMap<T_PSEG_K,T_PSEG_K,T_PSEG_D,T_PSEG_D&> m_pseg;
	CMap<T_ELEM_K,T_ELEM_K,T_PSEG_K,T_PSEG_K>  m_elemtopseg;
};

#endif

