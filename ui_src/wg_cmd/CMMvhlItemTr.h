#if !defined(AFX_CMMVHLITEMTR_H__A33F56C7_6553_4C7B_A835_C91051791513__INCLUDED_)
#define AFX_CMMVHLITEMTR_H__A33F56C7_6553_4C7B_A835_C91051791513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemTr.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemTr dialog

class __MY_EXT_CLASS__ CCMMvhlItemTr : public CDialogMove
{
// Construction
public:
	CCMMvhlItemTr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemTr)
	enum { IDD = IDD_CMD_ML_MVHL_ITEM_TR };
	CTextUnit	m_trWidthUnit;
	CTextUnit	m_trWheelLoadUnit;
	CTextUnit	m_trNumLanesUnit;
	CTextUnit	m_trMinDistUnit;
	CTextUnit	m_trLocationUnit;
	CTextUnit	m_trEdgeDistUnit;
	CTextUnit	m_trDistWidthUnit;
	CTextUnit	m_trLongWidthUnit; // MNET:XXXX 20071005 mylee
	CTextUnit	m_trDistWheelsUnit;
	CEditUnit	m_trEdgeDist;
	CEditUnit	m_trDistWheels;
	CEditUnit	m_trDistWidth;
	CEditUnit	m_trLongWidth; // MNET:XXXX 20071005 mylee
	CEditUnit	m_trLocation;
	CEditUnit	m_trMinDist;
	MEdit		m_trMvhlName;
	CFormulaEditSpin	m_trNumLanes;
	CEditUnit	m_trWheelLoad;
	CEditUnit	m_trWidth;
	CFormulaEditSpin m_edtLeftLaneNum;
	CEditUnit m_edtRightLaneNum;
	MButton   m_chkMedianStrip;
	MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemTr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemTr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnChangeCmdNumLeftLanesEdt();
	afx_msg void OnChangeCmdNumLanesText();
	afx_msg void OnMedianStripCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc*   m_pDoc;
	T_MVHLtr_D  m_Data;
	int m_nStndCode;
	CString   m_csOldName;
	BOOL m_bModify;
	BOOL      m_bInit;
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
public:
	void SetParamData(T_MVHLtr_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMTR_H__A33F56C7_6553_4C7B_A835_C91051791513__INCLUDED_)
