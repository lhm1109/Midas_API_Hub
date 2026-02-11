// InelasticHingeAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "InelasticHingeAssignDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_common\wg_common_Query.h"

#include "..\wg_main\wg_mainRes2.h"

#include "InelasticHingeImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg dialog

//IMPLEMENT_DYNCREATE(CInelasticHingeAssignDlg, CCMChildBarBase)

CInelasticHingeAssignDlg::CInelasticHingeAssignDlg()
	: CCMChildBarBase(CInelasticHingeAssignDlg::IDD)
{
	//{{AFX_DATA_INIT(CInelasticHingeAssignDlg)
	m_nElemType = ET_BEAM;
	m_nAddDelete = 0;
	m_strMembLoc = _T("");
	m_strMatl = _T("");
	m_strMembType = _T("");
	//}}AFX_DATA_INIT

	m_Data.Initialize();

	m_hEdtBrush = NULL;
}

CInelasticHingeAssignDlg::~CInelasticHingeAssignDlg()
{
	if (m_hEdtBrush)
	{
		::DeleteObject(m_hEdtBrush);
		m_hEdtBrush = NULL;
	}
}


void CInelasticHingeAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInelasticHingeAssignDlg)
	DDX_Control(pDX, IDC_INELASTIC_HINGE_PROPERTY_CMB, m_cmbHingeProp);
	DDX_Radio(pDX, IDC_INELASTIC_ELEMTYPE_TRUSS, m_nElemType);
	DDX_Radio(pDX, IDC_INELASTIC_ADD, m_nAddDelete);
	DDX_Control(pDX, IDC_SECT_NAME_STC, m_edtSectThik);

	//DDX_Text(pDX, IDC_MATL_NAME_STC, m_strMatl);
	//DDX_Text(pDX, IDC_MEMB_LOCA_STC, m_strMembLoc);
	//DDX_Text(pDX, IDC_MEMB_TYPE_STC, m_strMembType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInelasticHingeAssignDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CInelasticHingeAssignDlg)
	ON_CBN_SELCHANGE(IDC_INELASTIC_HINGE_PROPERTY_CMB, OnSelchangeInelasticHingePropCmb)
	ON_BN_CLICKED(IDC_INELASTIC_HINGE_PROPERTY_BTN, OnInelasticHingePropertyBtn)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_BEAM, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ADD, OnInelasticAddDelete)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_TRUSS, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_WALL, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_DELETE, OnInelasticAddDelete)
	ON_BN_CLICKED(IDC_SELECT_BTN, OnSelectBtn)
	ON_BN_CLICKED(IDC_CMD_IMPORT_HINGE_BTN, OnImportHingeData)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg message handlers

BOOL CInelasticHingeAssignDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();

	MakeIehpCombo(m_nElemType);
	OnSelchangeInelasticHingePropCmb();

	m_hEdtBrush = CreateSolidBrush(RGB(233, 235, 239));

#if defined(_CIVIL)
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->ShowWindow(FALSE);
#else
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif
	GetDlgItem(IDC_CMD_APPLY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(SW_HIDE);
return TRUE;  // return TRUE unless you set the focus to a control
	            // EXCEPTION: OCX Property Pages should return FALSE
}

void CInelasticHingeAssignDlg::UpdateIehpCombo()
{
	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel == CB_ERR)
	{
		MakeIehpCombo(m_nElemType);
	}
	else
	{
		UINT key = m_cmbHingeProp.GetItemData(nSel);
		MakeIehpCombo(m_nElemType);
		ChangeIehpComboSel(key);
		OnSelchangeInelasticHingePropCmb();
	}
}

void CInelasticHingeAssignDlg::MakeIehpCombo(int nElemType/*=-1*/)
{
	m_cmbHingeProp.ResetContent();

	CArray<T_IEHP_K, T_IEHP_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetIehpKeyList(rKeyList);

	int nSize = rKeyList.GetSize();
	if (nSize < 1)
		return;

	int nIndex;
	T_IEHP_D IehpD;
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetIehp(rKeyList[i], IehpD);
		if (IehpD.nHingeType == 2) continue;

		// MQC 18093 : 선택한 옵션으로 항목 필터
		if(m_nElemType == ET_TRUSS && IehpD.nHingeType!=3) continue;
		if(m_nElemType == ET_BEAM  && (IehpD.nHingeType!=0 && IehpD.nHingeType!=1)) continue;
		if(m_nElemType == ET_WALL  && IehpD.nHingeType!=4) continue;

		nIndex = m_cmbHingeProp.AddString(IehpD.strName);
		m_cmbHingeProp.SetItemData(nIndex, rKeyList[i]);
	}
}

