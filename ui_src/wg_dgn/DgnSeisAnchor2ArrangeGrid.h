#if !defined(__DgnSeisAnchor2ArrangeGrid_H__)
#define __DgnSeisAnchor2ArrangeGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2ArrangeGrid.h : header file
//

//#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_common\wg_common_TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ArrangeGrid window
#include "HeaderPre.h"

class CDgnSeisAnchorEvalItem2NewSubArrangeDlg;
class __MY_EXT_CLASS__ CDgnSeisAnchor2ArrangeGrid : public CTBCommon
{
// Construction
public:
	CDgnSeisAnchor2ArrangeGrid(CWnd* pParent = NULL);
	virtual ~CDgnSeisAnchor2ArrangeGrid();

// Attributes
public:
	T_ANEV_D* m_pData;
	T_ANEV_D  m_AnveD;
	int m_nInputType;
	int  m_nCurveType;
	CString m_aCurDefVal[10];

	BOOL m_bDblClick;

// Operations
public:
	void Initialize(T_ANEV_D* pData);    // nType : (1) 3D  (2) 2D-XY (3) 2D-XZ
	BOOL DeleteRecordByKey(const double &key, const T_ANEV_LAYER_D &data);
	BOOL InsertRecordByKey(const double &key, const T_ANEV_LAYER_D &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_ANEV_LAYER_D &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	
	virtual BOOL OnDeleteRecord(ROWCOL nRow);

	BOOL GetValue(ROWCOL nRow, double &key, T_ANEV_LAYER_D &data, T_ANEV_BRPR_KEYS_D &Prop, T_ANEV_REG_SUPP_LAYER_D &Supp, T_ANEV_FAIL_D& Fail);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_ANEV_LAYER_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_ANEV_LAYER_D &data, CRowColArray &awCols);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData);
	void OnUnitChange();

protected:
	void ConvStrToData(int i, CString& value, T_ANEV_LAYER_D& data);
	void ConvDataToStr(ROWCOL nRow, int i, const T_ANEV_LAYER_D& data, CString& value);
	BOOL AddData(double key, T_ANEV_LAYER_D data, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail);
	BOOL AddData(CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData, BOOL bNotify=TRUE);
	void MakeSearchKey(const double &key, const T_ANEV_LAYER_D &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(ROWCOL nRow, double oldKey, double key, T_ANEV_LAYER_D data, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData);
	void ChangeGridColEnable(ROWCOL nRow);
	void ChangeGridColData(ROWCOL nRow);
	void ChangeGridColDataSet(ROWCOL nRow, double Key, T_ANEV_LAYER_D Layer, T_ANEV_LAYER_D& LayerNew);

	void RemoveAt(int nPos);
	void InsertAt(int nPos, T_ANEV_LAYER_D data, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail);

	BOOL CheckData(T_ANEV_LAYER_D Layer, BOOL bMsg=TRUE);
	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
//	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeCellDataForDlg(int nNumData, BOOL bEndEdit=FALSE);

	void SetViewData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchor2ArrangeGrid)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	CDgnSeisAnchorEvalItem2NewSubArrangeDlg* m_pParent;

	//{{AFX_MSG(CDgnSeisAnchor2ArrangeGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2ArrangeGrid_H__)
