// DgnStlRatingLcomAASHTOMainGrid.h: interface for the CDgnStlRatingLcomAASHTOMainGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNSTLRATINGLCOMAASHTOMAINGRID_H__)
#define __DGNSTLRATINGLCOMAASHTOMAINGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_MsgDll.h"



class CDgnStlRatingLcomAASHTODlg;
class CDgnStlRatingLcomAASHTOSubGrid;

class CDgnStlRatingLcomAASHTOMainGrid : public CTBCommon
{
public:
	CDgnStlRatingLcomAASHTOMainGrid();

	T_RTLD_D RateLoad[8];

// Attributes
public:
	static CString m_aCurDefVal[];
	CDgnStlRatingLcomAASHTOSubGrid* m_pSubGrid;

	ROWCOL m_nCurrentRow;
	
// Operations
public:
	void Initialize(T_RTLD_D RateData[8], CDgnStlRatingLcomAASHTODlg* pParent);

	BOOL DeleteRecordByKey(int &key, T_RTLD_D &data);
	BOOL InsertRecordByKey(int &key, T_RTLD_D &data);
	
	void UnitChanged(CDBDoc* pDoc);

	BOOL LockUpdateGrid(BOOL bUpdate);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCombMainGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_RTLD_D &data);
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
	BOOL GetCurrentRcbe(int& nKey);
	// 외부에서 현재 레코드로 Current Cell 이동
	BOOL SetCurrentRcbe(int key, T_RTLD_D& data);

	virtual void MakeItemEx();

	CDBDoc* m_pDoc;

	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);

	void SaveCurrentGrid();	

// Implementation
public:
	virtual ~CDgnStlRatingLcomAASHTOMainGrid();
	BOOL GetValue(ROWCOL nRow, int &key, T_RTLD_D &data);
	BOOL SetValue(ROWCOL nRow, int key, T_RTLD_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, int key, T_RTLD_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_RTLD_D& data);
	void ConvDataToStr(int i, T_RTLD_D& data, CString& value);

	void GetAllSelectedStld(CArray<int, int>& aStldKey);
	void MakeSearchKey(int &key, T_RTLD_D &data, CStringArray &aKey);

	CString GetLoadTypeCode(CString &csLoadTypeName);
	CString GetLoadTypeName(CString &csLoadTypeCode);	

	void LoadDlgData();
	void SetRtldData(T_RTLD_D RateData[8]);

	// Generated message map functions
protected:
	int  m_nLcomType;
	BOOL m_bElastStag;

	BOOL IsActiveCol(ROWCOL& nCol);

	int m_nColHeader;
	int m_nColCount;

	//{{AFX_MSG(CCMLoadCombMainGrid)
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<int, int> m_aSelectedKeys;

	void Set_Title();
	void SetCellStyle();
	int GetDataIndexfromRow(int nRow);

private:
	CDgnStlRatingLcomAASHTODlg* m_pParent;

};

#endif // !defined(__DGNSTLRATINGLCOMAASHTOMAINGRID_H__)
