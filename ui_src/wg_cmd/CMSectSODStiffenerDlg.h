#if !defined(__CMSECSODSTIFFENERDLG_H__)
#define __CMSECSODSTIFFENERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectSODStiffenerDlg.h : header file
//

#include "CMSecViewWnd.h"
#include "CMSectStiffenerGrid.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectSODStiffenerDlg dialog
class __MY_EXT_CLASS__ CCMSectSODStiffenerDlg : public CDialogMove
{
// Construction
public:
	CCMSectSODStiffenerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectSODStiffenerDlg();

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitUnit();
	void InitCombo();
	void InitView();

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

	void GetShapeList_by_Sect(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape);

	CCMSecViewWnd m_wndSecView;
	MillustViewer m_ImgSectItem;

	CBitmap*	m_pStiffnerBmp;

public:
	void UpdateSectView();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectSODStiffenerDlg)
	enum { IDD = IDD_CMD_SECT_SOD_STIFFENER_DLG };

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
	//}}AFX_DATA

	T_SECT_D m_Data;
	T_SECT_D m_Data_Draw;
	BOOL m_bTapJ; // 이번 버전에는 없는데 나중에 필요하면 추가 한다
	int  m_nSelIndex; // Dlg 띄울때 List 선택해주는 옵션
	BOOL m_nDlgType; // 0:Stiffener, 1:Rivet

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectSODStiffenerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectSODStiffenerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCMSectSODStiffenerTypeCmb();
	afx_msg void OnCMSectSODStiffenerSizeEdt();
	afx_msg void OnCMSectSODStiffenerAddBtn();
	afx_msg void OnCMSectSODStiffenerModBtn();
	afx_msg void OnCMSectSODStiffenerDelBtn();
	afx_msg void OnCMItemchangedSODStiffenerList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECSODSTIFFENERDLG_H__)
