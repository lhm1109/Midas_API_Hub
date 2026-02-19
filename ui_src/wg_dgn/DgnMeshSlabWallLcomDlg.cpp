// DgnMeshSlabWallLcomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMeshSlabWallLcomDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshSlabWallLcomDlg dialog


CDgnMeshSlabWallLcomDlg::CDgnMeshSlabWallLcomDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMeshSlabWallLcomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMeshSlabWallLcomDlg)		
	//}}AFX_DATA_INIT
	aCtrlID_HideInKR.RemoveAll();
	aCtrlID_HideInKR.Add(IDC_WG_DGN_STATIC4);
	aCtrlID_HideInKR.Add(IDC_WG_DGN_STATIC5);
	aCtrlID_HideInKR.Add(IDC_DGN_MESH_WALL_STRN_LST);
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnMeshSlabWallLcomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMeshSlabWallLcomDlg)		
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_STRN_LST, m_lstSlabStrength);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_STRN_ELS_LST, m_lstSlabELSStrength);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_SERV_LST, m_lstSlabService );
	DDX_Control(pDX, IDC_DGN_MESH_WALL_STRN_LST, m_lstWallStrength);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_DEFL_LST, m_lstSlabDeflection);  
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_CRACK_SERV_LST, m_lstSlabCrackService);  
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_STRN_LST, m_lstShellStrength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnMeshSlabWallLcomDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnMeshSlabWallLcomDlg)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SLAB_STRN_LST, OnSelchangeSlabStrnLst)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SLAB_STRN_ELS_LST, OnSelchangeSlabELSStrnLst)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SLAB_SERV_LST, OnSelchangeSlabServLst)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_WALL_STRN_LST, OnSelchangeWallStrnLst)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SLAB_DEFL_LST, OnSelchangeSlabDeflLst)	
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SHELL_STRN_LST, OnSelchangeShellStrnLst)
	ON_LBN_SELCHANGE(IDC_DGN_MESH_SLAB_CRACK_SERV_LST, OnSelchangeSlabCrackServLst)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshSlabWallLcomDlg message handlers

BOOL CDgnMeshSlabWallLcomDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	T_DCON_D DconD;
	DconD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(DconD);

	SetDlgTitle();

	// PMS 5319-3, 21 : ACI/KCI 아닐 때는 Strength/Special/Vertical 리스트에 _T("Strength") 하중조합만 보일 수 있도록 수정부탁드립니다. 
	//                  _T("Special/Vertical") 텍스트도 안보이거나 비활성화로 보이도록 처리되면 좋겠습니다. 

	BOOL bUS = CProduct::IsMovingType(D_PRODUCT_MOVING_US);
	BOOL bRUS = CProduct::IsMovingType(D_PRODUCT_MOVING_RUS);
	BOOL bAIJ_JP = CDBLib::IsCodeForMeshedDesignAIJ_JP();

	const BOOL bLcomSeisVert = CDBLib::IsCodeMeshSlabLcomSpecVert();
	if(!bLcomSeisVert || bUS || bAIJ_JP || bRUS)
	{
		GetDlgItem(IDC_WG_DGN_STATIC1 )->ShowWindow(FALSE);
		GetDlgItem(IDC_WG_DGN_STATIC10)->SetWindowText(_LS(IDS_DGN_RATING_STRENGTH));
	}

	SetDlgItemText(IDC_DGN_MESH_SLAB_DESC_EDT, _T(""));

	if(m_pDoc->IsNTC2018() || DconD.DesignCode == CONCODE_SP_63_13330_2018)
        ShowULSAndELSCtrl();

#if defined(_ORG) || defined(_JP) || defined(_CH)
	// Meshed Wall 삭제
	HideWallCtrls();
	ShowHideShellCtrls(FALSE);
#else
	ShowHideShellCtrls(TRUE);
