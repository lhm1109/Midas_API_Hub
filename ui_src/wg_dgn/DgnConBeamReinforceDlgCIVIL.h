#if !defined(__DGNCONBEAMREINFORCEDLGCIVIL_H__)
#define __DGNCONBEAMREINFORCEDLGCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamReinforceDlgCIVIL.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_cmd\wg_cmd_CMSectViewWnd.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnConBeamReinforceCommonDlgCIVIL.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceDlgCIVIL dialog

#include "DgnBeamGridWnd.h"
#include "DgnSectionDrawWnd.h"

#include "HeaderPre.h"
class CDgnBeamGridWndCIVIL;
class __MY_EXT_CLASS__ CDgnConBeamReinforceDlgCIVIL : public CDgnConBeamReinforceCommonDlgCIVIL
{
// Construction
public:
	CDgnConBeamReinforceDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
		
// Dialog Data
	//{{AFX_DATA(CDgnConBeamReinforceDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_BEAM_DLG };
	
	//}}AFX_DATA
	virtual void Execute() {} 

protected:  
	CDgnBeamGridWndCIVIL*  m_TopGrid;  // Top   Grid
	CDgnBeamGridWndCIVIL*  m_BotGrid;  // Botom Grid

	//{{AFX_VIRTUAL(CDgnConBeamReinforceDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	
	// 여기에 추가로 구연을 하려면 CDgnConBeamReinforceCommonDlgCIVIL에 있던 내용은 가상함수로 만들고, CDgnConBeamReinforceCommonDlgCIVIL을 상속받은 모든 Class에서 실제 내용을 추가하시오..
	virtual void SetLayerNumToGrid(int nTopBot, int nLayerNum);
	virtual BOOL CheckReDraw_RebarGrid(_DGNC_RBAR& RebarData, int& iChkResult);
	virtual void SetDataToGrid();
	virtual void InitGridData();	
	virtual void SetDataToGrid_TabCtrl();
	virtual void GetDataFromGrid_TabCtrl(int iIMJ);

private:
	void ShowHideCtrl();

private:
	CArray<UINT, UINT> m_aCrackCtrl;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONBEAMREINFORCEDLGCIVIL_H__)
