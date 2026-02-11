// StldCnldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldLaplDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\NumericOptimizer.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\CompFunc.h"
#include "..\wg_base\wg_base_strParser.h"

#include "..\wg_gr\GDisplaySetting.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldCnldDlg dialog

#define ELEM_FRAME  1
#define ELEM_PLANAR 2

CStldLaplDlg::CStldLaplDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldLaplDlg::IDD, pParent)
{
	m_nOption = -1;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bIgnoreMsg = FALSE;
}

void CStldLaplDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio(pDX,   IDC_TM_NL_OPT_ADD,       m_nOption);
	DDX_Control(pDX, IDC_TM_LAPL_CBO_LAGR,    m_wndLagrCobx);
	DDX_Control(pDX, IDC_TM_LAPL_ELEM_LIST,   m_edElemList);
	DDX_Control(pDX, IDC_TM_LIST,             m_List);  
}

void CStldLaplDlg::Execute()
{
	OnTmExecute();
}

BEGIN_MESSAGE_MAP(CStldLaplDlg, CMenuBarChildDlg)
//{{AFX_MSG_MAP(CStldLaplDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_LIST,  OnChangedItemSelection)
	ON_BN_CLICKED(IDC_TM_EXECUTE,            OnTmExecute        )
	ON_BN_CLICKED(IDC_TM_CLOSE,              OnTmClose          )
	ON_BN_CLICKED(IDC_TM_NL_OPT_ADD,         OnChangeOption     )
	ON_BN_CLICKED(IDC_TM_NL_OPT_REPLACE,     OnChangeOption     )
	ON_BN_CLICKED(IDC_TM_NL_OPT_DELETE,      OnChangeOption     )
	ON_BN_CLICKED(IDC_TM_LAPL_ELEM_TYPE1_RDO,OnChangeElemType   )
	ON_BN_CLICKED(IDC_TM_LAPL_ELEM_TYPE2_RDO,OnChangeElemType   )
	ON_BN_CLICKED(IDC_TM_DEFINE_LAGR_BTN,    OnTmDefineLagrBtn  )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldLaplDlg message handlers

BOOL CStldLaplDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
	// TODO: Add extra initialization here

	m_nOption = 0;

	UpdateData(FALSE);
	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);	

	SetListCtrlHeader();

	MakeItemList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldLaplDlg::OnTmExecute()
{
	UpdateData(TRUE);

	if(m_nOption == 0) //ADD
	{
		OnCmdAdd();
	}
	else if(m_nOption == 1) //Replace
	{
		OnCmdReplace();
	}
	else if(m_nOption == 2) //Delete
	{
		OnCmdDelete();
	}
	else
		ASSERT(0);
}

void CStldLaplDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldLaplDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CStldLaplDlg::OnChangeElemType() 
{
	MakeItemList();
}

void CStldLaplDlg::OnChangeAssignType()
{
	UpdateData(TRUE);
}

void CStldLaplDlg::OnTmDefineLagrBtn() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc, CCMLdarGrupDefDlg::IDD);		
}

void CStldLaplDlg::OnChangedItemSelection( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	BOOL bNoOp = TRUE;
	if (pNMLV->uChanged & LVIF_STATE)
	{
		if(pNMLV->uNewState & LVIS_FOCUSED)                                bNoOp = FALSE;
		else if(pNMLV->uOldState & LVIS_SELECTED && pNMLV->uNewState == 0) bNoOp = FALSE;
		else if(pNMLV->uOldState == 0 && pNMLV->uNewState & LVIS_SELECTED) bNoOp = FALSE;
	}

	if(m_bIgnoreMsg || bNoOp)
	{
		*pResult = 0;
		return ;
	}

	SetLaplDisplay();
	*pResult = 0;  
}

