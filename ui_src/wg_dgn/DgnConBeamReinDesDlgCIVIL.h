#if !defined(__DGNCONBEAMREINDESDLGCIVIL_H__)
#define __DGNCONBEAMREINDESDLGCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamDgnDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "DgnSelSectBase.h"
#include "DgnSectionDrawWnd.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinDesDlgCIVIL dialog
using namespace mit::frx;
class CDBDoc;

class __MY_EXT_CLASS__ CDgnConBeamReinDesDlgCIVIL : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamReinDesDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { }

// Dialog Data
	//{{AFX_DATA(CDgnConBeamReinDesDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_BEAM_DES_DLG };               
	MButton	    m_btnDel;
	MButton	    m_btnAdd;
	CListCtrl	  m_wndList;

	CEditUnit m_SubNum;
	MComboBox	m_SubCB;
	CTextUnit	m_dTunit;
	CEditUnit	m_dT;
	CTextUnit	m_hfunit;
	CEditUnit	m_hf;
	CTextUnit	m_Hcunit;
	CEditUnit	m_Hc;
	CTextUnit	m_bfunit;
	CEditUnit	m_bf;
	CTextUnit	m_Bcunit;
	CEditUnit	m_Bc;
	CTextUnit	m_dBunit;
	CEditUnit	m_dB;	
	BOOL      m_bTorsionalBar;
	MComboBox m_cmbTorBar;
	BOOL      m_bBundledBar;
	CEditUnit	m_edtBundledBarNum;
	MComboBox m_cmbLongiBar;
	CEditUnit	m_edtLongiBarNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamReinDesDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_RDGN_K  m_RdgnKey;
	T_RDGN_D  m_RdgnData;
	T_SECT_D m_SectD;
	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 
	CMap<int,int,T_RDGN_K,T_RDGN_K> m_arRdgnKey;
	BOOL m_bModify;
	
	CDBDoc* m_pDoc;

	MillustViewer m_wndPictureRect;
	MillustViewer m_wndPictureTee;

	CDgnSectionDrawWnd     m_SectWnd;  // Veiw Window  

	void ChangeItem(int nIndex);
	void MakeItemEx();
	void SetHeaderTitle();
	void InitTabCtrl();
	int  FindInsertionPos(T_SECT_K nID);
	BOOL InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RDGN_D &RData);

	void SetInitStirCB();
	void SetInitUnit();
	void SetData2Dlg();
	void ShowRECTBitmap();
	void ShowTEEBitmap();
	void ShowHideSectCtrl(BOOL bShow);
	BOOL CheckInputData();
	void SetDlg2Data();
	void ChangeSectionInfo();
	void SelectElemOnView();
	//void UpdateBuffer();
	BOOL SelectSection(CArray<T_RDGN_K, T_RDGN_K>&  arRdgnKey);
	void CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult);
	void ControlsEnableDisable();

	//virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);

	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamReinDesDlgCIVIL)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnTorsionalBar();
	afx_msg void OnDgnBundledStirrups();
	afx_msg void OnDgnDelete();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONBEAMREINDESDLGCIVIL_H__)
