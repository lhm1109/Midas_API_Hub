#if !defined(__CMSECTITEMPSCIDB_H__)
#define __CMSECTITEMPSCIDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMSectItemPSCIDB.h : header file
//
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSectItemPSCIDB 
{
public:
	CCMSectItemPSCIDB();
	virtual ~CCMSectItemPSCIDB();

	// interface
public:
	int  GetTypeNameList(CStringArray& aTypeList);
	int  GetDBNameList(CString &strTypeName, CStringArray& aNameList);
	BOOL GetDBData(CString &csName, CArray<BOOL, BOOL> &aOffset, CArray<double, double> &aSize);

	// operation
protected:
	void MakeData();

	// member var.
protected:
	CStringArray m_aDBNameList;
	CArray<CArray<double, double>*, CArray<double, double>*> m_aSize;
	CArray<CArray<BOOL, BOOL>*, CArray<BOOL, BOOL>*> m_aOffset;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCIDB_H__)
