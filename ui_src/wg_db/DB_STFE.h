#ifndef __DB_STFE_DB_H__
#define __DB_STFE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STFE
{
public:
	CDB_STFE();
	virtual ~CDB_STFE();
	CDBDoc* m_pDoc;

public:
	void Add(T_STFE_K Key,T_STFE_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STFE_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_STFE_K Key,T_STFE_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_STFE_K& rKey,T_STFE_D& rData);

protected:
	CMap<T_STFE_K,T_STFE_K,T_STFE_D,T_STFE_D&>m_stfe;
};

#endif