#endif

	if(bAIJ_JP)
	{
		GetDlgItem(IDC_WG_DGN_STATIC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_SLAB_DEFL_LST)->EnableWindow(FALSE);
	}

	int nLcomType = D_LCOMTYPE_CONCRETE;
	CArray<T_LCOM_K, T_LCOM_K> aLcomKeyList;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomKeyList);
	if(aLcomKeyList.GetSize() <= 0) return TRUE;
	
	int nIndex_SlabStrn=0;
	int nIndex_SlabELSStrn=0;
	int nIndex_SlabServ=0;
	int nIndex_WallStrn=0;
	int nIndex_SlabDefl=0;
	int nIndex_ShellStrn=0;
		
	CDC* pDC = m_lstSlabStrength.GetDC();  
	CFont* f = m_lstSlabStrength.GetFont();
	pDC->SelectObject(f);
	CSize sz;
	INT nWidth_strn = 0;
	INT nWidth_ELSstrn = 0;
	INT nWidth_serv = 0;
	INT nWidth_defl = 0;

	T_LCOM_D LcomD;
	for(int i=0; i<aLcomKeyList.GetSize(); i++)
	{
		LcomD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, aLcomKeyList[i], LcomD)) {ASSERT(0); continue;}

        const int nActive = LcomD.nActive;
		if(bLcomSeisVert && (nActive == EN_DGN_SPEC || nActive == EN_DGN_VERT))
		{
			nIndex_SlabStrn = m_lstSlabStrength.AddString(LcomD.LoadCombName);
			m_lstSlabStrength.SetItemData(nIndex_SlabStrn, aLcomKeyList[i]);

			nIndex_WallStrn = m_lstWallStrength.AddString(LcomD.LoadCombName);
			m_lstWallStrength.SetItemData(nIndex_WallStrn, aLcomKeyList[i]);
			
			sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
			sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
			if(sz.cx > nWidth_strn)
				nWidth_strn = sz.cx;
		}
        else if ( nActive == EN_DGN_STRN )
		{
			nIndex_SlabStrn = m_lstSlabStrength.AddString(LcomD.LoadCombName);
			m_lstSlabStrength.SetItemData(nIndex_SlabStrn, aLcomKeyList[i]);

			nIndex_WallStrn = m_lstWallStrength.AddString(LcomD.LoadCombName);
			m_lstWallStrength.SetItemData(nIndex_WallStrn, aLcomKeyList[i]);
			
			nIndex_ShellStrn = m_lstShellStrength.AddString(LcomD.LoadCombName);
			m_lstShellStrength.SetItemData(nIndex_ShellStrn, aLcomKeyList[i]);

			sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
			sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
			if(sz.cx > nWidth_strn)
				nWidth_strn = sz.cx;
		}
        else if ( nActive == EN_DGN_SERV )
		{
			if (DconD.DesignCode == _T("Eurocode2:04") || DconD.DesignCode == _T("Eurocode2"))
			{
				if(LcomD.nServLcomType == 1 || LcomD.nServLcomType == 2 || LcomD.nServLcomType == 3)
				{
					nIndex_SlabServ = m_lstSlabService.AddString(LcomD.LoadCombName);
					m_lstSlabService.SetItemData(nIndex_SlabServ, aLcomKeyList[i]);

					sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
					sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
					if(sz.cx > nWidth_serv)
						nWidth_serv = sz.cx;
				}

				if(m_pDoc->m_pAttrCtrl->IsStaticLCComb(LcomD) && LcomD.nServLcomType == 1)
				{
					nIndex_SlabDefl = m_lstSlabDeflection.AddString(LcomD.LoadCombName);
					m_lstSlabDeflection.SetItemData(nIndex_SlabDefl, aLcomKeyList[i]);

					sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
					sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
					if(sz.cx > nWidth_defl)
						nWidth_defl = sz.cx;
				}
			}
			else
			{
				// add to <serviceability : general> check combination.
				nIndex_SlabServ = m_lstSlabService.AddString(LcomD.LoadCombName);
				m_lstSlabService.SetItemData(nIndex_SlabServ, aLcomKeyList[i]);

				//
				nIndex_SlabServ = m_lstSlabCrackService.AddString(LcomD.LoadCombName);
				m_lstSlabCrackService.SetItemData(nIndex_SlabServ, aLcomKeyList[i]);

				sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
				sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
				if(sz.cx > nWidth_serv)
					nWidth_serv = sz.cx;

				if(!bAIJ_JP)
				{
					// add to <serviceability : deflection> check combination.
					nIndex_SlabDefl = m_lstSlabDeflection.AddString(LcomD.LoadCombName);
					m_lstSlabDeflection.SetItemData(nIndex_SlabDefl, aLcomKeyList[i]);
				}

				sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
				sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
				if(sz.cx > nWidth_defl)
					nWidth_defl = sz.cx;
			}
		}
        else if ( nActive == EN_DGN_STRE && m_pDoc->IsNTC2018() )
		{
			nIndex_SlabELSStrn = m_lstSlabELSStrength.AddString(LcomD.LoadCombName);
			m_lstSlabELSStrength.SetItemData(nIndex_SlabELSStrn, aLcomKeyList[i]);

			sz = pDC->GetTextExtent(LcomD.LoadCombName, (int)_tcslen(LcomD.LoadCombName));
			sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
			if(sz.cx > nWidth_ELSstrn)
				nWidth_ELSstrn = sz.cx;
		}
		else
			continue;
	} 
	
	// 수평 스크롤 추가
	int nSizePlus = 14;
	m_lstSlabStrength.SetHorizontalExtent(nWidth_strn+nSizePlus);
	m_lstSlabELSStrength.SetHorizontalExtent(nWidth_ELSstrn+nSizePlus);
	m_lstWallStrength.SetHorizontalExtent(nWidth_strn+nSizePlus);
	m_lstSlabService.SetHorizontalExtent(nWidth_serv+nSizePlus);
	m_lstSlabDeflection.SetHorizontalExtent(nWidth_defl+nSizePlus);
	m_lstShellStrength.SetHorizontalExtent(nWidth_strn+nSizePlus);
	m_lstSlabCrackService.SetHorizontalExtent(nWidth_serv+nSizePlus);
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnMeshSlabWallLcomDlg::SetDlgTitle()
{ 
#if defined(_ORG) || defined(_JP) || defined(_CH)
	// Meshed Wall 삭제
	if(CDBLib::IsConCodeForShell())
	{
		this->SetWindowText(_LS(IDS_DGN_SLAB_SHELL_LCOM));
	}
	else
	{
		this->SetWindowText(_LS(IDS_DGN_MESHEDSLABLCOM));
	}
#else
	if(CDBLib::IsConCodeForShell())
	{
		this->SetWindowText(_LS(IDS_DGN_SLAB_WALL_SHELL_LCOM));
	}
	else
	{
		this->SetWindowText(_LS(IDS_DGN_MESHED_SLAB_WALL_LCOM));    
	}
#endif
}