void CStldLaplDlg::SetListCtrlHeader()
{
	CStringArray aTitles;
	CArray<float,float> aWRatio;
	aTitles.Add(_LS(IDS_WG_TREEMENU_LAPL_Index));
	aTitles.Add(_LS(IDS_WG_TREEMENU_ADD3_Group));  
	aTitles.Add(_LS(IDS_WG_TREEMENU_Element_List));
	CSize size;
#pragma warning( disable : 4305)
	aWRatio.Add((float)0.17);
	aWRatio.Add((float)0.30);
	aWRatio.Add((float)0.44);
#pragma warning( default : 4305 )

	CDlgUtil::_SetListCtrlHeader(&m_List, aTitles, &aWRatio, NULL);
}

void CStldLaplDlg::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_AFTER:
		MakeItemList();
		break;
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		break;
	default:
		//ASSERT(FALSE);
		break;
	}  
}

void CStldLaplDlg::MakeItemList()
{
	int nElemType = GetElementType();

	m_List.DeleteAllItems();
	if(m_pDoc->m_pAttrCtrl2 == NULL)
		return;
	
	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountLapl();
	if(nItemCount == 0) return;
	
	T_LAPL_K Key;
	T_LAPL_D Data;
	CArray<UINT, UINT> KeyBuffer;
	POSITION Pos;
	Pos = m_pDoc->m_pAttrCtrl2->GetStartLapl();
	while(Pos != NULL)
	{
		m_pDoc->m_pAttrCtrl2->GetNextLapl(Pos, Key, Data);
		KeyBuffer.Add(Key);
	}
	
	int nIndex = 1;
	int nSize = KeyBuffer.GetSize();
	CStringArray Contents;
	CString      TStr;
	CNumericOptimizer optimizer;
	T_LAPL_D     LcData  ; // Load Case   
	T_LAGR_D     LagrData; // Load Group Data

	if(nSize)
	{
		m_bIgnoreMsg = TRUE;
		qsort(KeyBuffer.GetData(),nItemCount,sizeof(T_LAPL_K), CCompFunc::UINTAsc);

		T_ELEM_D ElemD; ElemD.Initialize();
		for(int i = 0 ; i < nSize; i++)
		{
			if(!m_pDoc->m_pAttrCtrl2->GetLapl(KeyBuffer[i], Data)) continue;
			if(Data.arKeyElem.GetSize()<1) { ASSERT(0); continue; }
			if(!m_pDoc->m_pAttrCtrl->GetElem(Data.arKeyElem[0],ElemD)) { ASSERT(0); continue; }
			if(nElemType==ELEM_FRAME&&m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp)) continue;
			else if(nElemType==ELEM_PLANAR&&m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp)) continue;
			
			Contents.RemoveAll();
			TStr.Format(_T("%d"), nIndex++);
			Contents.Add(TStr);

			if(Data.LagrK > 0)
			{
				m_pDoc->m_pAttrCtrl2->GetLagr(Data.LagrK, LagrData);
				Contents.Add(LagrData.GroupName);
			}
			else
			{
				Contents.Add(_LS(IDS_DB_GROUP_NAME_DEFAULT));
			}
			TStr = optimizer.Optimize((long*)Data.arKeyElem.GetData(),Data.arKeyElem.GetSize());
			Contents.Add(TStr);

			CDlgUtil::SetListItem(&m_List, nIndex-2, Contents, (DWORD)KeyBuffer[i]);
	  }
	}
	m_bIgnoreMsg = FALSE;
}

BOOL CStldLaplDlg::SetDataInDlg()
{
	int nSize, i, j;
	nSize = GetSelectedLapl(m_arKeyLapl);

	//선택요소를 지운다.
	m_arKeyElem.RemoveAll();

	//   if(nSize == 1) //1개일때만 요소를 선택하자.
	//   {  
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K&> mapElem;
	mapElem.RemoveAll();
	mapElem.InitHashTable(m_pDoc->GetHashSizeElem());


	T_LAPL_D LaplD;
	for(i = 0 ; i < nSize; i++)
	{
		if(m_pDoc->m_pAttrCtrl2->GetLapl(m_arKeyLapl[i], LaplD))
		{
			for(j = 0 ; j < LaplD.arKeyElem.GetSize(); j++)
			{
				mapElem.SetAt(LaplD.arKeyElem[j], LaplD.arKeyElem[j]);
			}
		}
	}

	T_ELEM_K ElemK1;
	T_ELEM_K ElemK2;
	POSITION pos = mapElem.GetStartPosition();
	while(pos)
	{
		mapElem.GetNextAssoc(pos, ElemK1, ElemK2);
		m_arKeyElem.Add(ElemK1);

	}
	//}

	return TRUE;
}

