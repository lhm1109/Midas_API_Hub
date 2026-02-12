#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CRFSecViewWnd.h"

class CRS_WeldGrid : public CTBCommon
{
public:
	CRS_WeldGrid();

	// Attributes
public:
	T_RFST_WELD_D* m_pWeldData;
	CArray<T_RFST_WELD_SUB_D, T_RFST_WELD_SUB_D&>* m_pData;
	static CString m_aCurDefVal[5];

	// Operations
public:
	void Initialize(T_RFST_WELD_D* pWeldData, CArray<T_RFST_WELD_SUB_D, T_RFST_WELD_SUB_D&>* pData);
	void Data2Grid();
	void Grid2Data();
	//BOOL DeleteRecordByKey(const double &key, const double &data);
	//BOOL InsertRecordByKey(const double &key, const double &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long* keys, int nSize) {}
	virtual long GetSelectedKeys(CArray<long, long>& caKey) { return 0; }

	virtual BOOL ValidateAndMakeRecord(CStringArray& value, CRowColArray& aCols, double& key, double& data) { return TRUE; }
	virtual BOOL ValidateField(CString value, int nColID) { return TRUE; }
	virtual BOOL ValidateRecord(CStringArray& value, CRowColArray& aCols) { return TRUE; }

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
		CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemEx(long* aKey = 0, long nKeyNum = -2, BOOL bUnitChanged = FALSE);
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord) {}

	//BOOL GetValue(ROWCOL nRow, double &key, double &data);
	//void ConvStrToData(int i, CString& value, double& data);

	//void MakeSearchKey(const double &key, const double &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM& UnitSystem);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
public:
	virtual ~CRS_WeldGrid();
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

class CmdReinforceSectWeldDlg : public CDialogMove
{
	// Construction
public:
	CmdReinforceSectWeldDlg(T_RFST_WELD_D* pData, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CmdReinforceSectWeldDlg)
	enum
	{
		IDD = IDD_CMD_REINFORCE_SECT_WELD_DLG
	};
private:
	int				m_nWeldType;
	CComboBox		m_WeldCountCombox;
	CFormulaEdit	m_HfEdit;
	CFormulaEdit	m_LengthEdit;
	CFormulaEdit	m_SpaceEdit;

	CTextUnit	m_wndWeldUnit[3];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_RFST_WELD_D*	m_pWeldData;
	CArray<T_RFST_WELD_SUB_D, T_RFST_WELD_SUB_D&> m_SubWeldData;
	CRS_WeldGrid	m_wndGrid;

	void Data2Dlg();
	BOOL Dlg2Data();
public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
private:
	void InitGrid();
protected:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnChangeWeldCount();
	afx_msg void OnChangeLength();
	afx_msg void OnChangeSpace();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

