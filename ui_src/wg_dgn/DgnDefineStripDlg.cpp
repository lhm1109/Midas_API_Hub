// DgnDefineStripDlg.cpp : implementation file
//

#include "stdafx.h"
// #include "wg_dgn.h"
// #include "DgnDefineStripDlg.h"
// 
// #include "..\wg_base\wg_base_DlgUtil.h"
// #include "..\wg_base\wg_base_ControlEx.h"
// #include "..\wg_base\CheckDialogOpen.h"
// #include "..\wg_base\NumericOptimizer.h"
// #include "..\wg_base\I_GENModelBase.h"
// #include "..\wg_base\TreeMenuBarBase.h"
// #include "..\wg_base\I_GENStateDefine.h"
// 
// #include "..\wg_db\wg_db_DBDoc.h"
// #include "..\wg_db\ViewBuff.h"
// #include "..\wg_db\wg_db_EditData.h"
// #include "..\wg_db\wg_db_AttrCtrl.h"
// #include "..\wg_db\wg_db_AttrCtrl2.h"
// #include "..\wg_db\wg_db_UndoCtrl.h"
// 
// 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
// 
// const int STATUS_ADD=0;
// const int STATUS_DEL=1;
// const int STATUS_MOD=2;
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnDefineStripDlg dialog
// CDgnDefineStripDlg::CDgnDefineStripDlg(CWnd* pParent )
//   : CDgnDlgBase(CDgnDefineStripDlg::IDD, pParent)
// {
//   m_pDoc = CDBDoc::GetDocPoint();
//   m_pIGM = CTreeMenuBarBase::GetI_GENModelST();
//   //{{AFX_DATA_INIT(CDgnDefineStripDlg)	
//   //}}AFX_DATA_INIT
// }
// 
// CDgnDefineStripDlg::~CDgnDefineStripDlg()
// {
// 
// }
// 
// void CDgnDefineStripDlg::DoDataExchange(CDataExchange* pDX)
// {
//   CDgnDlgBase::DoDataExchange(pDX);
//   //{{AFX_DATA_MAP(CDgnDefineStripDlg)
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_NAME_EDT, m_edtName);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_NODE_EDT, m_wndNodeList);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_I_WTHL_EDT, m_edtW1);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_I_WTHR_EDT, m_edtW2);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_J_WTHL_EDT, m_edtW3);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_J_WTHR_EDT, m_edtW4);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_I_WTHL_UNT, m_untW1);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_I_WTHR_UNT, m_untW2);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_J_WTHL_UNT, m_untW3);
//   DDX_Control(pDX, IDC_DGN_DEFINE_STRIP_J_WTHR_UNT, m_untW4);
//   //}}AFX_DATA_MAP
// }
// 
// BEGIN_MESSAGE_MAP(CDgnDefineStripDlg, CDgnDlgBase)
//   //{{AFX_MSG_MAP(CDgnDefineStripDlg)
//   ON_BN_CLICKED(IDOK, OnOK)
//   ON_BN_CLICKED(IDCANCEL, OnCancel)
//   ON_BN_CLICKED(IDC_BTN_APPLY, OnApply)
//   ON_BN_CLICKED(IDC_DGN_DEFINE_STRIP_ADD_BTN , OnAddStripBtn)
//   ON_BN_CLICKED(IDC_DGN_DEFINE_STRIP_DEL_BTN , OnDelStripBtn)
//   ON_BN_CLICKED(IDC_DGN_DEFINE_STRIP_REG_BEAM_BTN, OnRegEffectiveBeamBtn)
//   ON_EN_SETFOCUS(IDC_DGN_DEFINE_STRIP_NODE_EDT, OnSetfocusStripNodeList)
//   ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
//   ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnMouseEditEscape)
//   //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// BOOL CDgnDefineStripDlg::OnInitDialog() 
// {
//   CDgnDlgBase::OnInitDialog();
// 
// 
//   // jslee : 임시로 strip 부재력 확인해보자
//   CArray<T_LCOM_K, T_LCOM_K> aLcomK;
//   T_LCOM_D LcomD;
//   m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL, aLcomK);
//   CArray<T_MSTR_K, T_MSTR_K> aMstrK_tmp;
//   m_pDoc->m_pAttrCtrl2->GetMstrKeyList(aMstrK_tmp);
//   CArray<T_FORCSTRIP_D,T_FORCSTRIP_D> aForce;
//   int nMode = 0; // Nodal Average Mode
//   m_pDoc->m_pAttrCtrl2->GetStripForce(D_LOADCASE_STATIC, 1, nMode, aMstrK_tmp, aForce);
//   // jslee : 임시로 strip 부재력 확인해보자
// 
// 
//   InitUnit();
// 
//   m_wndNodeList.SetAttNodeList();
//   m_wndNodeList.SetLButtonDownNotifyWindow(this);
//   m_wndNodeList.SetEnterNotifyWindow(this);
//   m_wndNodeList.SetEscNotifyWindow(this);
//   m_wndNodeList.SetModeToUse(MOUSEEDIT_USE_GET_FLOAD_NODE_LIST);
//   m_wndNodeList.SetMaxNodeKeyNum(D_FBLA_MAXNOD);
// 
//   m_wndGrid.SubclassDlgItem(IDC_DGN_DEFINE_STRIP_LIST_GRID, this);
//   m_wndGrid.Initialize(this);
//   m_edtName.SetWindowText(_LS(IDS_DGN_STRIP));
// 
//   Data2Dlg();
// 
//   return TRUE;  // return TRUE unless you set the focus to a control
//   // EXCEPTION: OCX Property Pages should return FALSE
// }
// 
// void CDgnDefineStripDlg::InitUnit()
// {
//   m_edtW1.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_untW1.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_edtW2.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_untW2.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_edtW3.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_untW3.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_edtW4.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_untW4.SetUnitType(D_UNITSYS_BASE_LENGTH);
// }
// 
// void CDgnDefineStripDlg::OnOK()
// {
//   Dlg2Data();
//   CDgnDlgBase::OnOK();
//   DestroyWindow();
// }
// 
// void CDgnDefineStripDlg::OnCancel()
// {
//   CDgnDlgBase::OnCancel();
//   DestroyWindow();
// }
// 
// void CDgnDefineStripDlg::OnApply()
// {
//   Dlg2Data();
//   m_wndGrid.ReInitialize(); // 그리드 갱신
// }
// 
// void CDgnDefineStripDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
// {
//   HWND hWnd;
//   if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
// 
//   switch(lHint)
//   {
//   case D_UPDATE_UNIT:
//     m_wndGrid.UnitChanged(m_pDoc);
//     break;
//   case D_UPDATE_SEL_ADD:    case D_UPDATE_SEL_DEL:
// 
//     break;
//   case D_UPDATE_BUFFER_AFTER:
//     UpdateBuffer();
//     break;
//   default:
//     break;
//   }
// }
// 
// void CDgnDefineStripDlg::UpdateBuffer()
// {
// 
// }
// 
// void CDgnDefineStripDlg::Data2Dlg()
// {
// 
// }
// 
// void CDgnDefineStripDlg::Dlg2Data()
// {
//   CArray<T_MSTR_K, T_MSTR_K> aMstrK;
//   CArray<T_MSTR_D, T_MSTR_D> aMstrD;
//   m_wndGrid.GetMstrKeyDataFromGrid(aMstrK, aMstrD);
// 
//   CString strCmd = _LS(IDS_DGN_DATACTRL_ADD_MSTR);
//   if(!m_pDoc->m_pUndoCtrl->StartEditDB(strCmd, CMDTYPE_REMOVE_DESIGN)) return;
// 
//   // DB에는 있는데 그리드에는 없는 Key -> 삭제하자
//   CArray<T_MSTR_K, T_MSTR_K> aMstrK_DB;
//   m_pDoc->m_pAttrCtrl2->GetMstrKeyList(aMstrK_DB);
//   int i,j;
//   for(i=0; i<aMstrK_DB.GetSize(); i++)
//   {
//     BOOL bAlive = FALSE;
//     for(j=0; j<aMstrK.GetSize(); j++)
//     {
//       if(aMstrK_DB[i]==aMstrK[j])
//       {
//         bAlive = TRUE;
//         continue;
//       }
//     }
//     if(!bAlive) m_pDoc->m_pEditData->DelMstr(aMstrK_DB[i]);
//   }
//   
//   // 추가/삭제
//   for(i=0; i<aMstrK.GetSize(); i++)
//   {
//     // Add : 새로 추가된 Key 
//     if(aMstrK[i]==0)
//     {
//       m_pDoc->m_pEditData->AddMstr(m_pDoc->m_pAttrCtrl2->GetStartNumMstr(), aMstrD[i]);
//     }
//     // Modify : 기존에 존재하던 데이터
//     else
//     {
//       m_pDoc->m_pEditData->ModifyMstr(aMstrK[i], aMstrD[i]);
//     }
//   }
// 
//   m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_MSTR_CMD);
// }
// 
// void CDgnDefineStripDlg::OnSetfocusStripNodeList()
// {
//   m_pIGM->EndStripSelect();
//   m_pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
//   m_pIGM->SetSelectNodeOnVerElemOnly(TRUE);
// }
// 
// LRESULT CDgnDefineStripDlg::OnMouseEditEscape(WPARAM wParam, LPARAM lParam)
// {
//   m_pIGM->EndStripSelect();
//   m_pIGM->SetModelingModeST(IG_STATE_INITALL);
//   m_wndNodeList.ClearNodeKeyList();
// 
//   return 0L;
// }
// 
// LRESULT CDgnDefineStripDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
// {
//   return 0L;
// }
// 
// void CDgnDefineStripDlg::PostNcDestroy()
// {
//   // 노드선택모드 끄기
//   m_pIGM->EndStripSelect();
//   m_pIGM->SetModelingModeST(IG_STATE_INITALL);
// 
//   CDgnDlgBase::PostNcDestroy();
// }
// 
// void CDgnDefineStripDlg::OnAddStripBtn()
// {
//   // 노드선택모드 끄기
//   m_pIGM->EndStripSelect();
//   m_pIGM->SetModelingModeST(IG_STATE_INITALL);
// 
//   // 노드리스트 들고오자
//   CArray<UINT, UINT> aNodeList;
//   CString csNodeList;
//   m_wndNodeList.GetWindowText(csNodeList);
//   if(!GetNodeList(csNodeList, aNodeList))
//   {
//     GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
//     return;
//   }
//   if(!CheckStripNode(aNodeList)) return;
//   
//   // 로컬데이터에 입력
//   T_MSTR_D MstrD;
//   MstrD.Initialize();
//   m_edtName.GetWindowText(MstrD.strName);
//   MstrD.adWidth[0] = m_edtW1.GetEditValue();
//   MstrD.adWidth[1] = m_edtW2.GetEditValue();
//   MstrD.adWidth[2] = m_edtW3.GetEditValue();
//   MstrD.adWidth[3] = m_edtW4.GetEditValue();
// 
//   int i;
//   for(i=0; i<aNodeList.GetSize()-1; i++)
//   {
//     MstrD.anNodeK[0] = aNodeList[i];
//     MstrD.anNodeK[1] = aNodeList[i+1];
//     m_wndGrid.AddFromDlg(0, MstrD);
//   }
// }
// 
// void CDgnDefineStripDlg::OnDelStripBtn()
// {
//   // 노드선택모드 끄기
//   m_pIGM->EndStripSelect();
//   m_pIGM->SetModelingModeST(IG_STATE_INITALL);
// 
//   m_wndGrid.DelFromDlg();
// }
// 
// // 유효보로 생성 정보를 스트립으로 자동 등록(버튼 클릭 시)
// // 등록 후 수정하고 다시 불러오면 수정한 정보 삭제 됨
// // Name은 유효보 Section 이름과 동일한 것으로 함
// void CDgnDefineStripDlg::OnRegEffectiveBeamBtn()
// {
//   CMap<T_SECT_K,T_SECT_K,T_MSTR_D,T_MSTR_D> mMstrPrototype; // T_MSTR_D 멤버변수 중 anNodeK는 빼고 채워진 데이터 맵
//   mMstrPrototype.InitHashTable(HASHSIZESECT);
//   CArray<T_SECT_K,T_SECT_K> aSectK;
//   m_pDoc->m_pAttrCtrl2->GetSectForEBeamKeyList(aSectK);
//   T_SECT_K SectK;
//   T_SECT_D SectD;
//   T_MSTR_D MstrPrototype;
//   double dDist[4];
//   for(int i=0; i<aSectK.GetSize(); i++)
//   {    
//     if(!m_pDoc->m_pAttrCtrl2->GetWidthSectForEBeam(aSectK[i], dDist)) { ASSERT(0); continue; }
//     if(!m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], SectD)) { ASSERT(0); continue; }
// 
//     MstrPrototype.Initialize();
//     MstrPrototype.strName = SectD.SName;
//     for(int j=0; j<4; j++) MstrPrototype.adWidth[j] = dDist[j];
//     mMstrPrototype.SetAt(aSectK[i], MstrPrototype);
//   }
// 
//   // 그리드의 데이터를 들고와서
//   CArray<T_MSTR_K, T_MSTR_K> aMstrK;
//   CArray<T_MSTR_D, T_MSTR_D> aMstrD;
//   m_wndGrid.GetMstrKeyDataFromGrid(aMstrK, aMstrD);
// 
//   // 갱신할 데이터와 이름이 같으면 다 지운다.
//   POSITION pos = mMstrPrototype.GetStartPosition();
//   while(pos)
//   {
//     mMstrPrototype.GetNextAssoc(pos, SectK, MstrPrototype);
//     for(i=aMstrK.GetSize()-1; i>=0; --i)
//     {
//       if(aMstrD[i].strName == MstrPrototype.strName)
//       {
//         aMstrK.RemoveAt(i);
//         aMstrD.RemoveAt(i);
//       }
//     }
//   }
// 
//   // 전체 요소를 돌며 스트립 데이터를 만들어서 배열에 넣는다.
//   CArray<T_ELEM_K,T_ELEM_K> aElemK;
//   m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
//   T_ELEM_D ElemD;
//   T_MSTR_D MstrD;
//   for(int i=0; i<aElemK.GetSize(); i++)
//   {
//     if(!m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD)) { ASSERT(0); continue; }
//     if(ElemD.eltyp!=BEAM_EL || ElemD.nSubType!=1) continue; // 유효보인가?
//     
//     if(!mMstrPrototype.Lookup(ElemD.elpro, MstrD)) { ASSERT(0); continue; }
//     MstrD.anNodeK[0] = ElemD.elnod[0];
//     MstrD.anNodeK[1] = ElemD.elnod[1];
//     aMstrK.Add(0);
//     aMstrD.Add(MstrD);
//   }
// 
//   // 열심히 갱신한 데이터를 그리드로 업데이트!
//   m_wndGrid.ResetGridWithData(aMstrK, aMstrD);
// }
// 
// BOOL CDgnDefineStripDlg::CheckStripNode(CArray<UINT, UINT>&NList)
// {
//   // 2개 이상의 절점 입력 필요
//   if(NList.GetSize() < 2)
//   {
//     GSaveHistoryFormatNF(_LS(IDS_DGN_NEED_MORE_THAN_2_NODE));
//     return FALSE;
//   }
//   // 수직부재(기둥,벽) 위의 절점인가?
//   for(int i=0; i<NList.GetSize(); i++)
//   {
//     if(!m_pDoc->m_pAttrCtrl2->IsVerElemConnectedNode(NList[i], TRUE)) return FALSE;
//   }
// 
//   return TRUE;
// }