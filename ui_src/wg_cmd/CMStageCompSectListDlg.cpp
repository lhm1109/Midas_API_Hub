// CMStageCompSectListDlg.cpp : implementation file
// 2002. 9. 14    by TAE
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageCompSectListDlg.h"
#include "CMStageCompSectItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectListDlg dialog


CCMStageCompSectListDlg::CCMStageCompSectListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageCompSectListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageCompSectListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_SectData.Initialize();
	m_nSectType = 0;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}


void CCMStageCompSectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageCompSectListDlg)
	DDX_Control(pDX, IDC_CMD_CS_SECT_LIST, m_ctrlList);
	//DDX_Control(pDX, IDC_CMD_CS_SECT_PREVIEW, m_ctrlViewFrm); // Unnecessary code
	DDX_Radio(pDX,   IDC_CMD_SECT_RDO, m_nSectType);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
// User Defined Function

#define LIST_COLM_NUM   5

// List Ctrl을 초기화하는 함수 - Style, Title, Size....
void CCMStageCompSectListDlg::InitItemList()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ctrlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ctrlList.GetSafeHwnd(), dwStyle);

	int aSize[] = {40, 70, 90, 65, 65};
	CString aTitle[] = {_LS(IDS_CMD_SECTION_id), _LS(IDS_CMD_CSCS__Stage), _LS(IDS_CMD_CSCS__Section), 
											_LS(IDS_CMD_CSCS__Type), _LS(IDS_CMD_CSCS__Shape)};
	CString title;
	LVCOLUMN lvcolumn;
	for(int i=0; i<LIST_COLM_NUM; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		if(i==0) lvcolumn.fmt = LVCFMT_RIGHT;
		else     lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aSize[i]);
		title = aTitle[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_ctrlList.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

// Section Type을 String으로 변화해주는 함수
CString CCMStageCompSectListDlg::ConvStypeToString(int nStype)
{
	CString str = _T("");

	switch(nStype)  
	{
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:  
	case D_SECT_TYPE_COMPO_PC:
	case D_SECT_TYPE_COMPO_G:
		str = _LS(IDS_CMD_CSCS__Composite); 
		break;
	case D_SECT_TYPE_SRC:       str = _LS(IDS_CMD_CSCS__SRC);       break;
	case D_SECT_TYPE_PSC:       str = _LS(IDS_CMD_CSCS__PSC);       break;
	case D_SECT_TYPE_TAPERED:   str = _LS(IDS_CMD_CSCS__Tapered);   break;
	case D_SECT_TYPE_REGULAR:   str = _LS(IDS_CMD_CSCS__Regular);   break;
	case D_SECT_TYPE_USER:      str = _LS(IDS_CMD_CSCS__User);      break;
	case D_SECT_TYPE_COMBINED:  str = _LS(IDS_CMD_CSCS__Combined);  break;
	case D_SECT_TYPE_CONSTRUCTION: str = _LS(IDS_CMD_CSCS__Construction); break;
	case D_SECT_TYPE_COLDFORMED: str = _LS(IDS_CMD_CSCS__ColdFormed); break;
	case D_SECT_TYPE_HIBEAM:     str = _LS(IDS_CMD_CSCS__Hi_Beam);  break;
	default: break;
	}
	return str;

	/*
	CString strSecType = _T("");

	switch(nStype)  
	{
	case D_SECT_TYPE_SRC:
		strSecType = _LS(IDS_CMD_CSCS__SRC);
		break;
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
		strSecType = _LS(IDS_CMD_CSCS__Composite);
		break;
	case D_SECT_TYPE_PSC:
		strSecType = _LS(IDS_CMD_CSCS__PSC);
		break;
	case D_SECT_TYPE_TAPERED:
		strSecType = _LS(IDS_CMD_CSCS__Tapered);
		break;
	default: ASSERT(0); break;
	}
	return strSecType;
	*/
}


CString CCMStageCompSectListDlg::DataToStr(int nIndex, T_CSCS_K key, T_CSCS_D& data)
{
	CString str;

	if(nIndex == 0) str.Format(_T("%d"), key);
	else if(nIndex == 1) 
	{
		T_STAG_D StagData;
		m_pDoc->m_pAttrCtrl->GetStag(data.ActStagKey, StagData);
		str = StagData.StageName;
	}
	else if(nIndex == 2) 
	{
		T_SECT_D SectData;
		m_pDoc->m_pAttrCtrl->GetSectOrSecv(data.SectKey, SectData, m_nSectType==1);
		str.Format(_T("%d : "), data.SectKey);
		str += SectData.SName;
	}
	else if(nIndex == 3) 
	{
		T_SECT_D SectData;
		m_pDoc->m_pAttrCtrl->GetSectOrSecv(data.SectKey, SectData, m_nSectType==1);
		str = ConvStypeToString(SectData.nStype);
	}
	else if(nIndex == 4)
	{
		T_SECT_D SectData;
		m_pDoc->m_pAttrCtrl->GetSectOrSecv(data.SectKey, SectData, m_nSectType==1);
		str = SectData.SectBefore.Shape;
	}
	else ASSERT(0);
	
	return str;
}

// List Ctrl에 Item을 한줄씩 만드는 함수
void CCMStageCompSectListDlg::MakeEachItem(int nIndex, T_CSCS_K key, T_CSCS_D& data)
{
	CString str = _T("");
	
	LV_ITEM lvitem;
	for(int i=0; i<LIST_COLM_NUM; i++)
	{
		lvitem.iItem = nIndex;
		lvitem.iSubItem = i;
		str = DataToStr(i, nIndex+1, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if(i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = key;
			m_ctrlList.InsertItem(&lvitem);
		}
		else       m_ctrlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

// List Ctrl에 Item을 채워넣는 함수(전체)
void CCMStageCompSectListDlg::UpdateItemList()
{
	m_ctrlList.DeleteAllItems();

	int nCount = m_pDoc->m_pAttrCtrl->GetCountCscsOrCscv(m_nSectType==1);
	if(nCount <= 0) return;

	T_CSCS_D CscsD;
	CArray<T_CSCS_K, T_CSCS_K> aCscsK;
	m_pDoc->m_pAttrCtrl->GetCscsOrCscvKeyList(aCscsK, m_nSectType==1);
	ASSERT(nCount == aCscsK.GetSize());
	// Stage 순서, Section Key 순서로 Sorting 한다.
	SortCscsKey(aCscsK);

	for (int i = 0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetCscsOrCscv(aCscsK[i], CscsD, m_nSectType==1);
		MakeEachItem(i, aCscsK[i], CscsD);
	}
}

void CCMStageCompSectListDlg::SortCscsKey(CArray<T_CSCS_K, T_CSCS_K>& raCscsK)
{
	int nSize = raCscsK.GetSize();
	if (nSize <= 1) return;  // Sorting할 필요 없음
	CArray<T_POS_KEY2_PAIR, T_POS_KEY2_PAIR&> aPosKey2;
	aPosKey2.SetSize(nSize);
	T_CSCS_D CscsD;
	T_STAG_D StagD;
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetCscsOrCscv(raCscsK[i], CscsD, m_nSectType==1);
		m_pDoc->m_pAttrCtrl->GetStag(CscsD.ActStagKey, StagD);
		aPosKey2[i].Set(i, StagD.nStageId, CscsD.SectKey);
	}
	qsort(aPosKey2.GetData(), aPosKey2.GetSize(), sizeof(T_POS_KEY2_PAIR), CCompFunc::ComparePosKey2PairA);
	CArray<T_CSCS_K, T_CSCS_K> aTmpCscsK;
	aTmpCscsK.Copy(raCscsK);
	for (int i = 0; i < nSize; i++)
		raCscsK[i] = aTmpCscsK[aPosKey2[i].nPos];
}

void CCMStageCompSectListDlg::InitSection()
{
	BOOL bShowRdo = m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode();
	if (CDBLib::ActivateVBEM())	{ /*skip*/ } // 레지스트리 남겨둠.
	else
	{
		bShowRdo = FALSE;
	}

	// Virtual Option 숨김
	CArray<UINT, UINT> aCtrl; aCtrl.RemoveAll();
	aCtrl.Add(IDC_CMD_SECT_RDO);
	aCtrl.Add(IDC_CMD_SECV_RDO);
	CDlgUtil::CtrlShowHide(this, aCtrl, bShowRdo);

	GetDlgItem(IDC_CMD_CS_BTN_ADD        )->EnableWindow(m_nSectType==0);
	GetDlgItem(IDC_CMD_CS_BTN_DEL        )->EnableWindow(m_nSectType==0);
	GetDlgItem(IDC_CMD_CS_BTN_UPDATE     )->EnableWindow(m_nSectType==0);
	GetDlgItem(IDC_CMD_CS_UPDATE_LONG_BTN)->EnableWindow(m_nSectType==0);
}

BEGIN_MESSAGE_MAP(CCMStageCompSectListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageCompSectListDlg)
	ON_BN_CLICKED(IDC_CMD_CS_BTN_ADD, OnCmdCsBtnAdd)
	ON_BN_CLICKED(IDC_CMD_CS_BTN_DEL, OnCmdCsBtnDel)
	ON_BN_CLICKED(IDC_CMD_CS_BTN_MOD, OnCmdCsBtnMod)
	ON_BN_CLICKED(IDC_CMD_CS_BTN_UPDATE, OnCmdCsBtnUpdate)
	ON_BN_CLICKED(IDC_CMD_CS_UPDATE_LONG_BTN, OnCmdCsUpdateLongTermBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_CS_SECT_LIST, OnItemchangedCmdCsSectList)
	ON_BN_CLICKED(IDC_CMD_CS_BTN_CLOSE, OnCmdCsBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_CS_SECT_LIST, OnDblclkCmdCsSectList)
	ON_BN_CLICKED(IDC_CMD_SECT_RDO, OnSectType)
	ON_BN_CLICKED(IDC_CMD_SECV_RDO, OnSectType)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectListDlg message handlers

//--------------------------------------------------------------------------
// OnInitDialog   초기화 
//--------------------------------------------------------------------------
BOOL CCMStageCompSectListDlg::OnInitDialog() 
{
	BOOL bShowRdo = FALSE;
	if (CDBLib::ActivateVBEM()) { /*skip*/ } // 레지스트리 남겨둠.
	else
	{
		bShowRdo = FALSE;
	}

	if (bShowRdo && m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode())
	{
		m_nSectType = 1;
		UpdateData(FALSE);
	}

	MInitCombo initCombo;
	SetRedraw(FALSE);

	CDialogMove::OnInitDialog();

	InitSection();
	InitItemList();
	UpdateItemList();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

    CWnd* pWnd = GetDlgItem(IDC_CMD_CS_SECT_PREVIEW); ASSERT(pWnd);
    m_ctrlViewFrm.Init(pWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------------
// OnCmdCsBtnClose   닫기
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnCmdCsBtnClose() 
{
	CDialogMove::OnOK();
}

//--------------------------------------------------------------------------
// OnCmdCsBtnAdd   추가 
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnCmdCsBtnAdd() 
{
	CCMStageCompSectItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.SetVirtualType(m_nSectType == 1);
	dlg.DoModal();
}

//--------------------------------------------------------------------------
// OnCmdCsBtnMod  수정   
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnCmdCsBtnMod() 
{
	T_CSCS_K Key;
	T_CSCS_D Data;

	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
 
	Key = m_ctrlList.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetCscsOrCscv(Key, Data, m_nSectType == 1)) return;

	CCMStageCompSectItemDlg dlg;
	dlg.SetParam(Key, Data);
	dlg.SetVirtualType(m_nSectType == 1);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_ctrlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

//--------------------------------------------------------------------------
// OnCmdCsBtnDel  삭제    
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnCmdCsBtnDel() 
{
	if (m_nSectType == 1)
		return;

	T_CSCS_K Key;

	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	Key = m_ctrlList.GetItemData(iItem);
	m_pDoc->m_pDataCtrl->DelCscs(Key);

	int nCount = m_ctrlList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_ctrlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

//--------------------------------------------------------------------------
// OnCmdCsBtnUpdate - H 재계산
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnCmdCsBtnUpdate() 
{
	if (m_nSectType == 1)
		return;

	//T_CSCS_K Key;
	CArray<T_CSCS_K, T_CSCS_K> aKey; aKey.RemoveAll();

	/*
	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_ctrlList.GetNextSelectedItem(pos);
			Key = m_ctrlList.GetItemData(nItem);
			aKey.Add(Key);
		}
	}*/
	m_pDoc->m_pAttrCtrl->GetCscsKeyList(aKey);

	CCMStageCompSectItemDlg dlgForCalc;

	T_CSCS_D CscsD;
	int nCount = aKey.GetSize();
	int nSuccess = 0;
	for (int i=0 ; i<nCount ; ++i)
	{
		BOOL bResult = FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetCscs(aKey[i], CscsD)) continue;

		// 메시지 출력 및 계산
		CString str;
		str.Format(_LS(IDS_CMD_CSCS__Msg_WaitMessage), i+1, nCount);
		GSaveHistoryFormatNF(str);
		int nPartSize = CscsD.aPart.GetSize();
		CArray<double, double> aH;
		if (!dlgForCalc.Get_H_FromCscs(nPartSize, CscsD, aH)) continue; // CscsD 는 복사 입력값

		// H 값 세팅
		if (CscsD.nCompType == 0) // 12 3
		{
			for(int j=0 ; j<nPartSize ; ++j)
			{
				if(j==0 || j==1) CscsD.aPart[j].dH = aH[0];
				else CscsD.aPart[j].dH = aH[1];
			}
		}
		else if (CscsD.nCompType == 1) // 1 23
		{
			for(int j=0 ; j<nPartSize ; ++j)
			{
				if(j==0) CscsD.aPart[j].dH = aH[0];
				else CscsD.aPart[j].dH = aH[1];
			}
		}
		else if (CscsD.nCompType == -2) // Normal
		{
			for(int j=0 ; j<nPartSize ; ++j)
			{
				if(j==0) CscsD.aPart[j].dH = aH[0];
				else CscsD.aPart[j].dH = aH[1];
			}
		}
		else if (CscsD.nCompType == -1) // User
		{
			for(int j=0 ; j<nPartSize ; ++j) CscsD.aPart[j].dH = 0.;
		}
		else ASSERT(0);

		// CscsData 수정
		bResult = m_pDoc->m_pDataCtrl->ModifyCscs(aKey[i], CscsD);
		if(bResult) nSuccess++;
	}

	CString str;
	if (nSuccess > 0)
	{ str.Format(_LS(IDS_CMD_CSCS__Msg_SuccessMessage), nSuccess); }
	else
	{ str = _LS(IDS_CMD_CSCS__Msg_FailMessage); }
	AfxMessageBox(str);
}

void CCMStageCompSectListDlg::OnCmdCsUpdateLongTermBtn()
{
	if (m_nSectType == 1)
		return;

	// 1) 시공단계 해석이 구성되어 있어야 함.
	if(m_pDoc->m_pAttrCtrl->GetCountStag() <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD_CSCS_ERROR_MSG_CSCS));
		return;
	}
	
	// 2)	시공단계 합성단면 데이터(Composite Section for CS)가 정의되어 Part2의 단계별로 활성화되어야 함. 
	if(m_pDoc->m_pAttrCtrl->GetCountCscs() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_CSCS_ERROR_MSG_STAGE));
		return;
	}
	
	T_STAG_D StagD; StagD.Initialize();
	T_BNGR_D BngrD; BngrD.Initialize();
	T_CSCS_D CscsD; CscsD.Initialize();
	CArray<T_STAG_K, T_STAG_K>  aStagK; aStagK.RemoveAll();
	CArray<T_SECF_K, T_SECF_K>  aSecfK; aSecfK.RemoveAll();
	CArray<T_SECF_D, T_SECF_D&> aSecfD; aSecfD.RemoveAll();
	CArray<T_CSCS_K, T_CSCS_K>  aCscsK; aCscsK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsK);	
	CMap<T_STAG_K, T_STAG_K, T_STAG_D, T_STAG_D> mStage;

	CArray<T_STAG_K, T_STAG_K>  aStagK_Mod; aStagK_Mod.RemoveAll(); // Modify DB
	CArray<T_STAG_D, T_STAG_D&> aStagD_Mod; aStagD_Mod.RemoveAll(); // Modify DB
	
	m_pDoc->m_pAttrCtrl->GetStagKeyList(aStagK);
	for(int i=0; i<aStagK.GetSize(); ++i)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStag(aStagK[i], StagD)) continue;
		mStage.SetAt(aStagK[i], StagD);
	}

	for(int i=0; i<aCscsK.GetSize(); ++i)
	{
		BngrD.Initialize();
		CscsD.Initialize();
		
		if(!m_pDoc->m_pAttrCtrl->GetCscs(aCscsK[i], CscsD)) continue;
		if(CscsD.nCompType != -2) continue; // Normal Type 만 허용
		if(CscsD.aPart.GetSize() != 2) continue; // Part Size 는 2개 만 허용 한다.
		
		m_pDoc->m_pAttrCtrl->GetSecfSect(CscsD.SectKey, aSecfK, aSecfD);
		
		for(int j=0; j<aSecfD.GetSize(); ++j)
		{
			if(!m_pDoc->m_pAttrCtrl->GetBngr(aSecfD[j].GroupKey, BngrD)) continue; // Boundary Group 이 설정되어 있지 않으면 패쓰
			
			if(BngrD.nAutoType != 0) continue; // Creep Group 이 아니면 패쓰
			
			// Target이 되는 Boundary Group Stage 에 저장되어 있으면 삭제 한다.
			m_pDoc->m_pAttrCtrl->GetStagKeyList(aStagK);
			
			for(int k=0; k<aStagK.GetSize(); ++k)
			{
				if(!mStage.Lookup(aStagK[k], StagD)) continue;
				
				for(int l=0; l<StagD.aActiveBndr.GetSize(); ++l)
				{
					if(StagD.aActiveBndr[l].BngrKey == aSecfD[j].GroupKey)
					{
						StagD.aActiveBndr.RemoveAt(l);
						break;
					}
				}
				
				for(int l=0; l<StagD.aDeactiveBndr.GetSize(); ++l)
				{
					if(StagD.aDeactiveBndr[l] == aSecfD[j].GroupKey)
					{
						StagD.aDeactiveBndr.RemoveAt(l);
						break;
					}
				}

				mStage.SetAt(aStagK[k], StagD);
			}
			
			// Target 이 되는 Stage 가져온다.
			//if(!m_pDoc->m_pAttrCtrl->GetStag(CscsD.aPart[1].CompStagKey, StagD)) continue;
			if(!mStage.Lookup(CscsD.aPart[1].CompStagKey, StagD)) continue;
			
			T_STAG_BNDR_BASE BndrBase; BndrBase.Initialize();
			BndrBase.BngrKey = aSecfD[j].GroupKey;
			BndrBase.nPosition = 2;
			
			StagD.aActiveBndr.Add(BndrBase);
			
			mStage.SetAt(CscsD.aPart[1].CompStagKey, StagD);
		}		
	}

	T_STAG_D StagD_Tmp; StagD_Tmp.Initialize();

	for(int i=0; i<aStagK.GetSize(); ++i)
	{
		if(!mStage.Lookup(aStagK[i], StagD)) continue;

		StagD_Tmp = StagD;
		StagD.aActiveBndr.RemoveAll();

		// 정렬 하자
		CArray<T_BNGR_K, T_BNGR_K> aSortBngrK;

		for(int j=0; j<StagD_Tmp.aActiveBndr.GetSize(); ++j)
		{
			aSortBngrK.Add(StagD_Tmp.aActiveBndr[j].BngrKey);
		}

		qsort(aSortBngrK.GetData(), aSortBngrK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

		for(int j=0; j<aSortBngrK.GetSize(); ++j)
		{
			for(int k=0; k<StagD_Tmp.aActiveBndr.GetSize(); ++k)
			{
				if(aSortBngrK[j] == StagD_Tmp.aActiveBndr[k].BngrKey)
				{
					StagD.aActiveBndr.Add(StagD_Tmp.aActiveBndr[k]);
				}			
			}
		}

		aStagK_Mod.Add(aStagK[i]);
		aStagD_Mod.Add(StagD);
	}
	
	m_pDoc->m_pDataCtrl->ModifyStag(aStagK_Mod, aStagD_Mod);
}

