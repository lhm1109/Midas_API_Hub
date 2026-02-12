#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*   Modify nonlinear deformation model of concrete:T_MNDC_D			*/
/************************************************************************/
class CDBDoc;
class CDB_ELEM;
class CDB_MNDC
{
public:
	CDB_MNDC();
	virtual ~CDB_MNDC();
	CDBDoc* m_pDoc;

public:
	void Add(T_MNDC_K Key,T_MNDC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MNDC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MNDC_K Key,T_MNDC_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_MNDC_K& rKey, T_MNDC_D& rData);
protected:
	CMap<T_MNDC_K, T_MNDC_K, T_MNDC_D, T_MNDC_D&> m_mndc;
};

