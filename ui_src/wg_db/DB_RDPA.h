#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*      ReinForce Design Parameter:T_RDPA_D                             */
/************************************************************************/
class CDBDoc;
class CDB_RDPA
{
public:
	CDB_RDPA();
	virtual ~CDB_RDPA();
	CDBDoc* m_pDoc;

public:
	void Add(T_RDPA_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RDPA_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RDPA_K& rKey, T_RDPA_D& rData);

protected:
	CMap<T_RDPA_K, T_RDPA_K, T_RDPA_D, T_RDPA_D&> m_rdpa;
};

