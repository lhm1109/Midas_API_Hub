#if !defined(__DGNRCPLATEGRID_H__)
#define __DGNRCPLATEGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCPlateGrid.h : header file
//

///#include "..\wg_common\wg_common_TBBrowserWnd.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

//!/
#include "RCDesignPlateParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateGrid window

class CDBDoc;
class CRCDesignPlateParamDlg;
class CRCPlateParameter;

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateGrid window
class CDgnRCPlateGrid : public CTBCommon
{
// Construction
public:
	CDgnRCPlateGrid();

// Attributes
public:
	static CString m_aCurDefVal[3];

// Operations
public:
	void Initialize(CRCPlateParameter* pParent);

	BOOL DeleteRecordByKey(T_RCPL_K &key, T_RCPL_D &data);
	//BOOL DeleteRecordByKey(CString strName);
	BOOL InsertRecordByKey(CString strName);
	
	void UnitChanged(CDBDoc* pDoc);

	CMap<T_RCPL_K,T_RCPL_K,T_RCPL_D,T_RCPL_D> m_RcplData;	

	BOOL LockUpdateGrid(BOOL bUpdate);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRCPlateGrid)
	//}}AFX_VIRTUAL
public:
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_RCPL_K &key, T_RCPL_D &data);
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

	// 외부에서 현재 레코드의 키 참조
	BOOL GetCurrentRcpl(T_RCPL_K& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentRcpl(T_RCPL_K key, T_RCPL_D& data);

	virtual void MakeItemEx();

	CDBDoc* m_pDoc;

protected:

	int m_nColHeader;
	int m_nColCount;

	

// Implementation
public:
	virtual ~CDgnRCPlateGrid();
	BOOL GetValue(ROWCOL nRow, T_RCPL_K &key, T_RCPL_D &data);
	BOOL SetValue(ROWCOL nRow, T_RCPL_K key, T_RCPL_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, T_RCPL_K key, T_RCPL_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_RCPL_D& data);
	void ConvDataToStr(int i, T_RCPL_K key, T_RCPL_D& data, CString& value);

	void GetAllSelectedStld(CArray<T_RCPL_K, T_RCPL_K>& aStldKey);
	void MakeSearchKey(T_RCPL_K &key, T_RCPL_D &data, CStringArray &aKey);

	CString GetLoadTypeCode(CString &csLoadTypeName);
	CString GetLoadTypeName(CString &csLoadTypeCode);	

	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnRCPlateGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CArray<T_RCPL_K, T_RCPL_K> m_aSelectedKeys;
	ROWCOL m_nCurrentRow;
	CRCPlateParameter* m_pParent;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNRCPLATEGRID_H__)

