#ifndef __DB_OCDF_DB_H__
#define __DB_OCDF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_OCDF
{
public:
	CDB_OCDF();
	virtual ~CDB_OCDF();
	CDBDoc* m_pDoc;

public:
	void Add(T_OCDF_K Key,T_OCDF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_OCDF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_OCDF_K Key,T_OCDF_D& rData);
		//{return m_ocdf.Lookup(Key,rData);}
	int GetCount();
		//{return m_ocdf.GetCount();}
	POSITION GetStart();
		//{return m_ocdf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_OCDF_K& rKey,T_OCDF_D& rData);
		//{m_ocdf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_OCDF_K,T_OCDF_K,T_OCDF_D,T_OCDF_D&>m_ocdf;
};

#endif

