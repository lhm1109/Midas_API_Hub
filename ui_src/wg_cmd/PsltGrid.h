#if !defined(__PSLTGRID_H__)
#define __PSLTGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PsltGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CPsltGrid window
class CCMPsltDlg;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CPsltGrid : public CTBCommon
{
	// Construction
public:
	CPsltGrid();

	// Attributes
public:
	static CString m_aCurDefVal[4];
	CCMPsltDlg* m_pParent;
	ROWCOL m_nCurrentRow;

	// Operations
public:
	void Initialize(CDBDoc* pDoc, CCMPsltDlg* pParent);

	BOOL DeleteRecordByKey(const T_PSLT_K &key, const T_PSLT_D &data);
	BOOL InsertRecordByKey(const T_PSLT_K &key, const T_PSLT_D &data);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsltGrid)
	//}}AFX_VIRTUAL

	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_PSLT_K &key, T_PSLT_D &data);
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

	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

	BOOL CheckCurrentRecordChanged();

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentPslt(T_PSLT_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentPslt(T_PSLT_K key, T_PSLT_D& data);

	// Implementation
public:
	virtual ~CPsltGrid();
	BOOL GetValue(ROWCOL nRow, T_PSLT_K &key, T_PSLT_D &data);
	BOOL SetValue(ROWCOL nRow, const T_PSLT_K &key, const T_PSLT_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_PSLT_K &key, const T_PSLT_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_PSLT_D& data);
	void ConvDataToStr(int i, const T_PSLT_D& data, CString& value);

	void GetAllSelectedPslt(CArray<T_PSLT_K, T_PSLT_K>& aSelKey);
	void MakeSearchKey(const T_PSLT_K &key, const T_PSLT_D &data, CStringArray &aKey);
	void SelectPsltAndUpdateDlg(T_PSLT_K PsltK);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPsltGrid)
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMap<int,int,T_PSLT_K,T_PSLT_K> m_mapRow2PsltK;
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PSLTGRID_H__)