void CDgnMeshSlabWallLcomDlg::Data2Dlg()
{  
	T_MLCB_D MlcbD; MlcbD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->Get_DefaultMlcb(MlcbD)) return;
 
	int i, nSize;
	UINT LcomK;  
	CMap<UINT,UINT,UINT,UINT> mapLcomK;
	
	nSize = MlcbD.aSlabServK.GetSize();
	if(nSize > 0)
	{
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aSlabServK[i], (UINT)i);
		
		for(i=0; i<m_lstSlabService.GetCount(); i++)
		{
			LcomK = m_lstSlabService.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstSlabService.SetCheck(i, TRUE);
		} 
	}
	
	nSize = MlcbD.aSlabStreK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aSlabStreK[i], (UINT)i);
	
		for(i=0; i<m_lstSlabStrength.GetCount(); i++)
		{
			LcomK = m_lstSlabStrength.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstSlabStrength.SetCheck(i, TRUE);
		} 
	}

	nSize = MlcbD.aSlabELSStreK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aSlabELSStreK[i], (UINT)i);

		for(i=0; i<m_lstSlabELSStrength.GetCount(); i++)
		{
			LcomK = m_lstSlabELSStrength.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstSlabELSStrength.SetCheck(i, TRUE);
		} 
	}

	nSize = MlcbD.aWallStreK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aWallStreK[i], (UINT)i);
	
		for(i=0; i<m_lstWallStrength.GetCount(); i++)
		{
			LcomK = m_lstWallStrength.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstWallStrength.SetCheck(i, TRUE);
		} 
	}  

	nSize = MlcbD.aSlabCracK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aSlabCracK[i], (UINT)i);
		
		for(i=0; i<m_lstSlabDeflection.GetCount(); i++)
		{
			LcomK = m_lstSlabDeflection.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstSlabDeflection.SetCheck(i, TRUE);
		} 
	}  

	nSize = MlcbD.aShellStreK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aShellStreK[i], (UINT)i);

		for(i=0; i<m_lstShellStrength.GetCount(); i++)
		{
			LcomK = m_lstShellStrength.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstShellStrength.SetCheck(i, TRUE);
		} 
	}

	nSize = MlcbD.aSlabCrackServK.GetSize();
	if(nSize > 0)
	{
		mapLcomK.RemoveAll();
		for(i=0; i<nSize; i++)  
			mapLcomK.SetAt(MlcbD.aSlabCrackServK[i], (UINT)i);

		for(i=0; i<m_lstSlabCrackService.GetCount(); i++)
		{
			LcomK = m_lstSlabCrackService.GetItemData(i);
			if(mapLcomK.Lookup(LcomK, LcomK))
				m_lstSlabCrackService.SetCheck(i, TRUE);
		} 
	}
}

