#if !defined(__CMSECTITEMSTLGIRDER_H__)
#define __CMSECTITEMSTLGIRDER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemStlGirder.h : header file
//
#include "CMSectItemBase.h"
#include "CMSecViewWnd.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"
using namespace mit::frx;

class CCMSectItemStlgView;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStlGirder dialog
//class CCMSectItemStlGirder;
class CCMSectItemStlGirder : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemStlGirder(CWnd* pParent = NULL);   // standard constructor

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;

	CCMSectItemStlgView* GetViewPointer() { return m_pStlgViewer; }

//---------------------------------------------------
// Interface
//---------------------------------------------------
	
//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitCtrl();
	void InitUnit();
	void InitView();
	void InitImage();

	void AlignControlDynamic();
	void AlignControl();
	void ControlsShowHide();
	void CreateOrDestoryView();
	void EnableDisableControls();

	void ChangeText();
	
	BOOL SaveData();
	BOOL Dlg2Data();
	void ShowKey2Dlg();
	void GetSizeEdtArByCurState(std::vector<CEditUnit*>& aPtSizeEdt);
	BOOL Dlg2Data_Box();
	BOOL Dlg2Data_I();
	BOOL Dlg2Data_MCell();
	BOOL GetPeriAndStiff();

	void Data2Dlg(BOOL bCopyBaseMembToLocal);
	void Data2Dlg_Box();
	void Data2Dlg_I();
	void Data2Dlg_MCell(BOOL bAfterSymmetric);
	void ChangeSectType();

	void SetOffsetData(T_SECT_SECTION_D* pData1, T_SECT_SECTION_D* pData2);
	void SymmetricSectAutoCalc();
	void DestroyView();
	void CreateSTLGView();
	BOOL IsViewWindow();
	void SetEdtValAuto();
	void DrawViewDlgSection();
protected:
	CCMSecViewWnd m_wndSecViewStlGirderBox;
	CCMSecViewWnd m_wndSecViewStlGirderI;

	CStringArray m_aOffsetName;

	T_SECT_D  m_DataBox;
	T_SECT_D  m_DataI;
	T_SECT_D  m_DataMCell;

	DWORD m_nLastType;
	static unsigned int WM_AFTERINITDIALOG;

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemStlGirder)
	enum { IDD = IDD_CMD_SECT_ITEM_STL_GIRDER };

	CEditUnit	m_edtID;
	CEditUnit m_edtName;
	MComboBox	m_cmbSectType;
	BOOL      m_bSymAutoCalc;

	CEditUnit m_edtTop;
	CEditUnit m_edtBot;
	CEditUnit m_edtB1;
	CEditUnit m_edtB2;
	CEditUnit m_edtB3;
	CEditUnit m_edtB4;
	CEditUnit m_edtB5;
	CEditUnit m_edtB6;
	CEditUnit m_edtH;
	CEditUnit m_edtt1;
	CEditUnit m_edtt2;
	CEditUnit m_edttw1;
	CEditUnit m_edttw2;

	CTextUnit m_untTop;
	CTextUnit m_untBot;
	CTextUnit m_untH;
	CTextUnit m_untt1;
	CTextUnit m_untt2;
	CTextUnit m_unttw1;
	CTextUnit m_unttw2;

	CEditUnit m_edtMCellB1;
	CEditUnit m_edtMCellB2;
	CEditUnit m_edtMCellB3;
	CEditUnit m_edtMCellB4;
	CEditUnit m_edtMCellB5;
	CEditUnit m_edtMCellB6;
	CEditUnit m_edtMCellH;
	CEditUnit m_edtMCellTf1;
	CEditUnit m_edtMCellTw1;
	CEditUnit m_edtMCellAddL1;
	CEditUnit m_edtMCellAddL2;
	CEditUnit m_edtMCellAddL3;
	CEditUnit m_edtMCellAddL4;
	CEditUnit m_edtMCellAddL5;
	CEditUnit m_edtMCellAddL6;
	CEditUnit m_edtMCellBr1;
	CEditUnit m_edtMCellBr2;
	CEditUnit m_edtMCellBr3;
	CEditUnit m_edtMCellBr4;
	CEditUnit m_edtMCellBr5;
	CEditUnit m_edtMCellTf2;
	CEditUnit m_edtMCellTw2;
	CEditUnit m_edtMCellAddR1;
	CEditUnit m_edtMCellAddR2;
	CEditUnit m_edtMCellAddR3;
	CEditUnit m_edtMCellAddR4;
	CEditUnit m_edtMCellAddR5;
	CEditUnit m_edtMCellAddR6;

	CTextUnit m_untMCellSize1;
	CTextUnit m_untMCellSize2;
	CTextUnit m_untMCellSize3;
	CTextUnit m_untMCellSize4;
	CTextUnit m_untMCellSize5;
	CTextUnit m_untMCellSize6;
	CTextUnit m_untMCellSize7;
	CTextUnit m_untMCellSize8;
	CTextUnit m_untMCellSize9;
	CTextUnit m_untMCellSize10;
	CTextUnit m_untMCellSize11;
	CTextUnit m_untMCellSize12;
	CTextUnit m_untMCellSize13;

	MButton m_chkMCellSym;

	CSpinButtonCtrl	m_spnMCellNoCell;
	CEditUnit	m_edtMCellNoCell;

	MButton m_chkMCellJ1;
	MButton m_chkMCellJ2;
	MButton m_chkMCellJr1;
	MButton m_chkMCellJr2;

	CArray<UINT, UINT> m_aRdoMCellLorR;
	CArray<UINT, UINT> m_aRdoMCellFShape;

	BOOL      m_bShearDeform;
	BOOL      m_bWarpingEffect;
	CEditUnit m_edtOffset;
	
	CCMSectItemStlgView* m_pStlgViewer;
	MillustViewer m_ImgViewer_B;
	MillustViewer m_ImgViewer_I;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemStlGirder)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemStlGirder)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSectTypeCmb();
	afx_msg void OnSymmetricAutoCalcBtn();
	afx_msg void OnChangeFlangeShape();
	afx_msg void OnChangeCtrlConnectSize();  
	afx_msg void OnChangeSizeEdt();
	afx_msg void OnStiffenerBtn();
	afx_msg void OnRivetAngleBtn();
	afx_msg void OnOffsetBtn();
	afx_msg void OnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnAfterInitDialog(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMSTLGIRDER_H__)
