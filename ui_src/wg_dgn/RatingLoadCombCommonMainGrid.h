#if !defined(AFX_RATINGLOADCOMBCOMMONMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RATINGLOADCOMBCOMMONMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombCommonMainGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_MsgDll.h"

#define D_LCOM_CT_ADD       _LS(IDS_CMD_LOADCOMBI_Add)
#define D_LCOM_CT_ENVELOPE  _LS(IDS_CMD_LOADCOMBI_Envelope)
#define D_LCOM_CT_ABS       _T("ABS")
#define D_LCOM_CT_SRSS      _T("SRSS")

#define LCOM_KEY_FMT        _T("%d")
#define D_TYPE_DEF  D_LCOM_CT_ADD

class CRatingLoadCombCommonSubGrid;

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonMainGrid window

// 메모 : DB에 값을 넣는 것은 Child에 있음

class CRatingLoadCombCommonMainGrid : public CTBCommon
{
// Construction
public:
	CRatingLoadCombCommonMainGrid();

// Attributes
public:
	CString m_aCurDefVal[50];
	CRatingLoadCombCommonSubGrid* m_pSubGrid;
	CRatingLoadCombCommonSubGrid* m_pSubGridMV;

// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);

	//void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }
	void ChangeLcomType(int nLcomType);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombCommonMainGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID()
	{
		return 0;
	}
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual long GetSelectedKeys(CArray<long, long> &caKey);
	virtual BOOL AddToDB(ROWCOL nRow) = 0;
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData) = 0;
	virtual BOOL DeleteFromDB(ROWCOL nRow) = 0;
	virtual BOOL DeleteFromDB(CRowColArray& awRows) = 0;
	virtual BOOL ModifyToDB(ROWCOL nRow) = 0;
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
									CArray<void*, void*>& raKey, CArray<void*, void*>& raData) = 0;

	virtual CString GetNewKey() = 0;
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData) = 0;
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData) = 0;

	virtual long GetCountData() = 0;
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols) = 0;
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord) = 0;

	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);

	// Main Grid, Sub Grid간의 동기화
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow) = 0;
	virtual BOOL OnCanceledModify();

	virtual BOOL OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow) = 0;

	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste 막는다.
	virtual BOOL DeleteRecord(ROWCOL nRow);

	//virtual BOOL OnAddNewRecord(ROWCOL nRow);

	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);
	BOOL CheckCurrentRecordChanged();
	void SetModeEdit();	
public: 
	int     GetCombType(CString &value);
	CString GetCombTypeStr(int nCombType);
	CString GetLcomTypeList(int nLcomType);


// Implementation
public:
	virtual ~CRatingLoadCombCommonMainGrid();

	void GetAllSelectedLcom(CArray<UINT, UINT>& aSelKey);

	CString GetActiveChoiceList(int nLcomType);
	int  GetActive(int nLcomType, CString& value);
	CString GetActiveName(int nLcomType, int value);
	CString GetLcomActiveDef(int nLcomType);


	// Generated message map functions
protected:
	BOOL GetRowNoByKey(UINT key, ROWCOL& nRow);
	BOOL GetInsertRowByKey(UINT key, ROWCOL& nRow);

	//int  m_nLcomType;
	BOOL m_bElastStag;
	BOOL m_bTB10002_1_05;
	
	BOOL IsActiveCol(ROWCOL& nCol);
	BOOL m_bIgnoreSetEditMode;
	BOOL m_LastDelRow;

	//{{AFX_MSG(CRatingLoadCombCommonMainGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);  
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<UINT, UINT> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATINGLOADCOMBCOMMONMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)

	