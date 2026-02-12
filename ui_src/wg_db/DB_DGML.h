#ifndef __DB_DGML_DB_H__
#define __DB_DGML_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DGML
{
public:
	CDB_DGML();
	virtual ~CDB_DGML();
	CDBDoc* m_pDoc;
	int m_nCmd;
	T_DGML_K m_nStartNum;
	T_DGML_K m_nLastNum;

public:
	void Add(T_DGML_K Key,T_DGML_D& rData);
	BOOL Del(T_DGML_K Key);

public:
	BOOL Get(T_DGML_K Key,T_DGML_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_DGML_K& rKey,T_DGML_D& rData);
	CMap<T_DGML_K,T_DGML_K,UINT, UINT>* GetDgmlMap();

protected:
	CMap<T_DGML_K,T_DGML_K,T_DGML_D,T_DGML_D&>m_dgml;
};

#endif
