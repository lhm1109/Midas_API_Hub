#if !defined(AFX_CMLOADPNLDGRID_H__EB56DB4D_4CFC_43A2_B17A_F34CD0220132__INCLUDED_)
#define AFX_CMLOADPNLDGRID_H__EB56DB4D_4CFC_43A2_B17A_F34CD0220132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPNLDGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDGrid window

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
/////////////////////////////////////////////////////////////////////////////


class CMLoadPNLDDlg;
class CCMLoadPNLDGrid : public CTBCommon
{
// Construction
public:
	CCMLoadPNLDGrid();

// Attributes
public:

// Operations
public:


	// Attributes
public:
	static CString m_aCurDefVal[3];
	CMLoadPNLDDlg* m_pParent;
	ROWCOL m_nCurrentRow;

// Operations
public:
	void Initialize(CDBDoc* pDoc, CMLoadPNLDDlg* pParent);

	BOOL DeleteRecordByKey(const T_PNLD_K &key, const T_PNLD_D &data);
	BOOL InsertRecordByKey(const T_PNLD_K &key, const T_PNLD_D &data);

	  virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_PNLD_K &key, T_PNLD_D &data);
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
	BOOL GetCurrentPnld(T_PNLD_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentPnld(T_PNLD_K key, T_PNLD_D& data);

	
// Implementation
public:
	virtual ~CCMLoadPNLDGrid();
	BOOL GetValue(ROWCOL nRow, T_PNLD_K &key, T_PNLD_D &data);
	BOOL SetValue(ROWCOL nRow, const T_PNLD_K &key, const T_PNLD_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_PNLD_K &key, const T_PNLD_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_PNLD_D& data);
	void ConvDataToStr(int i, const T_PNLD_D& data, CString& value);

	void GetAllSelectedPnld(CArray<T_PNLD_K, T_PNLD_K>& aSelKey);
	void MakeSearchKey(const T_PNLD_K &key, const T_PNLD_D &data, CStringArray &aKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPNLDGrid)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadPNLDGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPNLDGRID_H__EB56DB4D_4CFC_43A2_B17A_F34CD0220132__INCLUDED_)
