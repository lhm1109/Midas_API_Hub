// DgnCreateRebarGrid.h: interface for the CDgnCreateRebarGrid_J class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCREATEREBARGRID_J_H__8C3EA376_66F8_4DAF_85A5_43E26DACADEE__INCLUDED_)
#define AFX_DGNCREATEREBARGRID_J_H__8C3EA376_66F8_4DAF_85A5_43E26DACADEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDgnRebarGrid.h"
#include "CNotifyTemplate.h"

#include "GTB_DgnRebarData.h"
#include "DgnStruct.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"


class CConcSectionCreateRebar_J;
/////////////////////////////////////////////////////////////////////////////
// CDgnCreateRebarGrid_J window
class CDgnCreateRebarGrid_J : public CTBCommon
{
// Construction
public:
	CDgnCreateRebarGrid_J();

// Attributes
public:
	static CString m_aCurDefVal[4];

// Operations
public:
	void Initialize(CArray<T_REBT_MABR, T_REBT_MABR&>* pData, CConcSectionCreateRebar_J* pParent);

	BOOL DeleteRecordByKey(REBAR_KEY &key, T_REBT_MABR &data);
	BOOL InsertRecordByKey(REBAR_KEY &key, T_REBT_MABR &data);
	void UnitChanged(CDBDoc* pDoc);
	
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_MabrDataList;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCreateRebarGrid_J)
	//}}AFX_VIRTUAL
public:
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, REBAR_KEY &key, T_REBT_MABR &data);
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
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow) {return TRUE;};

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	BOOL CheckCurrentRecordChanged();
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	BOOL SaveGridData();

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentMaRb(REBAR_KEY& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentMaRb(REBAR_KEY key, T_REBT_MABR& data);

	virtual void MakeItemEx();

	CDBDoc* m_pDoc;

protected:

	int m_nColHeader;

	CArray<T_REBT_MABR, T_REBT_MABR&>* m_pData;

// Implementation
public:
	virtual ~CDgnCreateRebarGrid_J();
	BOOL GetValue(ROWCOL nRow, REBAR_KEY &key, T_REBT_MABR &data);
	BOOL SetValue(ROWCOL nRow, REBAR_KEY key, T_REBT_MABR &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, REBAR_KEY key, T_REBT_MABR &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_REBT_MABR& data);
	void ConvDataToStr(int i, REBAR_KEY key, T_REBT_MABR& data, CString& value);

	void GetAllSelectedStld(CArray<REBAR_KEY, REBAR_KEY>& aStldKey);
	void MakeSearchKey(REBAR_KEY &key, T_REBT_MABR &data, CStringArray &aKey);

	CString GetLoadTypeCode(CString &csLoadTypeName);
	CString GetLoadTypeName(CString &csLoadTypeCode);

	//데이터의 Key값으로 해당 Row를 삭제한다
	void DeleteRowByKey(REBAR_KEY key);

	REBAR_KEY GetLastRebarKey();

	BOOL GridCheckData();
	BOOL bCheck_int(CString str);
	BOOL bCheck_double(CString str);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnCreateRebarGrid_J)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CArray<REBAR_KEY, REBAR_KEY> m_aSelectedKeys;
	ROWCOL m_nCurrentRow;
	CConcSectionCreateRebar_J* m_pParent;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCREATEREBARGRID_J_H__8C3EA376_66F8_4DAF_85A5_43E26DACADEE__INCLUDED_)
