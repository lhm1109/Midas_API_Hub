#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*      Initial deflection:T_INDF_D			                            */
/************************************************************************/
class CDBDoc;
class CDB_ELEM;
class CDB_INDF
{
public:
	CDB_INDF();
	virtual ~CDB_INDF();
	CDBDoc* m_pDoc;

public:
	void Add(T_INDF_K Key,T_INDF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_INDF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_INDF_K Key,T_INDF_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_INDF_K& rKey, T_INDF_D& rData);
protected:
	CMap<T_INDF_K, T_INDF_K, T_INDF_D, T_INDF_D&> m_indf;
};

