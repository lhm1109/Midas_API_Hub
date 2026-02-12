#if !defined(AFX_CMMVHLITEMUSRPERMITDLG_H__3ECCDD24_FF3B_49DF_A932_B578B52ED4AD__INCLUDED_)
#define AFX_CMMVHLITEMUSRPERMITDLG_H__3ECCDD24_FF3B_49DF_A932_B578B52ED4AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrPermitDlg.h : header file
//

class CDBDoc;
struct T_MVHL_D;
struct T_MVHL_AXLE_TYPE;

#include "CMPermitLoadGrid.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#define D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE  0
#define D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD      1

#define D_MVHL_INDIA_IRC6_2000                0
#define D_MVHL_INDIA_IRS_BRIDGE_RULES         1

class CCMAxleTypeGrid : public CGXGridWnd
{
public:
	CCMAxleTypeGrid();
	virtual ~CCMAxleTypeGrid();
public:
	void Initialize();
protected:
	void SetHeaderTitle();
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	BOOL ValidateField(CString value, int nColID);
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrPermitDlg dialog

class CCMMvhlItemUsrPermitDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrPermitDlg(CWnd* pParent = NULL);   // standard constructor
	enum { CHANGE_TYPE=1, CLOSE_DLG };
public:
	int GetNextAction() { return m_nNextAction; }
	int GetType();
	int GetBSType()          { return m_nBSType;          }
	int GetEuroBSType()      { return m_nEuroBSType;      }
	int GetSouthAfricaType() { return m_nSouthAfricaType; }
	int GetIndiaType()       { return m_nIndiaType;       }
	int GetASType()          { return m_nASType;      }
	int GetCodeType() { return m_nCodeType; }
	CRect GetLastWindowPos() { return m_WindowPos; }
	void SetModifyData(BOOL bModify, LPCTSTR lpszOldName, T_MVHL_D* pData, CDBDoc* pDoc, CString* paRadioText, int nMovingType) 
	{ 
		m_bModify = bModify;
		m_csOldName = lpszOldName;
		m_pDoc = pDoc; m_pData = pData; m_paRadioText = paRadioText; 
		m_nMovingType = nMovingType;
	}

protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL Apply();
	void AxleType2Dlg(T_MVHL_AXLE_TYPE* pData);
	BOOL Dlg2AxleType(T_MVHL_AXLE_TYPE* pData);
	
	void InitGrid();
	void InitCombo();
	void ChangeCombo();
	void AlignControl();
	void ShowHideControls();
	void ResizeDialog();
	void AdjustComboListBox(CComboBox& Cbx);


protected:
	CArray<UINT, UINT> m_aCtrlLoadType;
	CArray<UINT, UINT> m_aCtrlBSLoadType;
	CArray<UINT, UINT> m_aCtrlEuroBSLoadType;
	CArray<UINT, UINT> m_aCtrlSouthAfricaLoadType;
	CArray<UINT, UINT> m_aCtrlIndiaType;
	CArray<UINT, UINT> m_aCtrlVehiProp;
	CArray<UINT, UINT> m_aCtrlASLoadType;
	int m_nNextAction;
	int m_nType;
	int m_nBSType;
	int m_nEuroBSType;
	int m_nSouthAfricaType;
	int m_nIndiaType;
	int m_nASType;

	int m_nMovingType;
	int m_nCodeType;
	CRect m_WindowPos;

protected:
	CDBDoc* m_pDoc;
	BOOL m_bModify;
	CString m_csOldName;
	T_MVHL_D* m_pData;
	CString* m_paRadioText;

protected:
	CCMAxleTypeGrid m_grdAxleType;
	CCMPermitLoadGrid m_grdPermitLoad;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrPermitDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_PERMIT };
	MComboBox	m_wndCodeName;
	MEdit	m_edtAxleName;
	CListBox	m_lstAxleName;
	MButton	m_chkAxleSymm;
	MButton	m_chkAxleEven;
	MEdit	m_edtVehicleName;
	CEditUnit m_edtImpactFactor;
	MComboBox m_cmbPermitStdName;
	MComboBox m_cmbPermitLoadName;
	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrPermitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnSelChangeCmbPermitStdName();
	void OnSelChangeCmbPermitLoadName();
	void InitWithBonusVehicle(int nStd, int nLoad, T_MVHL_PERMIT_LOAD& PermitLoad);

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrPermitDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCmdMvhlCodeCmb();
	afx_msg void OnCmdApply();
	virtual void OnCancel();
	afx_msg void OnCmdMlMvhlItemuTrkRd();
	afx_msg void OnCmdMlMvhlItemuBSRdo();
	afx_msg void OnCmdMlMvhlItemuEuroBSRdo();
	afx_msg void OnCmdMlMvhlItemuSouthAfricaRdo();
	afx_msg void OnCmdMlMvhlItemuIndiaRdo();
	afx_msg void OnCmdMlMvhlItemuASRdo();
	afx_msg void OnCmdAxleEvenChk();
	afx_msg void OnCmdAxleSymmChk();
	afx_msg void OnCmdAddBtn();
	afx_msg void OnCmdModifyBtn();
	afx_msg void OnCmdDeleteBtn();
	afx_msg void OnSelchangeCmdAxleNameLst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRPERMITDLG_H__3ECCDD24_FF3B_49DF_A932_B578B52ED4AD__INCLUDED_)