BOOL CDgnMeshSlabWallLcomDlg::Dlg2Data()
{
	UINT LcomK;  
	T_MLCB_D MlcbD; MlcbD.Initialize();
	for(int i=0; i<m_lstSlabService.GetCount(); i++)
	{
		if(m_lstSlabService.GetCheck(i) == 1)
		{
			LcomK = m_lstSlabService.GetItemData(i);
			MlcbD.aSlabServK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstSlabStrength.GetCount(); i++)
	{
		if(m_lstSlabStrength.GetCheck(i) == 1)
		{
			LcomK = m_lstSlabStrength.GetItemData(i);
			MlcbD.aSlabStreK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstSlabELSStrength.GetCount(); i++)
	{
		if(m_lstSlabELSStrength.GetCheck(i) == 1)
		{
			LcomK = m_lstSlabELSStrength.GetItemData(i);
			MlcbD.aSlabELSStreK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstWallStrength.GetCount(); i++)
	{
		if(m_lstWallStrength.GetCheck(i) == 1)
		{
			LcomK = m_lstWallStrength.GetItemData(i);
			MlcbD.aWallStreK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstSlabDeflection.GetCount(); i++)
	{
		if(m_lstSlabDeflection.GetCheck(i) == 1)
		{
			LcomK = m_lstSlabDeflection.GetItemData(i);
			MlcbD.aSlabCracK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstShellStrength.GetCount(); i++)
	{
		if(m_lstShellStrength.GetCheck(i) == 1)
		{
			LcomK = m_lstShellStrength.GetItemData(i);
			MlcbD.aShellStreK.Add(LcomK);
		}
	} 

	for(int i=0; i<m_lstSlabCrackService.GetCount(); i++)
	{
		if(m_lstSlabCrackService.GetCheck(i) == 1)
		{
			LcomK = m_lstSlabCrackService.GetItemData(i);
			MlcbD.aSlabCrackServK.Add(LcomK);
		}
	} 

	if(!m_pDoc->m_pDataCtrl->AddMlcb(MlcbD))
		return FALSE;

	return TRUE;
}

void CDgnMeshSlabWallLcomDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	CDialog::OnOK();
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeSlabStrnLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstSlabStrength.GetSelCount();
	if(nCount<1) return;
	
	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstSlabStrength.GetSelItems(nCount, aSelItem.GetData());
	
	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstSlabStrength.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeSlabELSStrnLst()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstSlabELSStrength.GetSelCount();
	if(nCount<1) return;

	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstSlabELSStrength.GetSelItems(nCount, aSelItem.GetData());

	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstSlabELSStrength.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeShellStrnLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstShellStrength.GetSelCount();
	if(nCount<1) return;

	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstShellStrength.GetSelItems(nCount, aSelItem.GetData());

	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstShellStrength.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeSlabServLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstSlabService.GetSelCount();
	if(nCount<1) return;
	
	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstSlabService.GetSelItems(nCount, aSelItem.GetData());
	
	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstSlabService.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeSlabCrackServLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstSlabCrackService.GetSelCount();
	if(nCount<1) return;

	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstSlabCrackService.GetSelItems(nCount, aSelItem.GetData());

	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstSlabCrackService.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeWallStrnLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstWallStrength.GetSelCount();
	if(nCount<1) return;
	
	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstWallStrength.GetSelItems(nCount, aSelItem.GetData());
	
	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstWallStrength.GetItemData(nIndex);
	SetDescription(nLcomK);
}

void CDgnMeshSlabWallLcomDlg::OnSelchangeSlabDeflLst() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstSlabDeflection.GetSelCount();
	if(nCount<1) return;
	
	CArray<int, int> aSelItem;
	aSelItem.SetSize(nCount);
	m_lstSlabDeflection.GetSelItems(nCount, aSelItem.GetData());
	
	int  nIndex = aSelItem.GetAt(nCount-1);
	UINT nLcomK = m_lstSlabDeflection.GetItemData(nIndex);
	SetDescription(nLcomK);
}


void CDgnMeshSlabWallLcomDlg::SetDescription(UINT nLcomK)
{
	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, nLcomK, LcomD)) {ASSERT(0); return;}

	int nNum = m_pDoc->m_pAttrCtrl->GetCountLcomCase(D_LCOMTYPE_CONCRETE, LcomD);
	for(int i=0; i<nNum; i++)
	{
		LcomD.aCombination[i].AnalType;
		LcomD.aCombination[i].LoadCaseKey;
		LcomD.aCombination[i].Factor;
	}
		
	SetDlgItemText(IDC_DGN_MESH_SLAB_DESC_EDT, LcomD.Description);
}

// Meshed Wall 삭제
void CDgnMeshSlabWallLcomDlg::HideWallCtrls()
{  
	GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowText(_LS(IDS_DGN_MESHEDSLABLCOM));

	CDlgUtil::CtrlShowHide(this, aCtrlID_HideInKR, FALSE);
	
	// 벽이 떠난 빈자리만큼 대화상자를 줄여주자
	CRect rGroupSlab, rGroupWall;
	GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rGroupSlab);
	GetDlgItem(IDC_WG_DGN_STATIC4)->GetWindowRect(rGroupWall);
	int nWallVacancy = rGroupWall.right - rGroupSlab.right;

	CRect rDesc;
	
	if (CDBLib::GetConCodeName() == CONCODE_GB50010_10)
	{
		CRect rDefl;
		CRect rCrackServ;
		CRect rStatic7;
		CRect rServ;
		GetDlgItem(IDC_DGN_MESH_SLAB_DEFL_LST)->GetWindowRect(rDesc);
		GetDlgItem(IDC_DGN_MESH_SLAB_SERV_LST)->GetWindowRect(rServ);
		int nSetpOff = rDesc.left - rServ.right;
		GetDlgItem(IDC_DGN_MESH_SLAB_CRACK_SERV_LST)->GetWindowRect(rCrackServ);
		GetDlgItem(IDC_WG_DGN_STATIC7)->GetWindowRect(rStatic7);
		int nOff = rCrackServ.left - (rDesc.right + nSetpOff);
		rStatic7.left -= nOff;
		rStatic7.right -= nOff;
		rCrackServ.left -= nOff;
		rCrackServ.right -= nOff;
		GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowPos(NULL, rGroupSlab.left, rGroupSlab.top, rGroupSlab.Width() + rCrackServ.Width() + nSetpOff, rGroupSlab.Height(), SWP_NOMOVE | SWP_NOZORDER);
		int nAddOff = rCrackServ.Width() + nSetpOff;
		nWallVacancy -= nAddOff;
		ScreenToClient(rStatic7);
		ScreenToClient(rCrackServ);
		GetDlgItem(IDC_WG_DGN_STATIC7)->MoveWindow(rStatic7);
		GetDlgItem(IDC_DGN_MESH_SLAB_CRACK_SERV_LST)->MoveWindow(rCrackServ);
	}

	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->GetWindowRect(rDesc);
	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->SetWindowPos(NULL,rDesc.left,rDesc.top,rDesc.Width()-nWallVacancy,rDesc.Height(),SWP_NOMOVE | SWP_NOZORDER);

	CRect rOK, rCancel;
	GetDlgItem(IDOK)->GetWindowRect(rOK);
	rOK.left  -= nWallVacancy;
	rOK.right -= nWallVacancy;
	ScreenToClient(rOK);
	GetDlgItem(IDOK)->MoveWindow(rOK);
	GetDlgItem(IDCANCEL)->GetWindowRect(rCancel);
	rCancel.left  -= nWallVacancy;
	rCancel.right -= nWallVacancy;
	ScreenToClient(rCancel);
	GetDlgItem(IDCANCEL)->MoveWindow(rCancel);
		
	CRect rDlg;
	this->GetWindowRect(rDlg);
	this->SetWindowPos(NULL,rDlg.left,rDlg.top,rDlg.Width()-nWallVacancy,rDlg.Height(),SWP_NOMOVE | SWP_NOZORDER);
}

void CDgnMeshSlabWallLcomDlg::ShowHideShellCtrls(BOOL bShowWallCtrl)
{
	if(!CDBLib::IsConCodeForShell())
	{
		return; 
	}

	CRect rRef, rToMove;
	int nDistX,nDistY;	
	nDistX = nDistY = 0;
	CArray<UINT, UINT> aCtrl;

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_SHELL_STC);
	aCtrl.Add(IDC_WG_DGN_SHELL_STRN_STC);
	aCtrl.Add(IDC_DGN_MESH_SHELL_STRN_LST);
	GetDlgItem(IDC_WG_DGN_STATIC4)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_SHELL_STC)->GetWindowRect(rToMove);
	if(bShowWallCtrl)
	{
		nDistX = rRef.right - rToMove.left + globalUtils.ScaleByDPI(8);
	}
	else
	{
		nDistX = rRef.left - rToMove.left;
	}
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	aCtrl.RemoveAll();
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);
	GetDlgItem(IDC_WG_DGN_SHELL_STC)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);  
	nDistX = rRef.right - rToMove.right;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);
	
	// resize desc edit size
	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->GetWindowRect(rToMove);
	GetDlgItem(IDC_WG_DGN_SHELL_STC)->GetWindowRect(rRef);    
	rToMove.right = rRef.right;  
	ScreenToClient(rToMove);
	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right+globalUtils.ScaleByDPI(16);
	MoveWindow(r);
}

