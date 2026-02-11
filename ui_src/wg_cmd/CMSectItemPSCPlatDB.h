#if !defined(__CMSECTITEMPSCPLATDB_H__)
#define __CMSECTITEMPSCPLATDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMSectItemPSCIDB.h : header file
//
class CCMSectItemPSCPlatDB 
{
public:
	CCMSectItemPSCPlatDB();
	virtual ~CCMSectItemPSCPlatDB();

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
	CArray<CArray<double, double>*, CArray<double, double>*> m_aSize;
	//CArray<int, int> m_aOffset;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCPLATDB_H__)