void CInelasticHingeAssignDlg::ChangeIehpComboSel(UINT key)
{
	int nSize = m_cmbHingeProp.GetCount();

	UINT cmbKey;
	for (int i=0; i<nSize; i++)
	{
		cmbKey = m_cmbHingeProp.GetItemData(i);

		if (cmbKey == key)
		{
			m_cmbHingeProp.SetCurSel(i);
			break;
		}
	}
}

void CInelasticHingeAssignDlg::Execute() 
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));  // No element is selected
		return;
	}

	CArray<T_ELEM_K, T_ELEM_K> aSelKey1, aSelKey2, aSelKey3;
	
	if(m_nAddDelete == 0)// Add Or Replace, Filtering by Element type
	{
		switch(m_nElemType)
		{
			case ET_TRUSS:// Truss
				aSelKey1.Copy(aSelKey); aSelKey2.Copy(aSelKey); aSelKey3.Copy(aSelKey);
				aSelKey.RemoveAll();
				GetElementByKind(aSelKey1, TRUSS_EL); aSelKey.Append(aSelKey1);
				GetElementByKind(aSelKey2, TRTENS_EL); aSelKey.Append(aSelKey2);
				GetElementByKind(aSelKey3, TRCOMP_EL); aSelKey.Append(aSelKey3);

				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Truss_is_selected__));
					return;
				}
				break;
			case ET_BEAM:// Beam
				GetElementByKind(aSelKey,BEAM_EL);
				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Beam_is_selected__));
					return;
				}
				break;
			case ET_WALL:// Wall
				{
					GetElementByKind(aSelKey,WALL_EL);
					if (aSelKey.GetSize() == 0) 
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Wall_is_selected__));
						return;
					}
					BOOL bRet = m_pDoc->m_pAttrCtrl->MakeRootWallElemKeyList(aSelKey);	//Root Elem 들만 aElemKeyList에 저장한다.
					if(!bRet)
					{
						GSaveHistoryFormatNF(_LS(IDS_DB_POSTCTRL_____MakePostData_WALL_STORYERROR)); // _T("층정보가 없어 할당이 되지 않습니다.");
						return;
					}

					//CDBDoc* pDoc = CDBDoc::GetDocPoint();
					//CStoryData StoryData(m_pDoc);
					//StoryData.MakeStoryData(1);
					CStoryData* pStoryData = m_pDoc->GetStoryData();

					for(int i=0; i<aSelKey.GetSize(); i++)
					{
						CArray<T_ELEM_K,T_ELEM_K> SortedWallIdElemList;
						CArray<T_ELEM_K,T_ELEM_K> ConnectedLinearElemList;
						CArray<T_ELEM_K,T_ELEM_K> CoordiLinearElemList;

						m_pDoc->m_pAttrCtrl->GetEqualWallIdElemList(SortedWallIdElemList,aSelKey[i], pStoryData);
						double dCompareLevel = -1;
						for(int j =0; j<SortedWallIdElemList.GetSize(); j++)
						{
							T_STOR_K StorK;
							T_STOR_D StorD;
							m_pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK,SortedWallIdElemList[j]);
							m_pDoc->m_pAttrCtrl->GetStor(StorK,StorD);
							if(dCompareLevel == -1)
								dCompareLevel = StorD.dStoryLevel;
							else if (dCompareLevel != StorD.dStoryLevel)
							{
								//error = 같은 층, 같은 Wall ID로 정렬 한 결과 중, 층 Level 이 다를 경우
								m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_DiffLevel));
								return;
							}
						}
						m_pDoc->m_pAttrCtrl->GetConnectedLinearElem(ConnectedLinearElemList,aSelKey[i],SortedWallIdElemList);
						if(ConnectedLinearElemList.GetSize() != SortedWallIdElemList.GetSize())
						{
							//error = 연결이 안되어 있는 경우 (중간중간이 빌 경우)
							m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Connected));
							return;
						}
						m_pDoc->m_pAttrCtrl->GetEqualCoordLinearElem(ConnectedLinearElemList, CoordiLinearElemList);
						if(CoordiLinearElemList.GetSize() != ConnectedLinearElemList.GetSize())
						{
							//error = 연결은 되어있는지 꺾여서 연결되는 경우
							m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Coordi));
							return;
						}
					}
				}
				break;
		}
	}


	BOOL bSuccess=FALSE;
	int nConvElemType = 0;
	if (m_nAddDelete == 0) // add or replace
	{		
		T_IEHP_K KeyIehp;
		T_IEHP_D DataIehp;
		T_IEHG_D DataIehg;
		DataIehp.Initialize();
		
		//if(!m_cmbHingeProp.GetSelectedIehp(KeyIehp)) return;
		int nSel = m_cmbHingeProp.GetCurSel();
		if (nSel==CB_ERR) return;
		KeyIehp = m_cmbHingeProp.GetItemData(nSel);
		if(!m_pDoc->m_pAttrCtrl->GetIehp(KeyIehp, DataIehp)) return;
	
		if(m_nElemType == ET_TRUSS) // Truss
		{
			if(DataIehp.nHingeType != 3)
				nConvElemType = D_IEHG_TYPE_TRUSS;

			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
				return;
			}
		}
		else if(m_nElemType == ET_BEAM) // Beam
		{
			nConvElemType = D_IEHG_TYPE_BEAMCOLUMN;

			if(DataIehp.nHingeType != 0 && DataIehp.nHingeType != 1)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
				return;
			}
		}
		else if(m_nElemType == ET_WALL) // Wall(CRB)
		{
			nConvElemType = D_IEHG_TYPE_WALL;

			if(DataIehp.nHingeType != 4)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
				return;
			}

			// 요소와 비탄성힌지의 벽체 타입(Membrace/Plate)이 같은지 검사
			T_ELEM_D ElemD;
			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i],ElemD);
				if(ElemD.nSubType-1!=DataIehp.nWallType)
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_WALL_TYPE_DIFFERENT), aSelKey[i]);
					return;
				}
			}

		}
		DataIehg.IehpKey = KeyIehp;

		CArray<T_IEHG_K, T_IEHG_K> aIehgKey;
		const int nSelKSize = aSelKey.GetSize();
		T_IEHG_K IehgK;
		for (int i = 0; i < nSelKSize; i++)
		{
			IehgK.key.objK = aSelKey[i];
			IehgK.key.type = nConvElemType;
			aIehgKey.Add(IehgK);
		}

		bSuccess = m_pDoc->m_pDataCtrl->AddIehg(aIehgKey, DataIehg);
	}
	else if (m_nAddDelete == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelIehg(nConvElemType, aSelKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

void CInelasticHingeAssignDlg::OnSelchangeInelasticHingePropCmb() 
{
	UpdateData(TRUE);

	int nCount = m_cmbHingeProp.GetCount();
	if(nCount <= 0)
	{
		GetDlgItem(IDC_SECT_NAME_TXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECT_NAME_STC)->EnableWindow(FALSE);
		GetDlgItem(   IDC_SELECT_BTN)->EnableWindow(FALSE);
		return;
	}

	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel==CB_ERR) 
	{
		m_cmbHingeProp.SetCurSel(0);
		nSel = 0;
	}
	T_IEHP_K key = m_cmbHingeProp.GetItemData(nSel);

	m_Data.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetIehp(key, m_Data)) return;

	const bool bEnableWindow = [this] ()
	{
		if ( m_Data.nFiberAutoUser == 0 ) // Auto
		{
			return false;
		}
		if ( m_Data.nDefinition == 1 ) // Fiber
		{
			return true;
		}

		return false;
	} ();

	GetDlgItem(IDC_SECT_NAME_TXT)->EnableWindow(bEnableWindow);
	GetDlgItem(IDC_SECT_NAME_STC)->EnableWindow(bEnableWindow);
	GetDlgItem(   IDC_SELECT_BTN)->EnableWindow(bEnableWindow);

	if(m_Data.nHingeType==4)
		GetDlgItem(IDC_SECT_NAME_TXT)->SetWindowText(_LS(IDS_CMD_THICKNESS_thickness));
	else
		GetDlgItem(IDC_SECT_NAME_TXT)->SetWindowText(_LS(IDS_CMD_CSCS__Section));
	

	if( bEnableWindow )
	{
		if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
		{
			T_SECT_D SectD; SectD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetSect(m_Data.SectKey, SectD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_SectionData_Invalid));
				return;
			}
			m_edtSectThik.SetWindowText(SectD.SName);
		}
		else
		{
			T_THIK_D ThikD; ThikD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetThik(m_Data.SectKey, ThikD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_ThicknessData_Invalid));
				return;
			}
			m_edtSectThik.SetWindowText(m_pDoc->m_pAttrCtrl->GetThikNameAuto(m_Data.SectKey));
		}
		m_edtSectThik.Invalidate(TRUE);
	}

	UpdateData(FALSE);
}

