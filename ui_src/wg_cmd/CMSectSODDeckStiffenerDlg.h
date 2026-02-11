#if !defined(__CMSECTSODDECKSTIFFENERDLG_H__)
#define __CMSECTSODDECKSTIFFENERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectSODDeckStiffenerDlg.h : header file
//

#include "CMSecViewWnd.h"
#include "CMSectSODDeckStiffenerGrid.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerDlg dialog
class CCMSectSODDeckStiffenerDlg : public CDialogMove
{
// Construction
public:
	CCMSectSODDeckStiffenerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectSODDeckStiffenerDlg();

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitUnit();
	void InitCombo();
	void InitGrid();
	void InitView();
	void InitSpin();

	void ChangeBitmap();
	void ChangeCombo();

	void ChangeDeckName();

	void ChangeText();

	void SetHeaderTitle_DefStiff();
	void MakeItemEx_DefStiff();
	BOOL InsertItem_DefStiff(int nIndex, T_STIFF_SHAPE_D &Data);
	CString DataToStr_DefStiff(int i, T_STIFF_SHAPE_D &Data);
	void CopytoPos(BOOL bCopyMirror, CArray<int, int>& arDeckPosSel, T_SECT_SOD_STIFF_SUB_D& StiffSubD);
	void CopytoPart(BOOL bCopyMirror, CArray<int, int>& arDeckPartSel, T_SECT_SOD_STIFF_SUB_D& StiffSubD);

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data);
	BOOL DeleteItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data);
	BOOL ModifyItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data);
	CString DataToStr(int i, T_SECT_SOD_STIFF_SUB_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void Data2Dlg_StiffSub(T_SECT_SOD_STIFF_SUB_D &Data);
	BOOL Dlg2Data_StiffSub(T_SECT_SOD_STIFF_SUB_D &Data);

	BOOL ErrorCheck_Stiffener(T_SECT_SOD_STIFF_SUB_D &Data, BOOL bAdd);


	CCMSecViewWnd m_wndSecView;
	MillustViewer m_ImgStiffenerItem;

	CBitmap*	m_pSectionBmp;

	CCMSectSODDeckStiffenerGrid* m_pGrid;
	
public:
	void UpdateSectView();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectSODDeckStiffenerDlg)
	enum { IDD = IDD_CMD_SECT_SOD_DECK_STIFFENER_DLG };
	
	CListCtrl m_lstDefStiff;
	MComboBox m_cmbDeckPos;
	MComboBox m_cmbDeckPart;
	CEditUnit m_edtDeckName;
	int       m_nRefPos;
	CFormulaEditSpin m_edtNumber;
	CListCtrl m_lstStiffener;
	//}}AFX_DATA

	T_SECT_D m_Data;
	BOOL m_bTapJ; // 이번 버전에는 없는데 나중에 필요하면 추가 한다
	int m_nDeckPos; // Deck Pos Combo 변경시 Defailt Setting 시에 사용하는 변수 (MQC Alpha/Beta : 4870-23)
	int m_nDeckPart; // Deck Pos Combo 변경시 Defailt Setting 시에 사용하는 변수

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectSODDeckStiffenerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectSODDeckStiffenerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCMSectSODDeckPosCmb();  

	void SetDefaultStiffPos(DWORD nDeckPos, int nDeckPart, CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D &> &aStiffData);

	afx_msg void OnCMSectSODDeckPartCmb();
	afx_msg void OnCMSectSODDefineStiffenerBtn();
	afx_msg void OnCMSectSODRefPosRdo();
	afx_msg void OnCMSectSODNumberEdt();  
	afx_msg void OnCMSectSODAddBtn();
	afx_msg void OnCMSectSODModBtn();
	afx_msg void OnCMSectSODDelBtn();
	afx_msg void OnCMSectSODCopyBtn();
	afx_msg void OnDblclkDefStiffList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCMItemchangedSODList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSODDECKSTIFFENERDLG_H__)
