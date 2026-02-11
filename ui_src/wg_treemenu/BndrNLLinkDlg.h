#if !defined(AFX_BNDRNLLINKDLG_H__EB208C20_3010_4B39_B578_FA9F6AF56967__INCLUDED_)
#define AFX_BNDRNLLINKDLG_H__EB208C20_3010_4B39_B578_FA9F6AF56967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrNLLinkDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_db\wg_db_CobxNllp.h"
#include "..\wg_db\wg_db_CobxIehp.h"
#include "..\wg_db\SelectCtrl.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CBndrNLLinkDlg dialog

class CBndrNLLinkDlg : public CMenuBarChildDlg  , public CDBUpdateConnector
{
// Construction
public:
	CBndrNLLinkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBndrNLLinkDlg();
	virtual void Execute();
#if defined(_MEC)
#else
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
#endif
	void CtrlMan();
	void CopyOptCtrlMan();
	void DisplayCurNllp();
	BOOL ExternalInit(UINT key);


	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CBndrNLLinkDlg)
	enum { IDD = IDD_TM_BNDR_NONLIN };
	CTextUnit	m_wndDistUnit;
	CTextUnit	m_wndDistUnit2;
	CTextUnit	m_wndAngleUnit;
	CTextUnit	m_wndVectorUnit;
	CTextUnit	m_wndSelect1NodeUnit;
	CTextUnit	m_wndSelectEFrameUnit;
	CTextUnit	m_wndSelectEFrameUnit2;
	MEdit	m_wndNllp;
	CCobxNllp	m_wndNllpCobx;
	MComboBox	m_wndIehpCobx;
	MEdit	m_wndDistEdit;
	MComboBox	m_wndBetaAngle;
	CCobxBngr	m_wndGroupCombo;
	CSelectEdit	m_wnd1Node;
	CSelectEdit	m_wndEFrame;
	CMouseEdit	m_wnd2Node;
	CMouseEdit	m_wndRefPointEdit;
	CMouseEdit	m_wndDirVectorEdit;
	CEditUnit	m_wndSelect1NodeEdit;
	CEditUnit	m_wndSelectEFrameDEdit;
	CEditUnit	m_wndSelectEFrameLEdit;
	int		m_nOption;
	BOOL	m_bCopyNLLink;
	int		m_nCopyAxis;
	CFormulaEditSpin m_editNtimes;
	CFormulaEditSpin m_editNodeInc;
	int m_nRefCoord;
	int m_nElementCoord;
	int m_nGlobalCoord;
	int m_nSelectType;//GEN_CH:Select 1 Node & 2 Node
	BOOL	m_bIehp;
	//}}AFX_DATA
	mit::frx::MComboBox  AngleCobx[3];
	CMouseEdit Pt3Edit[3];
	CTextUnit  Pt3Unit[3];
	CMouseEdit VtEdit[2];

	mit::frx::MillustViewer m_wndPicture;

	CArray<UINT, UINT> m_aCtrlCopyLoad;
	CArray<UINT, UINT> m_aCtrlCopyDist;
	CArray<UINT, UINT> m_aCtrlCopyNodeInc;
	CArray<UINT, UINT> m_aCtrlCopyType;
	CArray<UINT, UINT> m_aCtrlElementCoord;
	CArray<UINT, UINT> m_aCtrlElementAngle;
	CArray<UINT, UINT> m_aCtrlElementPoint;
	CArray<UINT, UINT> m_aCtrlElementVector;
	CArray<UINT, UINT> m_aCtrlGlobalCoord;
	CArray<UINT, UINT> m_aCtrlGlobalAngle;
	CArray<UINT, UINT> m_aCtrlGlobalPoint;
	CArray<UINT, UINT> m_aCtrlGlobalVector;
	CArray<UINT, UINT> m_aCtrlSelectNodeType;
	CArray<UINT, UINT> m_aCtrlSelectNodeTypeCH;
	CArray<UINT, UINT> m_aCtrlSelectNodeTypeCH_Move;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrNLLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);


// Implementation
protected:
	void AlignControl();
	void ChangeBitmap();
	BOOL GetBetaAngle(double& dBetaAngle, CArray<int, int>& aNodes);
	void MakeIehpCombo();
	BOOL ChangeSelectIehp(UINT key);
	void ShowHideStartLinkNumCtrl();

	void TmSelect1Node();
	void TmSelectElemFrame();
	// Generated message map functions
	//{{AFX_MSG(CBndrNLLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmOption();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnTmCopyLoad();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnSelchangeTmNlLinkPropCombo();
	afx_msg void OnTmNlLinkPropButton();
	afx_msg void OnTmCopyType();
	afx_msg void OnTmSelectNodeType();
	afx_msg void OnCoordRdo();
	afx_msg void OnGlobalCoordRdo();
	afx_msg void OnElementCoordRdo();
	afx_msg void OnTmIeHingePropButton();
	afx_msg void OnIehpChk();
	afx_msg void OnViewStartLinkNum();
	afx_msg void OnEFrameDChk();
	afx_msg void OnEFrameLChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRNLLINKDLG_H__EB208C20_3010_4B39_B578_FA9F6AF56967__INCLUDED_)