BOOL CStldLaplDlg::Dlg2Data()
{
// 	CString StrText;
// 	m_edElemList.GetWindowText(StrText);
// 
// 	CArray<int, int> aElem;
// 	int nLength = StrText.GetLength();
// 	if(nLength >= 1023)
// 	{
// 		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_LAPL_TOOMUCH_ELEM));
// 		return FALSE;
// 	}
// 
// 	if(CStrParser::ParsingListByTo(StrText, aElem))

	// [PMS 5011-6] 요소 많이 선택 시 문자열->요소번호 파싱하다 프로그램 다운
	CArray<T_ELEM_K, T_ELEM_K> aElem;  
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pViewCtrl->GetAllSelectedElem(aElem);
	if(!aElem.IsEmpty())
	{
		int i;
		int nSize = aElem.GetSize();

		if(nSize <= 0)
			return FALSE;

		m_arKeyElem.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			m_arKeyElem[i] = aElem[i];
		}
	}
	if(!m_wndLagrCobx.GetSelectedLagr(m_LagrK))
		return FALSE;

	return TRUE;
}

BOOL CStldLaplDlg::OnCmdDelete()
{
	CArray<DWORD, DWORD> arRetData;
	CArray<T_LAPL_K, T_LAPL_K> rDelKey;
	if(!CDlgUtil::GetSelectedListItemData(&m_List, arRetData))
		return FALSE;

	int nSize, i;
	nSize = arRetData.GetSize();
	rDelKey.SetSize(nSize);

	for(i = 0 ; i < nSize; i++)
	{
		rDelKey[i] = arRetData[i];
	}

	return m_pDoc->m_pDataCtrl->DelLapl(rDelKey);
}

BOOL CStldLaplDlg::OnCmdAdd()
{
	int nElemType = GetElementType();
	CArray<T_LAPL_D, T_LAPL_D&> arLaplData;
	int nSize = 0;
	BOOL bisRemoved = FALSE;
	if(nElemType==ELEM_PLANAR)
	{
		nSize = MakeLapl4Planar(arLaplData, bisRemoved);
	}
	else
	{
		nSize = MakeLapl(arLaplData, bisRemoved);
	}

	if(nSize > 0)
	{
		if(!m_pDoc->m_pDataCtrl->AddLapl(arLaplData)) return FALSE;
	}

	if(bisRemoved) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_LAPL_EXCLUDE));

	return TRUE; 
}

int CStldLaplDlg::GetSelectedLapl( CArray<T_LAPL_K, T_LAPL_K>& arKeyLapl )
{
	arKeyLapl.RemoveAll();
	CArray<DWORD, DWORD> arRetData;

	if(!CDlgUtil::GetSelectedListItemData(&m_List, arRetData))
		return 0;

	int nSize, i;
	nSize = arRetData.GetSize();  

	if(nSize > 0)
	{
		arKeyLapl.SetSize(nSize);    
		for(i = 0 ; i < nSize; i++)
		{
			arKeyLapl[i] = arRetData[i];
		}
	}

	return nSize;
}

