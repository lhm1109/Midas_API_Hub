#if !defined(__CINODEGRIDLIST_H__)
#define __CINODEGRIDLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshRatioList.h : header file
//

#define _GXDLL
#include "./Grid/GXALL.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////

// CBndrMconGrid window
#include "HeaderPre.h"

class CBndrMconDlg;

class __MY_EXT_CLASS__ CBndrMconGrid :  public CTBCommon, public CDBUpdateConnector
{
public:
// Construction
public:
	CBndrMconGrid();
	CBndrMconGrid(CBndrMconDlg* pParent);

// Attributes
public:
	T_MCON_DT*   m_pData;
	int m_nMasterDOF;
	
	static CString m_aCurDefVal[4];

private:
	CBndrMconDlg* m_pParent;

// Operations
public:
	UINT    GetDOFKey(CString& sName);
	CString GetDOFName(UINT nKey);

	void EnableTable(BOOL);
	void Initialize(T_MCON_DT* pData);
	BOOL DeleteRecordByKey(const int &key, const T_MCON_BASE &data);
	BOOL InsertRecordByKey(const int &key, const T_MCON_BASE &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);  
	

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_MCON_BASE &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	BOOL FindData(int key, UINT nDOF, int& nPos, int nStart);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
									CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);

	BOOL GetValue(ROWCOL nRow, int &key, T_MCON_BASE &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_MCON_BASE &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_MCON_BASE &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_MCON_BASE& data);
	void ConvDataToStr(int i, const T_MCON_BASE& data, CString& value);

	void MakeSearchKey(const int &key, const T_MCON_BASE &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL AddData(CArray<T_MCON_BASE, T_MCON_BASE&> &aData, BOOL bNotify = TRUE);
	BOOL AddData(int key, T_MCON_BASE& data);
	BOOL AddData(CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(int oldKey, int key, T_MCON_BASE &data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData);  

	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData);
	void Clear();
 // BOOL GetAllDisplayedValue(CArray<double, double>& allData);
	void BubbleSort(CArray<int, int>& Param ); // 오름 차순 
	void SetType(int nType);
	void Normalize(CArray<T_MCON_BASE, T_MCON_BASE&> &aData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrMconGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void SetMasterDOF(int nDOF);
	void ConvertCoeffUnit();
	virtual ~CBndrMconGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBndrMconGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGRIDLDLNGRID_H__)
