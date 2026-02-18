#if !defined(__DgnMeshBarLThk4SandwichDlg_H__)
#define __DgnMeshBarLThk4SandwichDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMeshBarLThk4SandwichDlg.h : header file
//

#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\InternationalDlg.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include <list>

struct LTHIK_4_SANDWICH
{
	BOOL    bThickOptimal; 
	double  dTopThik;        
	double  dBotThik;        
	int     nIteration;    
	double  dConvTolerance;
};

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarLThk4SandwichDlg dialog
using namespace mit::frx;
class CSWGirderSectSingleBeamImageDlg;
class CDgnMeshBarLThk4SandwichDlg : public CInternationalDlg
{
// Construction
public:
	CDgnMeshBarLThk4SandwichDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetData(LTHIK_4_SANDWICH& Data);
	void GetData(LTHIK_4_SANDWICH& Data);

protected:
// Dialog Data
	//{{AFX_DATA(CDgnMeshBarLThk4SandwichDlg)
	enum { IDD = IDD_DGN_MESH_BAR_LTHK_SAND_DLG };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMeshBarLThk4SandwichDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// 상속
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// 컨트롤
	// Generated message map functions
	//{{AFX_MSG(CDgnMeshBarLThk4SandwichDlg)
	afx_msg void OnChkAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// 일반
	void SetControl();
	void Data2Dlg();
	BOOL Dlg2Data();
	void EnableDisableControls();
	void ShowHideControls();
	void ChangeCaption();
			
	
	// 컨트롤				
	MButton m_chkAuto;	
	CFormulaEditSpin m_edtItr;	
	CEditUnit m_edtTop;
	CEditUnit m_edtBot;
	CEditUnit m_edtTol;

// 일반	
	LTHIK_4_SANDWICH m_Data;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnMeshBarLThk4SandwichDlg_H__)

