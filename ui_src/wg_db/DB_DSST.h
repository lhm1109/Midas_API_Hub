#pragma once

/************************************************************************/
/*      Damp Specify Special Type :T_DSST_D                            */
/************************************************************************/
class CDBDoc;
class CDB_DSST
{
public:
	CDB_DSST();
	virtual ~CDB_DSST();
	CDBDoc* m_pDoc;

public:
	void Add(T_DSST_K Key, T_DSST_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_DSST_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_DSST_K Key, T_DSST_D& rData);
	//{return m_mbtp.Lookup(Key,rData);}
	int GetCount();
	//{return m_mbtp.GetCount();}
	POSITION GetStart();
	//{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_DSST_K& rKey, T_DSST_D& rData);
	//{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DSST_K, T_DSST_K, T_DSST_D, T_DSST_D&> m_dsst;
};

