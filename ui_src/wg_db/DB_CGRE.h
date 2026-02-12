#ifndef __DB_CGRE_DB_H__
#define __DB_CGRE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGRE
{
public:
	CDB_CGRE();
	virtual ~CDB_CGRE();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGRE_K Key,T_CGRE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGRE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGRE_K Key,T_CGRE_D& rData);
		//{return m_cgre.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgre.GetCount();}
	POSITION GetStart();
		//{return m_cgre.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGRE_K& rKey,T_CGRE_D& rData);
		//{m_cgre.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGRE_K,T_CGRE_K,T_CGRE_D,T_CGRE_D&>m_cgre;
};

#endif