BOOL CStldLaplDlg::OnCmdReplace()
{
	if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_LAPL)), CMDTYPE_REMOVE_ANALYSIS))
		return FALSE;

	BOOL bIsRemoved=FALSE;
	try
	{
		int nSize, i;
		CArray<DWORD, DWORD> arRetData;

		if(!CDlgUtil::GetSelectedListItemData(&m_List, arRetData))
			throw 0;

		//선택된 녀석들을 지우고 만든다.(중복성 검사를 위해서 일단 지워야함.)
		nSize = arRetData.GetSize();    
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pDoc->m_pEditData->DelLapl((T_LAPL_K)arRetData[i]))
			{
				throw 0;
			}
		}

		int nElemType = GetElementType();
		CArray<T_LAPL_D, T_LAPL_D&> arLaplData;
		if(nElemType==ELEM_PLANAR)
		{
			nSize = MakeLapl4Planar(arLaplData, bIsRemoved);
		}
		else
		{
			nSize = MakeLapl(arLaplData, bIsRemoved);
		}

		if(nSize <= 0) throw 0;

		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pDoc->m_pEditData->AddLapl(arLaplData[i])) throw 0;
		}
	}
	catch (...)
	{
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB();

	if(bIsRemoved) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_LAPL_EXCLUDE));

	return TRUE;
}

int CStldLaplDlg::MakeLapl( CArray<T_LAPL_D, T_LAPL_D&>& arLaplData, BOOL& bIsRemoved )
{
	bIsRemoved = FALSE;
	arLaplData.RemoveAll();

	if(!Dlg2Data())
		return arLaplData.GetSize();

	int i, nSize;

	BOOL bTEST = FALSE;
	if(bTEST)
	{
		nSize = 100;
		arLaplData.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			arLaplData[i].Initialize();
			arLaplData[i].LagrK = 0;
			arLaplData[i].arKeyNode.SetSize(3);
			arLaplData[i].arKeyElem.SetSize(3);
			arLaplData[i].arKeyNode[0] = 1;
			arLaplData[i].arKeyNode[1] = 2;
			arLaplData[i].arKeyNode[2] = 3;
			arLaplData[i].arKeyElem[0] = 1;
			arLaplData[i].arKeyElem[1] = 2;
			arLaplData[i].arKeyElem[2] = 3;
		}

		return nSize;
	}

	CArray<T_ELEM_K, T_ELEM_K> arElemKey;
	CArray<T_NODE_K, T_NODE_K> arNodeKey;
	CArray<UINT, UINT> arSurfKey;
	CWindLoadGeneratorCtrl WindLoadSpaceCtrl;
	try
	{
		if(!WindLoadSpaceCtrl.RunFindClosedArea(m_arKeyElem, TRUE))
		{
			throw 0;
		}
	}
	catch (...)
	{
		arLaplData.RemoveAll();
		return arLaplData.GetSize();
	}

	nSize = WindLoadSpaceCtrl.GetSurfKeyList(arSurfKey);  
	arLaplData.SetSize(nSize);
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		if(WindLoadSpaceCtrl.GetSurfInfo(arSurfKey[i], arNodeKey, arElemKey))
		{
			arLaplData[i].Initialize();
			arLaplData[i].LagrK = m_LagrK;
			arLaplData[i].arKeyNode.Copy(arNodeKey);
			arLaplData[i].arKeyElem.Copy(arElemKey);
		}
		else
		{
			arLaplData.RemoveAll();
			break;
		}
		if(!CheckLaplData_Dupl(arLaplData[i]))
			arLaplData.RemoveAt(i);
	}

	if(nSize != arLaplData.GetSize())
		bIsRemoved = TRUE;

	return arLaplData.GetSize();  
}

