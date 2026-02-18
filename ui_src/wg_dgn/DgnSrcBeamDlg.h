#if !defined(AFX_DGNSRCBEAMDLG_H__5D7AB3A3_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSRCBEAMDLG_H__5D7AB3A3_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSrcBeamDlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnBeamGridWnd.h"
#include "DgnSelSectBase.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnSrcBeamDlg dialog

class CDgnSrcBeamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnSrcBeamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnSrcBeamDlg)
	enum { IDD = IDD_DGN_SRC_BEAM_DLG };
	CEditUnit	m_SjCtrl;
	CEditUnit	m_ScCtrl;
	CEditUnit	m_SiCtrl;
	mit::frx::MComboBox	m_StirrupRebar;

	CEditUnit	m_Dt;
	CEditUnit	m_Db;

	CEditUnit	m_Tw;
	CEditUnit	m_Tf;
	CEditUnit	m_Tf2;
	CEditUnit	m_Hc;
	CEditUnit	m_Cw;
	CEditUnit	m_Bc;
	CEditUnit	m_B;
	CEditUnit	m_H;
	CTextUnit	m_DbUnit;
	CTextUnit	m_DtUnit;
	CTextUnit	m_TwUnit;
	CTextUnit	m_TfUnit;
	CTextUnit	m_Tf2Unit;
	CTextUnit	m_HUnit;
	CTextUnit	m_HcUnit;
	CTextUnit	m_CwUnit;
	CTextUnit	m_BUnit;
	CTextUnit	m_BcUnit;

	int		m_nOption;

	mit::frx::MillustViewer m_wndSrcBeamRHB;
	//}}AFX_DATA

public:
	T_RCHK_K m_RchkKey;
	T_RBSB_D m_RchkData;
	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 

	CDgnBeamGridWnd m_wndGrid;
	CDgnSelSectBase m_SelectGrid;

	double m_StirrupSpaceI,m_StirrupSpaceC,m_StirrupSpaceJ;
	int m_iStirrupNumI, m_iStirrupNumC, m_iStirrupNumJ;

public:
	
	void Initial_ExistData();
	void Initial_WithoutData();
	void Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa);
	void Write_GridTitle();

	void Initial_GridWnd();
	void Initial_Unit();

	void Write_GridItem(CString Type,CString strNt1,CString strNt2,CString strNb1,CString strNb2);

	BOOL ErrorCheck();

	void SetData2Dlg();
	BOOL SetDlg2Data();
	void ChangeSectionInfo(); 
	void SelectElemOnView();
	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcBeamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcBeamDlg)
	afx_msg void OnPaint();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnBeamDel();
	afx_msg void OnDgnBeamAr();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnConcInode();
	afx_msg void OnDgnConcJnode();
	afx_msg void OnDgnConcMiddle();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnBeamSelectAll();
	afx_msg void OnDgnBeamUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMDLG_H__5D7AB3A3_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
