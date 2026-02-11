#if !defined(__CMTENDONPROFILEDLG_H__)
#define __CMTENDONPROFILEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonProfileDlg.h : header file
//

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\wg_base\FoldGroupBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxTdnt.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_CobxTdgr.h"

#include "CMDlgBase.h"
#include "TdnaItemGrid.h"
#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileDlg dialog
using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMTendonProfileDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMTendonProfileDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetTDNA(T_TDNA_K TdnaKey);
	void SetParentDlg(CWnd *pParentDlg);
	void ExternalSetting(T_TDNA_K key);

// Dialog Data
	//{{AFX_DATA(CCMTendonProfileDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_PROFILE_ITEM_DLG };
	MComboBox	m_cobxPostTension;
	CCobxTdgr m_wndTdgrCobx;
	BOOL	      m_bUserDef; // 삭제 and 추가 
	//CMouseEdit		m_wndEndUser; //
	//CMouseEdit		m_wndBeginUser; //
	CEditUnit		m_wndEndUser; //
	CEditUnit		m_wndBeginUser; //
	CTextUnit     m_unitBeginUser;
	CTextUnit     m_unitEndUser;

	CEditUnit	m_wndDebondBegin; //
	CEditUnit	m_wndDebondEnd; //
	CTextUnit   m_uniDebondEnd;

	CMouseEdit	  m_editInsertPos;
	MEdit   	m_editXDir;
	CTextUnit	m_unitAngle;
	CFormulaEditSpin 	  m_editAngle;
// 	CSpinButtonCtrl	m_spinAngle;
	MButton	m_chkProjection2;
	CTextUnit	m_unitOffsetZ;
	CMouseEdit	m_editOffsetZ;
	CTextUnit	m_unitOffsetY;
	CMouseEdit	m_editOffsetY;
	MComboBox	m_cobxCurveDir;
	CTextUnit	m_txtGradUnit;
// 	CSpinButtonCtrl	m_wndGradSpin;
	CFormulaEditSpin	m_wndGradAngle;
	MComboBox	m_cobxGradAxis;
	MButton	m_chkProjection;
	CTextUnit	m_txtOffsetUnit;
	CMouseEdit	m_wndOffsetEdit;
	CTextUnit	m_txtCenterUnit;
	CMouseEdit	m_wndCenterEdit;
	CTextUnit	m_txtVectorUnit;
	CMouseEdit	m_wndVectorEdit;
	CTextUnit	m_txtAxisAngleUnit;
// 	CSpinButtonCtrl	m_wndAxisAngleSpin;
	CFormulaEditSpin m_wndAxisAngleEdit;
	CTextUnit	m_txtInsertPointUnit;
	CMouseEdit m_wndInsertPoint;
	CCobxTdnt	m_wndTdntCobx;
	CTextUnit	m_txtEndLengthUnit;
	CTextUnit	m_txtBeginLengthUnit;
	CMouseEdit	m_wndEndLength;
	CMouseEdit m_wndBeginLength;
	CSelectEdit m_wndAssignElem;
	MEdit	m_wndTendonName;

	MButton	m_chkTypicalTendon;
	CFormulaEditSpin	      m_wndTendonCountEdit;   // No. of Tendon 
//   CEdit	      m_wndTendonCountEdit;   // No. of Tendon 
	CSpinButtonCtrl m_wndTendonCountSpin;   // No. of Tendon 

	CFoldGroupBox_Cross	m_chkDetail;

//  BOOL  m_bBottomZ; //dlg에서 삭제 2005.10.25 jkpark
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseEditTextChanged(WPARAM wParam, LPARAM lParam);

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void OnUnitChange();
	void OnGridDataChanged(BOOL bSave=TRUE);
	void OnGridDataChanged2DXY(BOOL bSave=TRUE);
	void OnGridDataChanged2DXZ(BOOL bSave=TRUE);
	void OnGridDataChanged3D(BOOL bSave=TRUE);

	BOOL Conv2DPlanesTo3DPlane(CArray<T_NODE_D, T_NODE_D&>& aPos2DXY, CArray<T_NODE_D, T_NODE_D&>& aPos2DXZ, std::vector<T_NODE_D>& aPos3D);
	// Implementation
