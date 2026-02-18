#if !defined(_DGNCONCOLMREINDESDLGCIVIL_H__)
#define _DGNCONCOLMREINDESDLGCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmDgnDlgCIVIL.h : header file
//

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "DgnSectGridWnd.h"
#include "DgnSectionDrawWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinDesDlgCIVIL dialog

#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
class __MY_EXT_CLASS__ CDgnConColmReinDesDlgCIVIL : public CDialogMove
{
// Construction
public:
	CDgnConColmReinDesDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {}

// Dialog Data
	//{{AFX_DATA(CDgnConColmReinDesDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_COLM_DES_DLG };
	MButton	    m_btnDel;
	MButton	    m_btnAdd;
	CListCtrl	  m_wndList;

	CEditUnit	m_barnum;
	MComboBox	m_StirrupCB;
	MComboBox	m_PropNoCB;
	CTextUnit	m_tunit;
	CEditUnit	m_t;
	CTextUnit	m_Hunit;
	CEditUnit	m_H;
	CTextUnit	m_bunit;
	CEditUnit	m_b;
	CTextUnit	m_Bunit;
	CEditUnit	m_B;
	CTextUnit	m_aunit;
	CEditUnit	m_a;
	CTextUnit	m_Dcunit;
	CEditUnit	m_Dc;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmReinDesDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_RDGN_K              m_RdgnKey;
	T_RDGN_D              m_RdgnData;
	T_SECT_D m_SectD;
	CMap<int,int,int,int> m_arColmData;   // Element 중 Concrete Beam의 Data 저장 
	CMap<int,int,T_RDGN_K,T_RDGN_K> m_arRdgnKey;

	CDBDoc*               m_pDoc;
	CArray<UINT, UINT>    m_BGroup, m_HGroup, m_tGroup, m_abGroup;
	CDgnSectionDrawWnd     m_SectWnd;  // Veiw Window  
	BOOL m_bModify;

	MillustViewer m_wndPictureRect;
	MillustViewer m_wndPictureRectSolid;
	MillustViewer m_wndPictureCirc;
	MillustViewer m_wndPictureCircSolid;
	MillustViewer m_wndPictureOcta;
	MillustViewer m_wndPictureOctaSolid;			
	MillustViewer m_wndPictureTrac;
	MillustViewer m_wndPictureTracSolid;
	MillustViewer m_wndPictureTracHalf;

	void SetInitStirCB();
	void SetInitUnit();
	void SetData2Dlg();
	void SetDlg2Data();
	BOOL CheckInputData();
	void ChangeSectionInfo();
	void ShowBitmap(CString& colType);
	void ShowHideControl(CString& colType);
	void SelectElemOnView();
	void SetControlText(BOOL bBox);

	void ChangeItem(int nIndex);
	void MakeItemEx();
	void SetHeaderTitle();
	int  FindInsertionPos(T_SECT_K nID);
	BOOL InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RDGN_D &RData);
	BOOL SelectSection(CArray<T_RDGN_K, T_RDGN_K>&  arRdgnKey);
	void CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult);

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmReinDesDlgCIVIL)
	virtual BOOL OnInitDialog();	
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnDelete();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_DGNCONCOLMREINDESDLGCIVIL_H__)
