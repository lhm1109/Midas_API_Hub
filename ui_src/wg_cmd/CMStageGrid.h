/* 사용안함
#if !defined(AFX_CMSTAGEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSTAGEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

class CCMStageDefMainDlg;

/////////////////////////////////////////////////////////////////////////////
// CCMStageGrid window
class CCMStageGrid : public CTBCommon, public CDBUpdateConnector
{
// Construction
public:
	CCMStageGrid();

// Attributes
public:
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(CCMStageDefMainDlg* pParent);

	BOOL DeleteRecordByKey(T_STAG_K &key, T_STAG_D &data);
	BOOL InsertRecordByKey(T_STAG_K &key, T_STAG_D &data);
	void UpdateBuffer();
	void UnitChanged();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageGrid)
	//}}AFX_VIRTUAL
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STAG_K &key, T_STAG_D &data);
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

	virtual void SetHeaderTitle(T_UNIT_SYSTEM& UnitSystem, BOOL bInit);


	virtual BOOL OnTrackColWidth(ROWCOL nCol);
	virtual BOOL OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	BOOL CheckCurrentRecordChanged();

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentStag(T_STAG_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentStag(T_STAG_K key, T_STAG_D& data);

	// Date 설정을 위해 Override
	virtual void AdjustAppendRowNo(ROWCOL nCol = 0);
	void SetStageDate(ROWCOL nDateCol);

	// ID 설정
	void SetId(ROWCOL nIdCol);

	// 셀 수정이 다른 셀의 값이나  Enable 상태 변경하는 경우 때문에 Override
	// Duration 변경시 Date의 값 변경
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnCanceledModify();
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol);
	void RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol);

// Implementation
public:
	virtual ~CCMStageGrid();
	BOOL GetValue(ROWCOL nRow, T_STAG_K &key, T_STAG_D &data);
	BOOL SetValue(ROWCOL nRow, T_STAG_K key, T_STAG_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, T_STAG_K key, T_STAG_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_STAG_K& rKey, T_STAG_D& rData);
	void ConvDataToStr(int i, T_STAG_K& rKey, T_STAG_D& rData, CString& value);

	void GetAllSelectedStag(CArray<T_STAG_K, T_STAG_K>& aStagKey);
	void MakeSearchKey(T_STAG_K &key, T_STAG_D &data, CStringArray &aKey);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMStageGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CArray<T_STAG_K, T_STAG_K> m_aSelectedKeys;
	ROWCOL m_nCurrentRow;
	CCMStageDefMainDlg* m_pParent;

	BOOL m_bBeforeSaved;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEGRID_H__ED800CE1_4A44_11D3_92DE_0000C0B0E6B3__INCLUDED_)
*/