protected:
	void AlignControls();
	void AlignControls2(int type);
	BOOL DoApply();
	BOOL Dlg2Data();
	void Data2Dlg();
	void InitGrid();
	void InitChart();
	void InitGraph(UINT nPlaceHolderID, SREGraphView* pGraphView, CMySRGraph* pGraph);
	void AddGraphLabel(CMySRGraph *pGraph, int ix, int lp, double x, double y, LPCTSTR lpszLabel);
	void SetValue(CMySRGraph* pGraph, int nIndex, int nGroup, double x,double y);
	void SetAxisLabelText(CMySRGraph* pGraph, int nComponent, LPCTSTR lbl);
	void EnableDisableCtrl();
	void ChangeInputType();
	void UnselectAll();
	BOOL MakeSymmetricTondon();

	void CalcMinMaxRange_xy(CArray<T_NODE_D, T_NODE_D&> &aTendonProfile, 
													double &dMinRangeX, double &dMinRangeY, double &dMaxRangeX, double &dMaxRangeY);
	void CalcMinMaxRange_xz(CArray<T_NODE_D, T_NODE_D&> &aTendonProfile, 
													double &dMinRangeX, double &dMinRangeZ, double &dMaxRangeX, double &dMaxRangeZ);
	void ComboBoxDynamicChange(CComboBox* pCombo);


protected:
	CWnd* m_pParentDlg;
	T_TDNA_K m_TdnaKey;
	T_TDNA_D m_TdnaData;
	//T_TDGR_K m_TdgrKey;
	//T_TDGR_D m_TdgrData;
	CString  m_csOldTdnaName;
	CDialog* m_pModalDlg;
	BOOL     m_bCloseDlg;
	BOOL m_bIgnoreNotify;
	BOOL m_bExternalSetting;
	int m_nAutoOrUser;
	int m_nTendonType;
	BOOL m_bDebonded;

	SREGraphView m_GraphView3DXY;
	CMySRGraph m_Graph3DXY;
	SREGraphView m_GraphView3DXZ;
	CMySRGraph m_Graph3DXZ;

	CTdnaItemGrid m_wndGrid3D;
	CTdnaItemGrid m_wndGrid2DXY;
	CTdnaItemGrid m_wndGrid2DXZ;

	CArray<UINT, UINT> m_aCtrlTendonCount;
	CArray<UINT, UINT> m_aCtrlType2D;
	CArray<UINT, UINT> m_aCtrlType3D;
	CArray<UINT, UINT> m_aCtrlType2DTemp;
	CArray<UINT, UINT> m_aCtrlType3DTemp;
	CArray<UINT, UINT> m_aCtrlTypeRadio;
	CArray<UINT, UINT> m_aCtrlSymPtRadio;
	CArray<UINT, UINT> m_aCtrlTendonShape;
	CArray<UINT, UINT> m_aCtrlXAxisDir;
	CArray<UINT, UINT> m_aCtrlCommon;
	CArray<UINT, UINT> m_aCtrlStraight;
	CArray<UINT, UINT> m_aCtrlCurve;
	CArray<UINT, UINT> m_aCtrlElement;
	CArray<UINT, UINT> m_aCtrlVector;
	CArray<UINT, UINT> m_aCtrlCurveType;
	CArray<UINT, UINT> m_aCtrlBeginEndLength;
	CArray<UINT, UINT> m_aCtrlXAxisInsertPos;
	CArray<UINT, UINT> m_aCtrlXAxisDirection;

	// Generated message map functions
	//{{AFX_MSG(CCMTendonProfileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnCmdTendonPropButton();
	afx_msg void OnCmdTendonGroupButton(); 
	afx_msg void OnChangeTendonShape();
	afx_msg void OnChangeDir();
	afx_msg void OnChangeCurveType();
	afx_msg void OnChangeProp();
	afx_msg void OnCmdTendonSymmetricButton();
	afx_msg void OnChangeCmdInsertposEdit();
	afx_msg void OnCmdInputTypeRadio();
	afx_msg void OnSelchangeCmdTendonGroupCombo();
	afx_msg void OnSelchangeCmdTendonPropPostTension();

// 	afx_msg void OnDeltaposCmdTendonCountSpin(NMHDR* pNMHDR, LRESULT* pResult);   // No. of Tendon 
	afx_msg void OnCmdTypicalTendonCheck();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnChangeBeginEndLength();
//	afx_msg void OnKillfocusCmdBeginLengthEdit();
//	afx_msg void OnKillfocusCmdEndLengthEdit();
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMTENDONPROFILEDLG_H__)
