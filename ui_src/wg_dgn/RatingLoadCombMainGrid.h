#if !defined(AFX_RATINGLOADCOMBMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RATINGLOADCOMBMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombMainGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

	#define D_LCOM_CT_ADD       _LS(IDS_CMD_LOADCOMBI_Add)
	#define D_LCOM_CT_ENVELOPE  _LS(IDS_CMD_LOADCOMBI_Envelope)
	#define D_LCOM_CT_ABS       "ABS"
	#define D_LCOM_CT_SRSS      "SRSS"


class CRatingLoadCombSubGrid;

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombMainGrid window

class CRatingLoadCombMainGrid : public CTBCommon
{
// Construction
public:
	CRatingLoadCombMainGrid();

// Attributes
public:
	static CString m_aCurDefVal[];
	CRatingLoadCombSubGrid* m_pSubGrid;
	CRatingLoadCombSubGrid* m_pSubGridMV;

// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);

	BOOL DeleteRecordByKey(const T_LCOM_K &key, const T_RLCS_D &data);
	BOOL InsertRecordByKey(const T_LCOM_K &key, const T_RLCS_D &data);

	//void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }
	void ChangeLcomType(int nLcomType);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombMainGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_LCOM_K &key, T_RLCS_D &data);
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

	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);

	// Main Grid, Sub Grid간의 동기화
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	virtual BOOL OnCanceledModify();

	virtual BOOL OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);

	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste 막는다.

	//virtual BOOL OnAddNewRecord(ROWCOL nRow);

	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);
	BOOL CheckCurrentRecordChanged();
	void SetModeEdit();	
public: 
	static int     GetCombType(CString &value);
	static CString GetCombTypeStr(int nCombType);
	static CString GetLcomTypeList(int nLcomType);


// Implementation
public:
	virtual ~CRatingLoadCombMainGrid();
	BOOL GetValue(ROWCOL nRow, T_RLCS_K &key, T_RLCS_D &data);
	BOOL SetValue(ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_RLCS_D& data);
	void ConvDataToStr(int i, const T_RLCS_D& data, CString& value);

	void GetAllSelectedLcom(CArray<T_RLCS_K, T_RLCS_K>& aSelKey);

	static CString GetActiveChoiceList(int nLcomType);
	static int  GetActive(int nLcomType, CString& value);
	static CString GetActiveName(int nLcomType, int value);
	static CString GetLcomActiveDef(int nLcomType);


	// Generated message map functions
protected:

	BOOL GetRowNoByKey(T_RLCS_K key, ROWCOL& nRow);
	BOOL GetInsertRowByKey(T_RLCS_K key, ROWCOL& nRow);

	//int  m_nLcomType;
	static BOOL m_bElastStag;
	static BOOL m_bTB10002_1_05;
	
	BOOL IsActiveCol(ROWCOL& nCol);
	BOOL m_bIgnoreSetEditMode;

	//{{AFX_MSG(CRatingLoadCombMainGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);  
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<T_RLCS_K, T_RLCS_K> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATINGLOADCOMBMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)

	