int CStldLaplDlg::MakeLapl4Planar(CArray<T_LAPL_D, T_LAPL_D&>& arLaplData, BOOL& bIsRemoved)
{
	if(!Dlg2Data()) return FALSE;

	bIsRemoved = FALSE;
	
	T_ELEM_D ElemD; ElemD.Initialize();
	for(int i=m_arKeyElem.GetSize()-1; i>=0; i--)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(m_arKeyElem[i],ElemD)) { ASSERT(0); return FALSE; }
		if(!m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)&&!m_pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) m_arKeyElem.RemoveAt(i);
	}
	
	int nSize = m_arKeyElem.GetSize();
	arLaplData.SetSize(nSize);
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		arLaplData[i].Initialize();
		arLaplData[i].LagrK = m_LagrK;
		arLaplData[i].arKeyElem.Add(m_arKeyElem[i]);

		// MQC 5261-14 : Planar 타입일 때 절점정보 저장함. CAttrCtrl2::GetWdprAverageWindVec() 등에서 사용
		if(!m_pDoc->m_pAttrCtrl->GetElem(m_arKeyElem[i],ElemD)) continue;
		for(int j=0; j<4; j++)
		{
			if(ElemD.elnod[j]!=0)
				arLaplData[i].arKeyNode.Add(ElemD.elnod[j]);
		}

		if(!CheckLaplData_Dupl(arLaplData[i])) arLaplData.RemoveAt(i);
	}

	if(nSize != arLaplData.GetSize())
		bIsRemoved = TRUE;

	return arLaplData.GetSize();
}

BOOL CStldLaplDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		CWnd* pWnd = GetFocus();
		if(pMsg->wParam == VK_DELETE && pWnd->GetSafeHwnd() == m_List.GetSafeHwnd())
		{
			OnCmdDelete();
			return TRUE;
		}
	}
	return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CStldLaplDlg::SetLaplDisplay()
{
	//1. 요소를 선택한다.
	SetDataInDlg();

	int nSize, i;
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);

	nSize = m_arKeyLapl.GetSize();

	//2. 현재 선택한 Lapl를 Display 한다.  
	T_LAGR_K LagrKTemp = 0;

	if(m_arKeyElem.GetSize() > 0)
		m_pDoc->m_pViewCtrl->SelectElem(NULL, m_arKeyElem, FALSE, TRUE);
	else
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	if(nSize > 0)
	{
		dspOpt.m_Load.nTypeLaplLagr = 1;
		dspOpt.m_Load.m_LoadingAreaKeys.SetSize(nSize);

		for(i = 0 ; i < nSize; i++)
		{
			dspOpt.m_Load.m_LoadingAreaKeys[i] = m_arKeyLapl[i];
		}
		T_LAPL_D LaplD;
		if(m_pDoc->m_pAttrCtrl2->GetLapl(m_arKeyLapl[0], LaplD))
			LagrKTemp =LaplD.LagrK;
	}
	else
	{
		dspOpt.m_Load.nTypeLaplLagr = 0;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	}
	ExportDisplaySetting::SET(&dspOpt);

	//3, Lagr를 선택한다.
	m_LagrK = LagrKTemp;
	m_wndLagrCobx.ChangeSelect(m_LagrK);
}

BOOL CStldLaplDlg::CheckLaplData_Dupl(const T_LAPL_D& rData)
{
	POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartLapl();
	T_LAPL_K LaplK;
	T_LAPL_D LaplD;
	int i, j;
	int nSize = rData.arKeyElem.GetSize();
	BOOL bSame;

	while(pos)
	{
		m_pDoc->m_pAttrCtrl2->GetNextLapl(pos, LaplK, LaplD);
		if(LaplD.arKeyElem.GetSize() == nSize)
		{
			for(i = 0 ; i < nSize; i++)
			{
				bSame = FALSE;
				for(j = 0 ; j < nSize; j++)
				{
					if(LaplD.arKeyElem[i] == rData.arKeyElem[j])
					{
						bSame = TRUE;
						break;
					}
				}
				if(!bSame) break;
			}
			if(bSame) return FALSE; //똑같은 Lapl이 존재한다.
		}
	}
	return TRUE;  
}

int CStldLaplDlg::GetElementType()
{
	int nElemType = ELEM_FRAME;
	if(((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->GetCheck())
		nElemType = ELEM_FRAME;
	else if(((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->GetCheck())
		nElemType = ELEM_PLANAR;
	else 
	{
		nElemType = ELEM_FRAME;
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->SetCheck(FALSE);
	}
	return nElemType;  
}
