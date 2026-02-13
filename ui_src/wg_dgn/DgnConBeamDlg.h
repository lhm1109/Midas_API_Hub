#if !defined(AFX_DGNCONBEAMDLG_H__5D7AB3A3_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONBEAMDLG_H__5D7AB3A3_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnBeamGridWnd.h"
#include "DgnSelSectBase.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlg dialog

class CDgnConBeamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConBeamDlg)
	enum { IDD = IDD_DGN_CON_BEAM_DLG };
	CEditUnit	m_SjCtrl;
	CEditUnit	m_ScCtrl;
	CEditUnit	m_SiCtrl;
	CComboBox	m_StirrupRebar;
	CEditUnit	m_Dt;
	CEditUnit	m_Db;
	CEditUnit	m_Hf;
	CEditUnit	m_Hc;
	CEditUnit	m_Bf;
	CEditUnit	m_Bc;
	CTextUnit	m_DbUnit;
	CTextUnit	m_DtUnit;
	CTextUnit	m_HfUnit;
	CTextUnit	m_HcUnit;
	CTextUnit	m_BfUnit;
	CTextUnit	m_BcUnit;
	int		m_nOption;
	CString	m_Shape;
 	int		m_nCrack;
	int		m_nDeflect;
	CComboBox	m_SideRebar;
	CEditUnit m_SideNum;
	//}}AFX_DATA

public:
	T_RCHK_K m_RchkKey;
	T_REBB_D m_RchkData;
	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 

	CDgnBeamGridWnd m_wndGrid;
	CDgnSelSectBase m_SelectGrid;

	double m_StirrupSpaceI,m_StirrupSpaceC,m_StirrupSpaceJ;
	int m_iStirrupNumI, m_iStirrupNumC, m_iStirrupNumJ;
	CArray<UINT, UINT> m_aTobeMovedCtrl;
	// Coded by Seungjun MNet:No.2393 ('20060830)
	CArray<UINT, UINT> m_aDeflectMovedCtrl;

public:
	
	void Initial_ExistData();
	void Initial_WithoutData();
	void Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa);
	void Write_GridTitle();
	void Set_SectionShape(CString strShape);
	void Set_ShowTSection();
	void Set_ShowSBSection();

	void Initial_GridWnd();
	void Initial_Unit();
	void ChangeDlgItemInfo();//Add By RSH 2002.12.13
	// Coded by Seungjun MNet:No.2393 ('20060830)
	void AlignControls(int iEnvNum, BOOL bWhich);

	void ShowBitmap(CString str);
	
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
	//{{AFX_VIRTUAL(CDgnConBeamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	CString GetDgnConcCode();

	void SetSideRebarControl();

	int m_iDeflectEnvNum;
	int m_iCrackEnvNum;
	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamDlg)
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
