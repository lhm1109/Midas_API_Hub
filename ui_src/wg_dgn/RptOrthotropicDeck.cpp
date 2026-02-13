
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptOrthotropicDeck.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"

//#include "Dgn_CSGRptManager_LSD12.h"
//#include "CRCExcelOutput.h"

//#include "DgnProgressDlg.h"
//#include "DgnPlateGirder.h"
//#include "CRCDataCtrl.h"

// #include "RptPlateGirder_EC4.h"
// #include "RptSteelGirder_LSD12.h"
// #include "RptSteelGirder_AASHTO12.h"
// #include "DgnPSCSelectPrintUintDlg.h"
#include "RptOrthotropicDeck_RUS.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptOrthotropicDeck::CRptOrthotropicDeck()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	//m_bStopExecute = FALSE;
	
	//m_bTestMode = CDBLib::CheckTheRegistryForDesignTest();
	//   T_CPGD_D CpgdD; CpgdD.Initialize();
	//   m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
	
	m_SoddD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnSodd(m_SoddD);  
	
	m_pDoc->SetCivilCodeDgn(m_SoddD.nDesignCode);
	//m_pDataCtrl = new CCRCDataCtrl();
	
}

CRptOrthotropicDeck::~CRptOrthotropicDeck()
{
//   if(m_pDataCtrl)
//   {
//     delete m_pDataCtrl;
//     m_pDataCtrl = NULL;
//   }
}



void CRptOrthotropicDeck::Start(CString sFileName, int iPrintOpt, BOOL bSaveImage)
{
	CCurUnitSaver Save(TRUE);

	int iRptSelUnit = 1;
	T_UNIT_INDEX CngUnit;

	if(m_SoddD.nDesignCode==SNiP_20503_84_SOD || m_SoddD.nDesignCode==SP_35_13330_11_SOD) 
	{
// 		CDgnPSCSelectPrintUnitDlg Dlg; Dlg.SetDesignType(1);
// 		int iRptSelUnit = 0;
// 		if(Dlg.DoModal() == IDOK)
// 		{
// 			iRptSelUnit  = Dlg.GetSelectUnit();
// 			CDBLib::SetReportSelUnit4PSC(iRptSelUnit);
// 		}
// 		else
// 		{
// 			return;
// 		}
		
		if(iRptSelUnit==0) // US unit // Default Unit-System. Kip / in
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		}
		else if(iRptSelUnit==1) // SI unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else ASSERT(0);

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		

		CRptOrthotropicDeck_RUS ODD_RUS;
		ODD_RUS.Execute_OrthotropicDeckRpt_RUS(m_SoddD.nDesignCode, sFileName, iPrintOpt, bSaveImage);
	}
	else
		ASSERT(0);
	
	
}


// BOOL CRptOrthotropicDeck::IsPrintPosiData()
// {
//   //if(!IsExistPrintCheckOfGlobalData()) return FALSE;  // 확인하기..
//   // 설계요소/출력요소별 정보를 비교해야 함. 0:None, 1:I, 2:J, 3:I&J
//   
//   int i=0;
//   int j=0;
//   CArray<T_POSC_K, T_POSC_K> arDesignElemKey; arDesignElemKey.RemoveAll();
//   m_pDoc->m_pAttrCtrl->GetPoscKeyList(arDesignElemKey);
//   CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
//   m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);
//   
//   // Check Array-Size!
//   int iDesign = arDesignElemKey.GetSize();
//   int iReport = arPrintElemKey.GetSize();
//   
//   if(iReport==0) return FALSE; // 출력정보가 없을 경우 return함.
//   
//   if(iDesign>0)
//   {
//     int iDesignElemKey = 0;
//     int iReportElemKey = 0;
//     
//     for(i=0; i<iReport; ++i)
//     {
//       iReportElemKey = arPrintElemKey.GetAt(i);
//       
//       // PrintOpt.을 통해서 None Type을 고려하지 않도록 조치!
//       T_POSX_D  PosxD;
//       if(!m_pDoc->m_pAttrCtrl->GetPosx(iReportElemKey, PosxD)) continue;
//       if(!CheckPosx_NoneTypeElem(PosxD))  continue; // 확인하기..
//       
//       for(j=0; j<iDesign; ++j)
//       {
//         iDesignElemKey  = arDesignElemKey.GetAt(j);
//         
//         if(iDesignElemKey==iReportElemKey)  return TRUE;  // 교집합이 하나라도 존재하면 Search를 멈추고 출력함.        
//       }
//     }
//   }
//   else 
//     return TRUE;    
//   
//   return FALSE;
// }




// BOOL CRptOrthotropicDeck::CheckPosx_NoneTypeElem(T_POSX_D PosxD)
// {
//   BOOL bOK = FALSE;
// 
//   int iDgnCode = m_CpgdD.iDgnCode;
// 
//   //
//   //하나의 아이템이라도 출력항목에 포함된다면 TRUE를 Return함.
//   =1901, KSCE_LSD15_CSG=1902, AASHTO_LRFD07_CSG=1903, AASHTO_LRFD12_CSG=
//   if(iDgnCode==EUROCODE4_2_04_CSG)
//   {
//     if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iBarChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iBarChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//   }
//   else if(iDgnCode==AASHTO_LRFD08 || iDgnCode==AASHTO_LRFD12 || iDgnCode==CSA_S6_10 ||
//           iDgnCode==JTG_D62_04|| iDgnCode==CJJ_11_2011 )
//   {
//     if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//   }
//   else if(iDgnCode==EUROCODE2_2_05_PSC || iDgnCode==KSCE_LSD15_PSC)
//   {    
//     if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     /*
//     if(PosxD.iMomChk[0]==0)         bOK = FALSE;
//     if(!bOK && PosxD.iMomChk[1]==0) bOK = FALSE;
//     if(!bOK && PosxD.iShrChk==0)    bOK = FALSE;
//     if(!bOK && PosxD.iTorChk==0)    bOK = FALSE;
//     */
//   }
//   else if (iDgnCode==SNiP_20503_84_PSC || iDgnCode==SP_35_13330_11_PSC)
//   {
//     if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
//     if(PosxD.nFatiChk   > 0)  return TRUE;
//     if(PosxD.nCrack     > 0)  return TRUE;
//   }
//   else
//     ASSERT(0);
// 
//   return bOK;
// }// 

CString CRptOrthotropicDeck::GetSaveFileDirEx()
{
	CString strFileName=m_pDoc->GetPathName();
	
	CFileCtrl FileCtrl(strFileName);
	
	return FileCtrl.GetFilePath();
}
// 
// 
CString CRptOrthotropicDeck::GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/)
{
	CString strFileName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);

 // SOD 계산서 base가 .xls 로 되어 있어 최신 엑셀 버전으로 출력하면 확장자명이 달라서 그런지 출력결과가 안보이고 있음.
 // 왜 글런지 모르겠음 ㅜㅜ
 // 그래서 무조건 .xls로 출력하기로 함.

	CString strSaveFileName;  
	//   if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC)
	//   {
	//     strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".")+CMSOfficeMgr::GetExtensionNameExcel();
	//   }
	//   else
	{
		//strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");
		strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".xlsx");
	}
	
	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);
	
	int count  = 0;
	while(saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);
		
		CString strSaveFileName;
		//     if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC)
		//     {
		//       strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();
		//     }
		//     else
		{
			strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xlsx");
		}
		
		delete saveFileCtrl;
		saveFileCtrl = new CFileCtrl(strSaveFileName);
	}
	
	if(bWithoutExtension) 
		strSaveFileName = saveFileCtrl->GetFileNameWithoutExtension();
	else
		strSaveFileName = saveFileCtrl->GetFilePathAndName();
	
	delete saveFileCtrl;
	
	return strSaveFileName;
}
// 
// CString CRptOrthotropicDeck::GetLoadCombType(int iMaxType)
// {
// 	
//   if (iMaxType > 12) return _T("-");
// 
//   LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
//                               _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
//                               _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};
//   
//   return aType[iMaxType];
// }
