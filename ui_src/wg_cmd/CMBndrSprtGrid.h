#if !defined(__CMBNDRSPRTGRID_H__)
#define      __CMBNDRSPRTGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBndrSprtGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

class CCMBndrSprtDlg;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMBndrSprtGrid : public CTBCommon
{
// Construction
public:
	CCMBndrSprtGrid();

// Attributes
public:
	static CString m_aCurDefVal[3];
	CCMBndrSprtDlg* m_pParent;
	ROWCOL m_nCurrentRow;

// Operations
public:
	void Initialize(CDBDoc* pDoc, CCMBndrSprtDlg* pParent);

	BOOL DeleteRecordByKey(const T_GSTP_K &key, const T_GSTP_D &data);
	BOOL InsertRecordByKey(const T_GSTP_K &key, const T_GSTP_D &data);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBndrSprtGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_GSTP_K &key, T_GSTP_D &data);
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

	BOOL CheckCurrentRecordChanged();

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentGSTP(T_GSTP_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentGSTP(T_GSTP_K key, T_GSTP_D& data);

// Implementation
public:
	virtual ~CCMBndrSprtGrid();
	BOOL GetValue(ROWCOL nRow, T_GSTP_K &key, T_GSTP_D &data);
	BOOL SetValue(ROWCOL nRow, const T_GSTP_K &key, const T_GSTP_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_GSTP_K &key, const T_GSTP_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_GSTP_D& data);
	void ConvDataToStr(int i, const T_GSTP_D& data, CString& value);

	void GetAllSelectedGstp(CArray<T_GSTP_K, T_GSTP_K>& aSelKey);
	void MakeSearchKey(const T_GSTP_K &key, const T_GSTP_D &data, CStringArray &aKey);

protected:
	//{{AFX_MSG(CCMBndrSprtGrid)
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMBNDRSPRTGRID_H__)