void CInelasticHingeAssignDlg::OnInelasticHingePropertyBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_ITHA_DEFINE_HINGE, 0));	
}

void CInelasticHingeAssignDlg::OnInelasticElemType() 
{	
	UpdateData(TRUE);

	MakeIehpCombo(m_nElemType);
	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel==CB_ERR) 
	{
		m_cmbHingeProp.SetCurSel(0);
	}
}

void CInelasticHingeAssignDlg::GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"),nElemType);
	CArray<UINT,UINT> ElemKeys;
	CArray<UINT,UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);
	
	QueryCtrl.SelectKeyFromElem(ElemKeys,QCond);
	QueryCtrl.IntersectKey(ElemKeys,SelKeys,aKey);

// CRB/Plate 구분하지 않고 비선형 해석 가능
//   if(nElemType == WALL_EL)
//   {
//     CArray<UINT,UINT> aWallKeyTemp;
//     aWallKeyTemp.Copy(aKey);
// 
//     aKey.RemoveAll();
//     T_ELEM_D ElemD;
//     for(int i = 0; i < aWallKeyTemp.GetSize(); i++)
//     {
//       if(!m_pDoc->m_pAttrCtrl->GetElem(aWallKeyTemp[i], ElemD)) continue;
//       if(ElemD.nWallType==1) aKey.Add(aWallKeyTemp[i]);
//     }
//   }
}	


