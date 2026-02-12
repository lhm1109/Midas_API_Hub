#if !defined(AFX_NODEPROJECTDLG_H__BAFE50C9_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_NODEPROJECTDLG_H__BAFE50C9_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeProjectDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CNodeProjectDlg dialog
class _NodeProject
{
public:
	int m_bCopyOrMove;
	int m_nProjectionType;

	// line
	double m_nLinePt1WX, m_nLinePt1WY, m_nLinePt1WZ;
	double m_nLinePt2WX, m_nLinePt2WY, m_nLinePt2WZ;
	// plane
	double m_nPlanePt1WX, m_nPlanePt1WY, m_nPlanePt1WZ;
	double m_nPlanePt2WX, m_nPlanePt2WY, m_nPlanePt2WZ;
	double m_nPlanePt3WX, m_nPlanePt3WY, m_nPlanePt3WZ;
	// cylinder
	double m_nCylPt1WX, m_nCylPt1WY, m_nCylPt1WZ;
	double m_nCylPt2WX, m_nCylPt2WY, m_nCylPt2WZ;
	double m_nCylRadius;
	// cone
	double m_nConePt1WX, m_nConePt1WY, m_nConePt1WZ;
	double m_nConePt2WX, m_nConePt2WY, m_nConePt2WZ;
	double m_nConeRadius1, m_nConeRadius2;
	// sphere
	double m_nSphrOrgWX, m_nSphrOrgWY, m_nSphrOrgWZ;
	double m_nSphrRadius;
	// ellipsoid
	double m_nElipOrgWX, m_nElipOrgWY, m_nElipOrgWZ;
	double m_nElipPt1WX, m_nElipPt1WY, m_nElipPt1WZ;
	double m_nElipRadius;
	// parabolic plane
	double m_nParaPlnPt1WX, m_nParaPlnPt1WY, m_nParaPlnPt1WZ;
	double m_nParaPlnPt2WX, m_nParaPlnPt2WY, m_nParaPlnPt2WZ;
	double m_nParaPlnPt3WX, m_nParaPlnPt3WY, m_nParaPlnPt3WZ;
	// element
	T_ELEM_K m_nElemKey;

	// Direction
	int m_nDirection;
	double m_nDirPt1WX, m_nDirPt1WY, m_nDirPt1WZ;

	BOOL m_bMergeNode;
	BOOL m_bCopyAttr;
	BOOL m_bIntersect;

	// Divide
	int m_nDivideType;
	int m_nDivide; 
	CArray<double, double> m_arDistance;

	// ucs vector
	double v1[3], v2[3], v3[3];
};

class CNodeProjectDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeProjectDlg(CWnd* pParent = NULL);   // standard constructor
	~CNodeProjectDlg();
	virtual void Execute();

	BOOL GetParameter(_NodeProject& NP);
	BOOL ProjectNode(_NodeProject& NP);

// Dialog Data
	//{{AFX_DATA(CNodeProjectDlg)
	enum { IDD = IDD_TM_NODE_PROJECT };
	MillustViewer m_wndPicture;
	MButton	m_chkDivide;
	MEdit	m_edtRatioDivide;
	CFormulaEditSpin	m_edtNumDivide;
	CTextUnit	  m_wndDirPt1Unit;
	CMouseEdit	m_wndDirPt1;
	CMouseEdit	m_wndElemNo;
	CTextUnit	  m_wndElipPt1Unit;
	CMouseEdit	m_wndElipPt1;
	CTextUnit	  m_wndElipRadUnit;
	CMouseEdit	m_wndElipRad;
	CTextUnit	  m_wndElipOrgUnit;
	CMouseEdit	m_wndElipOrg;
	CTextUnit	  m_wndParaPlnPt3Unit;
	CMouseEdit	m_wndParaPlnPt3;
	CTextUnit	  m_wndParaPlnPt2Unit;
	CMouseEdit	m_wndParaPlnPt2;
	CTextUnit	  m_wndParaPlnPt1Unit;
	CMouseEdit	m_wndParaPlnPt1;
	CTextUnit	  m_wndSphrRadUnit;
	CMouseEdit  m_wndSphrRad;
	CTextUnit	  m_wndSphrOrgUnit;
	CMouseEdit	m_wndSphrOrg;
	CTextUnit	  m_wndConeRad2Unit;
	CMouseEdit  m_wndConeRad2;
	CTextUnit	  m_wndConeRad1Unit;
	CMouseEdit	m_wndConeRad1;
	CTextUnit	  m_wndConePt2Unit;
	CMouseEdit	m_wndConePt2;
	CTextUnit	  m_wndConePt1Unit;
	CMouseEdit	m_wndConePt1;
	CTextUnit	  m_wndCylRadUnit;
	CMouseEdit	m_wndCylRad;
	CTextUnit	  m_wndCylPt2Unit;
	CMouseEdit	m_wndCylPt2;
	CTextUnit	  m_wndCylPt1Unit;
	CMouseEdit	m_wndCylPt1;
	CTextUnit	  m_wndPlanePt3Unit;
	CMouseEdit	m_wndPlanePt3;
	CTextUnit	  m_wndPlanePt2Unit;
	CMouseEdit	m_wndPlanePt2;
	CTextUnit	  m_wndPlanePt1Unit;
	CMouseEdit	m_wndPlanePt1;
	CTextUnit	  m_wndLinePt2Unit;
	CMouseEdit	m_wndLinePt2;
	CTextUnit	  m_wndLinePt1Unit;
	CMouseEdit	m_wndLinePt1;
	mit::frx::MComboBox	  m_wndProjectType;
	int		m_nCopyOrMove;
	int		m_nDirection;
	BOOL	m_bMergeNode;
	BOOL	m_bCopyAttr;
	int		m_nDivideType;
	BOOL  m_bIntersect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CArray<UINT,UINT> m_aDivideCtrl;
	CArray<UINT,UINT> m_aDivideECtrl;
	CArray<UINT,UINT> m_aDivideUECtrl;

protected:
	CDBDoc* m_pDoc;
	CBitmap* m_pBitmap;
	CArray<UINT,UINT> m_aLineCtrl;
	CArray<UINT,UINT> m_aPlaneCtrl;
	CArray<UINT,UINT> m_aCylinderCtrl;
	CArray<UINT,UINT> m_aConeCtrl;
	CArray<UINT,UINT> m_aSphereCtrl;
	CArray<UINT,UINT> m_aEllipsoidCtrl;
	CArray<UINT,UINT> m_aElementCtrl;
	CArray<UINT,UINT> m_aDirPt2Ctrl;
	CArray<UINT,UINT> m_aParaPlnCtrl;
	CArray<UINT,UINT> m_aDisableOnMoveCtrl;

	void ChangeBitmap(int nBitmap);
	void AlignControl();
	void ChangeProjectionType(int nProjType);
	BOOL Get3Point(CMouseEdit& pt, double &Px, double &Py, double &Pz);

	// Generated message map functions
	//{{AFX_MSG(CNodeProjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeProjectType();
	afx_msg void OnChangeDirection();
	afx_msg void OnChangeCopyMove();
	afx_msg void OnTmTolerance();
	afx_msg void OnTmCopynodedata();
	afx_msg void OnChangeDivideType();
	afx_msg void OnDeltaposTmSpinNumDivision(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmChkDivide();
	afx_msg void OnTmBtnIntersect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEPROJECTDLG_H__BAFE50C9_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