//--------------------------------------------------------------------------
// OnItemchangedCmdCsSectList  Item 선택이 바뀌었을 때 - View Update
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnItemchangedCmdCsSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// TODO: Add your control notification handler code here
	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

//  T_SECT_D Data;
	if (iItem == -1) 
		m_SectData.Initialize();
	else
	{
		T_CSCS_K CscsK;
		T_CSCS_D CscsD;
		CscsK = m_ctrlList.GetItemData(iItem);
		if(!m_pDoc->m_pAttrCtrl->GetCscsOrCscv(CscsK, CscsD, m_nSectType==1)) return;
		if(!m_pDoc->m_pAttrCtrl->GetSectOrSecv(CscsD.SectKey, m_SectData, m_nSectType==1)) return;
	}
	m_ctrlViewFrm.SetDataSource(&m_SectData);
	m_ctrlViewFrm.Invalidate();
	
	*pResult = 0;
}

//------------------------------------------------------------------------
// OnUpdate   DB Update - List 재작성 
//--------------------------------------------------------------------------
void CCMStageCompSectListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateItemList();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateItemList();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMStageCompSectListDlg::OnDblclkCmdCsSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_CSCS_K Key;
	T_CSCS_D Data;

	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
 
	Key = m_ctrlList.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetCscsOrCscv(Key, Data, m_nSectType==1)) return;

	CCMStageCompSectItemDlg dlg;
	dlg.SetParam(Key, Data);
	dlg.SetVirtualType(m_nSectType == 1);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_ctrlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	*pResult = 0;
}

void CCMStageCompSectListDlg::OnSectType()
{
	UpdateData(TRUE);

	InitSection();
	UpdateItemList();
}