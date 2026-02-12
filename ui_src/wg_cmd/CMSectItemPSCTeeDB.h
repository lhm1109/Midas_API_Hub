#if !defined(__CMSECTITEMPSCTEEDB_H__)
#define __CMSECTITEMPSCTEEDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMSectItemPSCIDB.h : header file
//
class CCMSectItemPSCTeeDB 
{
public:
	CCMSectItemPSCTeeDB();
	virtual ~CCMSectItemPSCTeeDB();

	// interface
public:
	int  GetDBNameList(CStringArray& aNameList);
	BOOL GetDBData(CString &csName, int &nOffset, CArray<double, double> &aSize);

	// operation
protected:
	void MakeData();
	void Convert2MidasForamt(CArray<double, double> &aDBSize, CArray<double, double> &aMSize);

	// member var.
protected:
	CStringArray m_aDBNameList;
	CArray<CArray<double, double>*, CArray<double, double>*> m_aSize,m_aSize2;
	CArray<int, int> m_aOffset,m_aOffset2;
	//CArray<int, int> m_aOffset;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCTEEDB_H__)
