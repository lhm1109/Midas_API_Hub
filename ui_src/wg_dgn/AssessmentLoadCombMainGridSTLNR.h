#if !defined(AFX_AssessmentLoadCombMainGridSTLNR_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_AssessmentLoadCombMainGridSTLNR_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentLoadCombMainGridSTLNR.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_MsgDll.h"

class CAssessmentLoadCombSubGridSTLNR;
class CAssessmentLoadCombSTLNRDlg;
/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombMainGridSTLNR window

// 메모 : DB에 값을 넣는 것은 Child에 있음

class CAssessmentLoadCombMainGridSTLNR : public CTBCommon
{
	// Construction
public:
	CAssessmentLoadCombMainGridSTLNR(CAssessmentLoadCombSTLNRDlg* pParent);

	// Attributes
public:
	CString m_aCurDefVal[50];
	CAssessmentLoadCombSubGridSTLNR* m_pSubGrid;
	BOOL m_nUpdateState; // 0 : By Modify, 1 : By Add

// Operations
public:
	BOOL DeleteRecordByKey(const T_LCOM_K &key, const T_ALCS_D &data);
	BOOL InsertRecordByKey(const T_LCOM_K &key, const T_ALCS_D &data);

	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssessmentLoadCombMainGridSTLNR)
	//}}AFX_VIRTUAL

	virtual int  GetKeyColID()
	{
		return 0;
	}
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();

	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);

	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);

	// Main Grid, Sub Grid간의 동기화
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt = 1, UINT flags = 0);
	virtual BOOL OnCanceledModify();

	virtual BOOL OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol);

	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste 막는다.
	virtual BOOL DeleteRecord(ROWCOL nRow);
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_LCOM_K &key, T_ALCS_D &data);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey,
		CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
		CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);

	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);

	// Main Grid, Sub Grid간의 동기화  
	virtual void OnChangeCurrentRecord(ROWCOL nRow);

	virtual CString GetNewKey();

	//virtual BOOL OnAddNewRecord(ROWCOL nRow);

	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);
	BOOL CheckCurrentRecordChanged();
	void SetModeEdit();
public:
	int     GetLimState(CString &value);
	CString GetLimStateStr(int nVal);
	int GetCombType(CString &value);
	CString GetCombTypeStr(int nVal);
	void GetCurSelKey(CArray<T_ALCS_K, T_ALCS_K>& aAlcsK);

	// Implementation
public:
	virtual ~CAssessmentLoadCombMainGridSTLNR();

	void GetAllSelectedLcom(CArray<UINT, UINT>& aSelKey);

	BOOL GetValue(ROWCOL nRow, UINT &key, T_ALCS_D &data);
	BOOL SetValue(ROWCOL nRow, const UINT &key, const T_ALCS_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const UINT &key, const T_ALCS_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_ALCS_D& data);
	void ConvDataToStr(int i, const T_ALCS_D& data, CString& value);

	// Generated message map functions
protected:
	BOOL GetRowNoByKey(UINT key, ROWCOL& nRow);
	BOOL GetInsertRowByKey(UINT key, ROWCOL& nRow);

	//int  m_nLcomType;
	BOOL m_bElastStag;
	BOOL m_bTB10002_1_05;

	BOOL IsActiveCol(ROWCOL& nCol);
	BOOL IsDataChange(T_ALCS_K key, T_ALCS_D& dataNew);
	BOOL m_bIgnoreSetEditMode;
	BOOL m_LastDelRow;

	//{{AFX_MSG(CAssessmentLoadCombMainGridSTLNR)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<UINT, UINT> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
	CAssessmentLoadCombSTLNRDlg* m_pParent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AssessmentLoadCombMainGridSTLNR_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)

