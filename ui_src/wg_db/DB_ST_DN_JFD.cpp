#include "stdafx.h"
#include "DB_ST_DN_JFD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// void T_HFOP_Detail::Initialize()
// {
//   StldK      = 0;
//   strGruName = _T("Default");
//   bTopHorizF = FALSE;
//   dTopHorizF = 0.5; //최상층의 수평력
//   bFndWeight = FALSE;
//   dFndWeight = 0.5; //기초 부분의 중량
//   bFndPlatWt = FALSE;
//   dFndPlatWt = 0.5; //기초 판의 중량
//   bAddedLoad = FALSE;
//   dAddedLoad = 0.5; //추가 하중
// }
// 
// T_HFOP_Detail::T_HFOP_Detail() { Initialize(); };
// 
// // T_HFOP_Detail& T_HFOP_Detail::operator=(const T_HFOP_Detail& src)
// // {
// //   
// //   return *this;
// // }

//////////////////////////////////////////////////////////////////////////
// void T_HFOP_D::Initialize()
// {
//   arHflopLcD.RemoveAll();
//   arHfopGrpD.RemoveAll();
//   arHfopDetail.RemoveAll();
// }
// 
// T_HFOP_D::T_HFOP_D() { Initialize(); };
// 
// T_HFOP_D::T_HFOP_D(T_HFOP_D& src) { *this = src; }
// 
// T_HFOP_D& T_HFOP_D::operator=(const T_HFOP_D& src)
// {
//   arHflopLcD.Copy(src.arHflopLcD);
//   arHfopGrpD.Copy(src.arHfopGrpD);
//   arHfopDetail.Copy(src.arHfopDetail);
//   
//   return *this;
// }
// 
// BOOL T_HFOP_D::IsHfopStld(const T_STLD_K stldK, int* nIndex/* = NULL*/)
// {
//   if(nIndex != NULL)
//   {
//     *nIndex = -1;
//   }
//   
//   BOOL bExist = FALSE;
//   int nHfopLcdSize = arHflopLcD.GetSize();
//   for(int i = 0; (i < nHfopLcdSize) && (bExist == FALSE) ; i++)
//   {
//     if(arHflopLcD.GetAt(i).StldK == stldK)
//     {
//       bExist = TRUE;
//     }
//   }
//   if(bExist && (nIndex!=NULL)) { *nIndex = i; }
//   return bExist;
// }
// 
// BOOL T_HFOP_D::GetHfopDeatilData(const T_STLD_K StldK, const CString strGrpName, T_HFOP_Detail* const pHfopDetail)
// {
//   BOOL bRet = FALSE;
//   int nDetailSize = arHfopDetail.GetSize();
// 
//   T_HFOP_Detail HfopDetail; HfopDetail.Initialize();
//   for( int i = 0; (i < nDetailSize) && (bRet == FALSE) ; i ++ )
//   {
//     HfopDetail = arHfopDetail.GetAt(i);
//     if( (HfopDetail.StldK == StldK) && (HfopDetail.strGruName == strGrpName) )
//     {
//       bRet = TRUE;
//     }
//   }
// 
//   if(bRet == TRUE)
//   {
//     *pHfopDetail = HfopDetail;
//   }
//   return bRet;
//}