void CDgnMeshSlabWallLcomDlg::ShowULSAndELSCtrl()
{
	CRect rRef, rToMove;
	int nDistX,nDistY;    
	nDistX = nDistY = 0;
	CArray<UINT, UINT> aCtrl;

	GetDlgItem(IDC_WG_DGN_STATIC10)->SetWindowText(_LSX(Strength (ULS)));

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_ELS_STC);
	aCtrl.Add(IDC_DGN_MESH_SLAB_STRN_ELS_LST);
	GetDlgItem(IDC_WG_DGN_STATIC2)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_ELS_STC)->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;

	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_STATIC2);
	aCtrl.Add(IDC_DGN_MESH_SLAB_SERV_LST);
	aCtrl.Add(IDC_WG_DGN_STATIC3);
	aCtrl.Add(IDC_DGN_MESH_SLAB_DEFL_LST);
	aCtrl.Add(IDC_WG_DGN_STATIC4);
	aCtrl.Add(IDC_WG_DGN_STATIC5);
	aCtrl.Add(IDC_DGN_MESH_WALL_STRN_LST);
	GetDlgItem(IDC_WG_DGN_STATIC3)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_STATIC2)->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;

	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	aCtrl.RemoveAll();
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);
	GetDlgItem(IDC_WG_DGN_STATIC4)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);  
	nDistX = rRef.right - rToMove.right;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	// resize desc edit size
	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->GetWindowRect(rToMove);
	GetDlgItem(IDC_WG_DGN_STATIC4)->GetWindowRect(rRef);    
	rToMove.right = rRef.right;  
	ScreenToClient(rToMove);
	GetDlgItem(IDC_DGN_MESH_SLAB_DESC_EDT)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

	// resize Slab Group Box size
	GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rToMove);
	GetDlgItem(IDC_WG_DGN_STATIC4)->GetWindowRect(rRef);    
	rToMove.right = rRef.left - globalUtils.ScaleByDPI(8);  
	ScreenToClient(rToMove);
	GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right+globalUtils.ScaleByDPI(16);
	MoveWindow(r);
}