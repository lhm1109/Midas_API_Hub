#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*      ReinForce Check Total Info :T_RCTI_D                            */
/************************************************************************/
class CDBDoc;
class CDB_RCTI
{
public:
	CDB_RCTI();
	virtual ~CDB_RCTI();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCTI_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCTI_D& rData);
	//{return m_mbtp.Lookup(Key,rData);}
	int GetCount();
	//{return m_mbtp.GetCount();}
	POSITION GetStart();
	//{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_RCTI_K& rKey, T_RCTI_D& rData);
	//{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RCTI_K, T_RCTI_K, T_RCTI_D, T_RCTI_D&> m_rcti;
};