void CInelasticHingeAssignDlg::OnInelasticAddDelete() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_BEAM)->EnableWindow(m_nAddDelete == 0);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_TRUSS)->EnableWindow(m_nAddDelete == 0); // MNET:XXXX-JWKWON-20140404 : MQC 7554
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->EnableWindow(m_nAddDelete == 0);
	GetDlgItem(IDC_INELASTIC_HINGE_PROPERTY_CMB)->EnableWindow(m_nAddDelete == 0);
	GetDlgItem(IDC_INELASTIC_HINGE_PROPERTY_BTN)->EnableWindow(m_nAddDelete == 0);

	if(m_nAddDelete == 0) OnSelchangeInelasticHingePropCmb();	
}


void CInelasticHingeAssignDlg::OnSelectBtn() 
{
	UpdateData(TRUE);
	T_ELEM_K ElemK;
	T_ELEM_D ElemD;
	T_MATL_D MatlD;

	if(m_Data.nDefinition == 1)
	{
		if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
		{
		  T_SECT_D SectD; SectD.Initialize();
		  if(!m_pDoc->m_pAttrCtrl->GetSect(m_Data.SectKey, SectD)) 
		  {
			  AfxMessageBox(_LS(IDS_CMD_ERROR_SectionData_Invalid));
			  return;
		  }
		  m_edtSectThik.SetWindowText(SectD.SName);
		}
		else
		{
			T_THIK_D ThikD; ThikD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetThik(m_Data.SectKey, ThikD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_ThicknessData_Invalid));
				return;
			}
			m_edtSectThik.SetWindowText(m_pDoc->m_pAttrCtrl->GetThikNameAuto(m_Data.SectKey));
		}
	}

	CArray<T_ELEM_K, T_ELEM_K> aElemK, aMatlElemK, aSectElemK, aSelKey;
	aElemK.RemoveAll(); aMatlElemK.RemoveAll(); aSectElemK.RemoveAll(); aSelKey.RemoveAll();
	
	if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
	{
		CArray<T_SECT_K, T_SECT_K> aSectK;
		aSectK.Add(m_Data.SectKey);
		m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectK, NULL, &aElemK);
	}
	else
	{
		CArray<T_THIK_K, T_THIK_K> aThikK;
		aThikK.Add(m_Data.SectKey);
		m_pDoc->m_pAttrCtrl->GetKeyListThikUser(&aThikK, NULL, &aElemK);
	}

	//m_pDoc->m_pAttrCtrl->GetElemKeyListMatlSectUser(NULL, m_Data.SectKey, &aElemK);

	int nSize = aElemK.GetSize();
	if(nSize<1) 
	{
		AfxMessageBox(_LS(IDS_DB_NO_ELEMENT));
		return;
	}
	for(int i=0; i<nSize; i++)
	{
		ElemD.Initialize();
		ElemK = aElemK[i];
		if(m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
			if(TransMembType(nMembType)<0) { ASSERT(0); return; }
			if(nMembType == 0) // Beam
			{
				// 멤버냐?
				T_MEMB_K MembK;
				if(m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK))
				{
					//공구리냐?
					if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) { ASSERT(0); return; }
					if(MatlD.Type == _T("C"))
					{
						T_MEMB_D MembD; MembD.Initialize();
						if(m_pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))
						{
							if(MembD.aElemList.GetSize()<=1) aSelKey.Add(ElemK); // Single element member는 location무시
							else
							{
								CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
								int nPos = pMembCtrl->GetMembPosiByElem(ElemK);
								if(nPos == m_Data.nLocation) aSelKey.Add(ElemK); 
							}
						}
					}
					else aSelKey.Add(ElemK);// beam인데 공구리가 아니면 location 고려안함
				}
				else aSelKey.Add(ElemK);// Member에 assign되지 않았다(single element member로 간주)
			}
			else aSelKey.Add(ElemK); // Beam이 아니면 member location무시
		}
	}
	// Element Type이 맞지 않은것은 뺀다. 
	// 속도향상 고려
	int nTgtElemType;
	switch(m_nElemType)
	{
	case ET_TRUSS: nTgtElemType = TRUSS_EL; break;
	case ET_BEAM : nTgtElemType = BEAM_EL; break;
	case ET_WALL : nTgtElemType = WALL_EL; break;
	}
	
	CArray<T_ELEM_K, T_ELEM_K> aTgtElemK;
	aTgtElemK.SetSize(aSelKey.GetSize());
	int nCnt = 0;
	for (int n = 0; n < aSelKey.GetSize(); n++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[n], ElemD)) continue;
		if (ElemD.eltyp == nTgtElemType) aTgtElemK[nCnt++] = aSelKey[n];
	}
	aTgtElemK.SetSize(nCnt);
	aSelKey.Copy(aTgtElemK);
	
	if(aSelKey.GetSize()<1)
	{
		AfxMessageBox(_LS(IDS_CMD_IEHP_NO_COND_ELEM));
		return;
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelKey, FALSE, TRUE, TRUE);
	
}

