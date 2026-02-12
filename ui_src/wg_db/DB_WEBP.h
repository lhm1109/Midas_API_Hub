#ifndef __DB_WEBP_DB_H__
#define __DB_WEBP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_WEBP
{
public:
	CDB_WEBP();
	virtual ~CDB_WEBP();
	CDBDoc* m_pDoc;

public:
	T_WEBP_K m_nStartNum;
	T_WEBP_K m_nLastNum;

public:
	void Add(T_WEBP_K Key, T_WEBP_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_WEBP_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_WEBP_K Key, T_WEBP_D& rData);
	//{return m_webp.Lookup(Key,rData);}
	int GetCount();
	//{return m_webp.GetCount();}
	POSITION GetStart();
	//{return m_webp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_WEBP_K& rKey, T_WEBP_D& rData);
	//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetWebpAssigned(T_ELEM_K KeyElem, T_WEBP_K& rKeyWebp);   // element가 속한 webper의 key를 찾아준다.
		//{return m_elemtowebp.Lookup(KeyElem, rKeyWebp);}
protected:
	CMap<T_WEBP_K, T_WEBP_K, T_WEBP_D, T_WEBP_D&> m_webp;
	CMap<T_ELEM_K, T_ELEM_K, T_WEBP_K, T_WEBP_K>  m_elemtowebp;
};

#endif

