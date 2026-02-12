#if !defined(__CMSECTRIVETANGLEDLG_H__)
#define __CMSECTRIVETANGLEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectRivetAngleDlg.h : header file
//

#include "CMSecViewWnd.h"
#include "CMSectRivetAngleGrid.h"
#include "CMSectRivetAngleGridNew.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleDlg dialog
class CCMSectRivetAngleDlg : public CDialogMove
{
// Construction
public:
	CCMSectRivetAngleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectRivetAngleDlg();

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitCombo();
	void InitGrid();
	void InitView();
	void ChangeBitmap();

	void SetHeaderTitle_DefStiff();
	void MakeItemEx_DefStiff();
	BOOL InsertItem_DefStiff(int nIndex, T_STIFF_SHAPE_D &Data);
	CString DataToStr_DefStiff(int i, T_STIFF_SHAPE_D &Data);

	void EnableDisableCtrl();

	void Data2Dlg();
	BOOL Dlg2Data();

	void GetDefStiffener();

	CCMSecViewWnd m_wndSecView;
	MillustViewer m_ImgRivet;

	//CCMSectRivetAngleGrid* m_pGrid;
	CCMSectRivetAngleGridNew* m_pGrid;
	
public:
	void UpdateSectView();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectRivetAngleDlg)
	enum { IDD = IDD_CMD_SECT_RIVET_ANGLE_DLG };
	
	CListCtrl m_lstDefStiff;
	BOOL      m_bSymmetric;
	BOOL      m_bAngle[8];
	MComboBox m_cmbAngle[8];
	BOOL      m_bRivet;
	//}}AFX_DATA

	T_SECT_D m_Data;
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> m_aSODStiffShape;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectRivetAngleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectRivetAngleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnCMSectRivetStiffenerBtn();
	afx_msg void OnDblclkDefStiffList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCMSectSymmetricChk();
	afx_msg void OnCMSectAngleChk();
	afx_msg void OnCMSectAngleCmb();
	afx_msg void OnCMSectRivetChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTRIVETANGLEDLG_H__)