BOOL CInelasticHingeAssignDlg::ExternalInit(T_IEHG_K key)
{
	if (key == 0) return FALSE;

	T_IEHG_D data;
	data.Initialize();
	m_pDoc->m_pAttrCtrl->GetIehg(key, data);
	//m_cmbHingeProp.ChangeSelect(data.IehpKey);
	ChangeIehpComboSel(data.IehpKey);

	OnSelchangeInelasticHingePropCmb();

	return TRUE;
}

int CInelasticHingeAssignDlg::TransMembType(int& nType)
{
	switch(nType)
	{
	case 1: // Column
		nType = 1;
		break;
	case 2: // Beam
		nType = 0;
		break;
	case 3: // Brace
		nType = 2;
		break;
	case 4: // Wall
		nType = 3;
		break;
	default: ASSERT(0);
		nType = -1;
		break;
	}

	return nType;
}

void CInelasticHingeAssignDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}
void CInelasticHingeAssignDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_IEHP_ADD): case(UR_IEHP_DEL):
			//OnSelchangeInelasticHingePropCmb();
			UpdateIehpCombo();
			break;
		case(UR_IEHP_MFD):
			// 여기서는 저장만 하고 UR_IEHP_MFS에서 처리한다.
			bMFD = TRUE;
			break;
		case(UR_IEHP_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			UpdateIehpCombo();
			//OnSelchangeInelasticHingePropCmb();
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

void CInelasticHingeAssignDlg::OnImportHingeData()
{
	CInelasticHingeImportDlg dlg;
	dlg.DoModal();
}

HBRUSH CInelasticHingeAssignDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CCMChildBarBase::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_SECT_NAME_STC)
		return m_hEdtBrush;

	return hbr;
}
