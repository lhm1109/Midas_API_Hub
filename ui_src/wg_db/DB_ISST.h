#pragma once

/************************************************************************/
/*      Isolation Specify Special Type :T_ISST_D						*/
/************************************************************************/
class CDBDoc;
class CDB_ISST
{
public:
	CDB_ISST();
	virtual ~CDB_ISST();
	CDBDoc* m_pDoc;

public:
	void Add(T_ISST_K Key, T_ISST_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_ISST_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_ISST_K Key, T_ISST_D& rData);
	//{return m_mbtp.Lookup(Key,rData);}
	int GetCount();
	//{return m_mbtp.GetCount();}
	POSITION GetStart();
	//{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_ISST_K& rKey, T_ISST_D& rData);
	//{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ISST_K, T_ISST_K, T_ISST_D, T_ISST_D&> m_dsst;
};

