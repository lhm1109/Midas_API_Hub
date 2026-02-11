#if !defined(AFX_LOADCASEUNDERMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_LOADCASEUNDERMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadCaseUnderMainGrid.h : header file
//

#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadCaseUnderMainGrid window
struct T_UGLC_D;
class CDBDoc;
class CLoadCaseUnderMainGrid : public CTBCommon
{
public:
	CLoadCaseUnderMainGrid();
	virtual ~CLoadCaseUnderMainGrid();

	// Attributes
public:
	CString m_aCurDefVal[2] = { _T("0"), _LS(IDS_CMD_PLUS) };

	// Operations
public:
	void Initialize(CWnd* pParent, int nWidth);
	void SetTitle();
		
	void SetData(T_UGLC_D& UglcD);
	BOOL GetData(T_UGLC_D& UglcD);
	void DeleteRecordAndData(ROWCOL nRow);
	void Data2Grid();
	void Grid2Data();

	// Overrides
	virtual int GetKeyColID() { return 0; }
	virtual long GetSelectedKeys(CArray<long, long> &caKey)  { return 0; }
	
	BOOL ValidateField(CString value, int nColID) {return TRUE;}
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols) { return TRUE; }
	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData) { return TRUE; }
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData) { return TRUE; }

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex) { return m_aCurDefVal[nIndex]; }
	virtual CString* GetDefValueArray() { return m_aCurDefVal; }
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData) { return TRUE; }
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData) {return ;}

	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols) {}
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols) {}
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord) {}

	BOOL GetValue(ROWCOL nRow, T_UGLC_BASE_LOAD &data);
	BOOL SetValue(ROWCOL nRow, T_UGLC_BASE_LOAD &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, T_UGLC_BASE_LOAD &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_UGLC_BASE_LOAD& data);
	void ConvDataToStr(int i, T_UGLC_BASE_LOAD& data, CString& value);
	
	BOOL FindData(T_UGLC_BASE_LOAD& data, int& nPos);
	BOOL ModifyData(int nOldIndex, int nIndex, T_UGLC_BASE_LOAD &data);
	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);
	BOOL CheckCurrentRecordChanged();
	
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();

public:
	void ResetComboColumn() { SetComboColumn(); }

	// override 
public:
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	
protected:
	int m_nWndWidth;
		
	void* m_pParent;
	CDBDoc* m_pDoc;

	CArray<T_UGLC_BASE_LOAD,T_UGLC_BASE_LOAD&> m_aData;

	void SetComboColumn();
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	BOOL GetLoadCaseName(const T_UGLC_BASE_LOAD& BaseLoad, CString& strLoadCaseName);
	CString GetDirTitle(int nDir);
	int GetDirByTitle(const CString& strTitle);

protected:
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADCASEUNDERMAINGRID_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
