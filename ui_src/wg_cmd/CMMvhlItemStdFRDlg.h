#if !defined(__CMMVHLITEMSTDFRDLG_H__)
#define __CMMVHLITEMSTDFRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdFRDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemStdFRGrid.h"
#include "CMMvhlItemStdFRNominalGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdFRDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdFRDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdFRDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdFRDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:

	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap(int nIndex, int nBitmap);
	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();
	BOOL ChangeData();
	
	void SetLoadTypeList(int nIndex);
	void  GetIndex(CString &csName, int *sitem=NULL);  

protected:  

	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer  m_wndPicture;
	CArray<UINT, UINT> m_aPicture;     // Picture
	CArray<UINT, UINT> m_aList;        // List

	CArray<UINT, UINT> m_aDist;    
	CArray<UINT, UINT> m_aCoeff;
	CArray<UINT, UINT> m_aCoeff2;
	CArray<UINT, UINT> m_aMilitaryTail;
	CArray<UINT, UINT> m_aMilitary, m_aMilitaryP, m_aMilitaryD, m_aMilitaryD1;
	CArray<UINT, UINT> m_aSidewalk;
	CArray<UINT, UINT> m_aDyFactor;

	CArray<UINT, UINT> m_aEtc;

	CCMMvhlItemStdFRGrid        m_wndCoeffGrid;
	CCMMvhlItemStdFRNominalGrid m_wndNominGrid;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdFRDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_FR };
	
	MComboBox m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox m_cmbVehicleType;
	CListCtrl m_List;

	CEditUnit m_edtTailDist;
	CTextUnit m_untTailDist;

	CEditUnit m_edtP;
	CTextUnit m_untP;

	CEditUnit m_edtD;
	CTextUnit m_untD;

	CEditUnit m_edtD1;
	CTextUnit m_untD1;

	CEditUnit m_edtW;
	CTextUnit m_untW;

	BOOL      m_bDynamicFactor;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdFRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdFRDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const;
	CString ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const;

	CString GetVehicleTypeStr() const;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDFRDLG_H__)
