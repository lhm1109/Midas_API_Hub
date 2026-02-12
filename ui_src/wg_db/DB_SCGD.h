#ifndef __DB_SCGD_DB_H__
#define __DB_SCGD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SCGD
{
public:
	CDB_SCGD();
	virtual ~CDB_SCGD();
	CDBDoc* m_pDoc;

public:
	T_SCGD_K m_nStartNum;
	T_SCGD_K m_nLastNum;

public:
	void Add(T_SCGD_K Key,T_SCGD_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SCGD_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SCGD_K Key,T_SCGD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SCGD_K& rKey,T_SCGD_D& rData);
	BOOL GetScgdAssigned(T_ELEM_K KeyElem, T_SCGD_K& rKeyMemb);   // element가 속한  Shear Connector Group의 key를 찾아준다.
protected:
	CMap<T_SCGD_K,T_SCGD_K,T_SCGD_D,T_SCGD_D&> m_scgd;
	CMap<T_ELEM_K,T_ELEM_K,T_SCGD_K,T_SCGD_K>  m_elemToShearConnGroup;
};

#endif

