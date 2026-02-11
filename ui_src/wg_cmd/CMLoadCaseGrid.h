#if !defined(AFX_CMLOADCASEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMLOADCASEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCaseGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

class CDBDoc;
class CCMLoadCaseDlg;

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseGrid window
class CCMLoadCaseGrid : public CTBCommon
{
// Construction
public:
	CCMLoadCaseGrid();

// Attributes
public:
	static CString m_aCurDefVal[4];

// Operations
public:
	void Initialize(CDBDoc* pDoc, CCMLoadCaseDlg* pParent);

	BOOL DeleteRecordByKey(T_STLD_K &key, T_STLD_D &data);
	BOOL InsertRecordByKey(T_STLD_K &key, T_STLD_D &data);
	void UnitChanged(CDBDoc* pDoc);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCaseGrid)
	//}}AFX_VIRTUAL
public:
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STLD_K &key, T_STLD_D &data);
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

	virtual BOOL OnTrackColWidth(ROWCOL nCol);
	virtual BOOL OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	BOOL CheckCurrentRecordChanged();

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentStld(T_STLD_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentStld(T_STLD_K key, T_STLD_D& data);

// Implementation
public:
	virtual ~CCMLoadCaseGrid();
	BOOL GetValue(ROWCOL nRow, T_STLD_K &key, T_STLD_D &data);
	BOOL SetValue(ROWCOL nRow, T_STLD_K key, T_STLD_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, T_STLD_K key, T_STLD_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_STLD_D& data);
	void ConvDataToStr(int i, T_STLD_D& data, CString& value);

	void GetAllSelectedStld(CArray<T_STLD_K, T_STLD_K>& aStldKey);
	void MakeSearchKey(T_STLD_K &key, T_STLD_D &data, CStringArray &aKey);

	CString GetLoadTypeCode(CString &csLoadTypeName);
	CString GetLoadTypeName(CString &csLoadTypeCode);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadCaseGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDynamicReportTable();                      
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);   
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CArray<T_STLD_K, T_STLD_K> m_aSelectedKeys;
	ROWCOL m_nCurrentRow;
	CCMLoadCaseDlg* m_pParent;
	CBCGPPopupMenu* m_pPopupMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADCASEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_)
