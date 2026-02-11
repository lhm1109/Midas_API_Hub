#if !defined(__CMSECTIONSTIFFENERDLG_H__)
#define __CMSECTIONSTIFFENERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectStiffenerDlg.h : header file
//

#include "CMSecViewWnd.h"
#include "CMSectStiffenerGrid.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectStiffenerDlg dialog
class CCMSectStiffenerDlg : public CDialogMove
{
// Construction
public:
	CCMSectStiffenerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectStiffenerDlg();

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitUnit();
	void InitCombo();
	void InitGrid();
	void InitView();
	void InitSpin();
	void InitSectBitmap();

	void EnableDisableControls();
	void ControlsShowHide();
	void ChangeSizeText();
	void ChangeBitmap();

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_STIFF_SHAPE_D &Data);
	BOOL DeleteItem(int nIndex, T_STIFF_SHAPE_D &Data);
	BOOL ModifyItem(int nIndex, T_STIFF_SHAPE_D &Data);
	CString DataToStr(int i, T_STIFF_SHAPE_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void Data2Dlg_Shape(T_STIFF_SHAPE_D &rData);
	BOOL Dlg2Data_Shape(T_STIFF_SHAPE_D &rData);

	BOOL ErrorCheck_Shape(T_STIFF_SHAPE_D &ShapeD, CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aStiffShape, BOOL bModify, int nIndex);
	BOOL ErrorCheck_Stiffener(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nPos);

	void GetShapeList_by_Sect(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape);

	void GetStiffenerList_by_Sect(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nPos);

	CCMSecViewWnd m_wndSecView;
	MillustViewer m_ImgSectItem;
	MillustViewer m_ImgStiffenerItem;

	CBitmap*	m_pSectionBmp;
	CBitmap*	m_pStiffnerBmp;

	CCMSectStiffenerGrid* m_pGridN1;
	CCMSectStiffenerGrid* m_pGridN2;
	CCMSectStiffenerGrid* m_pGridN3;
	CCMSectStiffenerGrid* m_pGridN4;

public:
	void UpdateSectView(int nStiffPos);
	void UpdateFromLeftToRight();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectStiffenerDlg)
	enum { IDD = IDD_CMD_SECT_STIFFENER_DLG };

	CEditUnit m_edtName;
	MComboBox m_cmbType;
	CEditUnit m_edtSize1;
	CTextUnit m_untSize1;
	CEditUnit m_edtSize2;
	CTextUnit m_untSize2;
	CEditUnit m_edtSize3;
	CTextUnit m_untSize3;
	CEditUnit m_edtSize4;
	CTextUnit m_untSize4;
	CEditUnit m_edtSize5;
	CTextUnit m_untSize5;
	CListCtrl m_lstStiffener;
	CFormulaEditSpin m_edtN1Spin;
	CFormulaEditSpin m_edtN2Spin;
	CFormulaEditSpin m_edtN3Spin;
	CFormulaEditSpin m_edtN4Spin;
	int       m_nPosition;
	int       m_nRefD;

	//}}AFX_DATA

	T_SECT_D m_Data;
	BOOL m_bTapJ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectStiffenerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectStiffenerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCMSectStiffenerTypeCmb();
	afx_msg void OnCMSectStiffenerAddBtn();
	afx_msg void OnCMSectStiffenerModBtn();
	afx_msg void OnCMSectStiffenerDelBtn();
	afx_msg void OnCMItemchangedStiffenerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCMSectStiffenerPosRdo();
	afx_msg void OnCMSectStiffenerN1Edt();
	afx_msg void OnCMSectStiffenerN2Edt();
	afx_msg void OnCMSectStiffenerN3Edt();
	afx_msg void OnCMSectStiffenerN4Edt();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTIONSTIFFENERDLG_H__)
