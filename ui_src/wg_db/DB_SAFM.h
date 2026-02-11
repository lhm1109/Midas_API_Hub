#pragma once
/************************************************************************/
/*      Seismic accounting for member (RC) : T_SAFM_D					*/
/************************************************************************/
class CDBDoc;
class CDB_ELEM;
class CDB_SAFM
{
public:
	CDB_SAFM();
	virtual ~CDB_SAFM();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_SAFM_K Key,T_SAFM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SAFM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SAFM_K Key,T_SAFM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SAFM_K& rKey, T_SAFM_D& rData);
protected:
	CMap<T_SAFM_K, T_SAFM_K, T_SAFM_D, T_SAFM_D&> m_safm;
};

