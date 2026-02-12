#ifndef __DB_TVSF_DB_H__
#define __DB_TVSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_TVSF
{
public:
    CDB_TVSF();
    CDB_TVSF(CDB_TVSF& src);
    virtual ~CDB_TVSF();
    CDB_TVSF& operator=(CDB_TVSF& src);
    CDBDoc* m_pDoc;

public:
    void Add(T_TVSF_K Key,T_TVSF_D& rData,CDB_SECT* pSect);
    BOOL Del(T_TVSF_K Key,CDB_SECT* pSect);

public:
    BOOL Get(T_TVSF_K Key,T_TVSF_D& rData);
    int GetCount();
    POSITION GetStart();
    void GetNext(POSITION& rNextPosition,T_TVSF_K& rKey,T_TVSF_D& rData);

    void AddList(T_TVSF_K Key);
    BOOL DelList(T_TVSF_K Key);
    void DelAllList();
    BOOL GetList(T_TVSF_K Key,TvsfList*& rpList);
    int GetCountList();
    void AddListItem(T_TVSF_K Key, int nCmd, int nKey);
    BOOL DelListItem(T_TVSF_K Key, int nCmd, int nKey);


protected:
    CMap<T_TVSF_K,T_TVSF_K,T_TVSF_D,T_TVSF_D&> m_tvsf;
    CMap<T_TVSF_K,T_TVSF_K,TvsfList*,TvsfList*> m_tvsflist;
};

#endif

