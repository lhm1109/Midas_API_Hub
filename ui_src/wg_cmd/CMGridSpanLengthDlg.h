// CMGridSpanLengthDlg.h: interface for the CCMGridSpanLengthDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMGRIDSPANLENGTHDLG_H__E511831D_EDF0_4AFC_A93D_55BC2D4DEF9B__INCLUDED_)
#define AFX_CMGRIDSPANLENGTHDLG_H__E511831D_EDF0_4AFC_A93D_55BC2D4DEF9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxCbem.h"

#include "CMChildBarBase.h"

class CCMGridSpanLengthDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMGridSpanLengthDlg();

	//DECLARE_DYNCREATE(CCMGridSpanLengthDlg)
public:
	virtual BOOL ExternalInit(UINT key);
	void Execute(){};
// Dialog Data
	//{{AFX_DATA(CCMGridSpanLengthDlg)
	enum { IDD = IDD_CMD_GRID_SPAN_LENGTH_DLG };
	int m_nSpanType;
	int m_nConnectionType;
	CEditUnit m_edtLength;
	CTextUnit m_untLength;
	CEditUnit m_edtLengthLoad;
	CTextUnit m_untLengthLoad;
	CCobxCbem m_cmbStart;
	CCobxCbem m_cmbEnd;
	CListCtrl m_GridList;
	CString m_strTypeBrg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridSpanLengthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMGridSpanLengthDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSpanType();
	afx_msg void OnAddBtn();
	afx_msg void OnModBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnCmdBridgeType();
	afx_msg void OnItemchangedSpanLengthList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc* m_pDoc;
	T_BSPN_D m_Data;

	int m_nTypeOfBridge; //0:Simple, Continuous Girder, 1:Gerber, 2:Rahmen

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void SetHeaderTitle();
	void MakeItemEx();
	int  FindInsertionPos(int nID);
	BOOL InsertItem(T_BSPN_K Key, T_BSPN_D &Data);
	BOOL DeleteItem(T_BSPN_K Key, T_BSPN_D &Data);
	BOOL ModifyItem(T_BSPN_K KeyOld, T_BSPN_K Key, T_BSPN_D &Data);
	CString DataToStr(int i, T_BSPN_D &data);

	void ChangeUnit();
	void RefreshRangeCombo();
	//void BridgeTypeCtrl();
	void OnCmdSpanLengthEdit();
};

#endif // !defined(AFX_CMGRIDSPANLENGTHDLG_H__E511831D_EDF0_4AFC_A93D_55BC2D4DEF9B__INCLUDED_)
