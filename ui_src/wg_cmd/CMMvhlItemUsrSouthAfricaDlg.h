#if !defined(__CMMVHLITEMUSRSOUTHAFRICADLG_H__)
#define __CMMVHLITEMUSRSOUTHAFRICADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrSouthAfricaDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemSouthAfricaLoadGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrSouthAfricaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemUsrSouthAfricaDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrSouthAfricaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemUsrSouthAfricaDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void DefaultSetting();
	void InitUnit();
	void InitControls();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void ChangeBitmap(int nBitmap);

	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();

protected:
	
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	CArray<UINT, UINT> m_aCtrl_NA;
	CArray<UINT, UINT> m_aCtrl_NB;
	CArray<UINT, UINT> m_aCtrl_NC;
	CArray<UINT, UINT> m_aCtrl_NBNC;
	CArray<UINT, UINT> m_aCtrl_OKCancel;

	CCMMvhlItemSouthAfricaLoadGrid m_NCGrid[3];

	int m_bPermitLoadCall;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrSouthAfricaDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_SOUTH_AFRICA };

	int       m_nSubType;
	CEditUnit m_edtVehicleName;
	CEditUnit m_edtNAW1;
	CTextUnit m_untNAW1;
	CEditUnit m_edtNAL1;
	CTextUnit m_untNAL1;
	CEditUnit m_edtNAW21;
	CEditUnit m_edtNAW22;
	CTextUnit m_untNAW2;
	CEditUnit m_edtNAL2;
	CTextUnit m_untNAL2;
	CEditUnit m_edtNAPa;
	CTextUnit m_untNAPa;
	BOOL      m_bNAIncreLength;
	CEditUnit m_edtNAIncreLength;
	CTextUnit m_untNAIncreLength;
	CEditUnit m_edtNBPb;
	CTextUnit m_untNBPb;
	CEditUnit m_edtNBNumUnit;
	CEditUnit m_edtNBd;
	CTextUnit m_untNBd;
	CEditUnit m_edtNBD1;
	CTextUnit m_untNBD1;
	CEditUnit m_edtNBD2;
	CTextUnit m_untNBD2;
	CEditUnit m_edtNBD3;
	CTextUnit m_untNBD3;
	CEditUnit m_edtNBD4;
	CTextUnit m_untNBD4;
	CEditUnit m_edtNBD5;
	CTextUnit m_untNBD5;
	CEditUnit m_edtNBD6;
	CTextUnit m_untNBD6;
	CEditUnit m_edtNCW;
	CTextUnit m_untNCW;
	CFormulaEditSpin m_edtNCNum[3];
	BOOL      m_bNCOpposite;
	CEditUnit m_edtNBNCP;
	CTextUnit m_untNBNCP;
	CEditUnit m_edtNBNCD1;
	CTextUnit m_untNBNCD1;
	CEditUnit m_edtNBNCD2;
	CTextUnit m_untNBNCD2;
	CEditUnit m_edtNBNCD3;
	CTextUnit m_untNBNCD3;
	CEditUnit m_edtNBNCD4;
	CTextUnit m_untNBNCD4;
	CEditUnit m_edtNBNCD5;
	CTextUnit m_untNBNCD5;
	CEditUnit m_edtNBNCD6;
	CTextUnit m_untNBNCD6;
	CEditUnit m_edtNBNCW1;
	CTextUnit m_untNBNCW1;
	CEditUnit m_edtNBNCW2;
	CTextUnit m_untNBNCW2;
	CEditUnit m_edtNBNCDD1W;
	CTextUnit m_untNBNCDD1W;
	CEditUnit m_edtNBNCDD2W;
	CTextUnit m_untNBNCDD2W;
	CEditUnit m_edtNBNCDD1L;
	CTextUnit m_untNBNCDD1L;
	CEditUnit m_edtNBNCDD2L;
	CTextUnit m_untNBNCDD2L;
	CEditUnit m_edtNBNCDD3L;
	CTextUnit m_untNBNCDD3L;
	CEditUnit m_edtNBNCWidth;
	CTextUnit m_untNBNCWidth;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrSouthAfricaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrSouthAfricaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlSubTypeRdo();
	afx_msg void OnCmdMvhlChangeL1Edt();
	afx_msg void OnCmdMvhlIncreLengthChk();
	afx_msg void OnCmdMvhlChangeDistEdt();
	afx_msg void OnCmdMvhlChangeNCNumaEdt();
	afx_msg void OnCmdMvhlChangeNCNumbEdt();
	afx_msg void OnCmdMvhlChangeNCNumcEdt();
	afx_msg void OnCmdMvhlApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMUSRSOUTHAFRICADLG_H__)
