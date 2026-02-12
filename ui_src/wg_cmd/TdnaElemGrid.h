#if !defined(AFX_TDNAELEMGRID_H__98213AF9_A982_41C7_92E9_AAA49304A961__INCLUDED_)
#define AFX_TDNAELEMGRID_H__98213AF9_A982_41C7_92E9_AAA49304A961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdnaElemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

struct T_TDNA_ELEM_D
{
	CString strAssigned;
	int nInsertion;
};

//#include "CMTendonProfileCopyDlg.h"
class CCMTendonProfileCopyDlg;

/////////////////////////////////////////////////////////////////////////////
// CTdnaElemGrid window
class CTdnaElemGrid : public CTBCommon
{
// Construction
public:
	CTdnaElemGrid();

// Attributes
public:
	static CString m_aCurDefVal[3];
	CDialogMove* m_pParent;
	ROWCOL m_nCurrentRow;


// Operations
public:
	void Initialize(CDBDoc* pDoc, CDialogMove* pParent);

	BOOL DeleteRecordByKey(const int &key, const T_TDNA_ELEM_D &data);
	BOOL InsertRecordByKey(const int &key, const T_TDNA_ELEM_D &data);
	BOOL ModifyRecordByKey(const int &key, const T_TDNA_ELEM_D &data);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTdnaElemGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_TDNA_ELEM_D &data);
	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual long GetSelectedKeys(CArray<long, long> &caKey);
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

	// Main Grid, Sub Grid간의 동기화
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow);

	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL CheckCurrentRecordChanged();
	BOOL GetCurrentElem(int& nKey);
	BOOL SetCurrentElem(int key, T_TDNA_ELEM_D& data);
	
	// 외부에서 현재 레코드의 키 참조
 // BOOL GetCurrentFbld(int& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	//BOOL SetCurrentFbld(int key, T_TDNA_ELEM_D& data);
	void ReStoreKeys();

// Implementation
public:
	virtual ~CTdnaElemGrid();
	BOOL GetValue(ROWCOL nRow, int &key, T_TDNA_ELEM_D &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_TDNA_ELEM_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_TDNA_ELEM_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_TDNA_ELEM_D& data);
	void ConvDataToStr(int i, const T_TDNA_ELEM_D& data, CString& value);

	void GetAllSelectedFbld(CArray<int, int>& aSelKey);
	void MakeSearchKey(const int &key, const T_TDNA_ELEM_D &data, CStringArray &aKey);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTdnaElemGrid)
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDNAELEMGRID_H__98213AF9_A982_41C7_92E9_AAA49304A961__INCLUDED_)
