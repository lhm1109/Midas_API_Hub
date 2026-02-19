#pragma once

#include "RCDataCtrl_CH.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_RC_Struct.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_PSC_Struct.h"
#include "DgnJTG_D62_04.h"
#include "..\wg_db\TendonGrup.h"

class WA_Convert
{
public:
	static _DGN_RC_MATL_CH_W Convert_A2W(const _DGN_RC_MATL_CH &AData)
	{
		_DGN_RC_MATL_CH_W DestD;
		DestD.dfcd =AData.dfcd ;
		DestD.dftd =AData.dftd ;
		DestD.dfcuk=AData.dfcuk;
		DestD.dEc  =AData.dEc  ;
		DestD.dfck1=AData.dfck1;
		DestD.dftk1=AData.dftk1;
		DestD.dfsd =AData.dfsd ;
		DestD.dfsd1=AData.dfsd1;
		DestD.dEs  =AData.dEs  ;
		DestD.dfsk =AData.dfsk ;
		DestD.dfsyd=AData.dfsyd;
		DestD.dEsy =AData.dEsy ;
		DestD.dfsky=AData.dfsky;
		DestD.dsib =AData.dsib ;
		DestD.dBeta=AData.dBeta;
		DestD.dEpsilon=AData.dEpsilon;//┖see [JTG D62--2004] 5.3.3 P34    
		return DestD;
	}
	static T_RPSC_MBAR_W Convert_A2W(const T_RPSC_MBAR &SrcD)
	{
		T_RPSC_MBAR_W DestD;
		DestD.strBarNa    = CT2W(SrcD.strBarNa);
		DestD.iBarNum     = SrcD.iBarNum;
		DestD.iStaYRef    = SrcD.iStaYRef;
		DestD.dStaY       = SrcD.dStaY;
		DestD.iStaZRef    = SrcD.iStaZRef;
		DestD.dStaZ       = SrcD.dStaZ;
		DestD.dPitch      = SrcD.dPitch;
		DestD.nPart       = SrcD.nPart;
		return DestD;
	}
	static _DGN_RC_COLUMN_SECTION_CH_W Convert_A2W(const _DGN_RC_COLUMN_SECTION_CH &SrcD)
	{
		_DGN_RC_COLUMN_SECTION_CH_W WData;
		WData.dbf1       = SrcD.dbf1   ;
		WData.dhf1       = SrcD.dhf1   ;
		WData.dbf2       = SrcD.dbf2   ;
		WData.dhf2       = SrcD.dhf2   ;
		WData.db         = SrcD.db     ;
		WData.dh         = SrcD.dh     ;
		WData.dr         = SrcD.dr     ;
		WData.dd         = SrcD.dd     ;
		WData.dd1        = SrcD.dd1    ;
		WData.dx0        = SrcD.dx0    ;
		WData.drs        = SrcD.drs    ;
		WData.dC         = SrcD.dC     ;
		WData.dh0        = SrcD.dh0    ;
		WData.das1       = SrcD.das1   ;
		WData.das        = SrcD.das    ;
		WData.dA         = SrcD.dA     ;
		WData.dAs1       = SrcD.dAs1   ;
		WData.dAs        = SrcD.dAs    ;
		WData.dAsl       = SrcD.dAsl   ;
		WData.dAsr       = SrcD.dAsr   ;
		WData.dAsh       = SrcD.dAsh   ;
		WData.dPith      = SrcD.dPith  ;
		WData.dAnglh     = SrcD.dAnglh ;
		WData.dAsy       = SrcD.dAsy   ;
		WData.dAsz       = SrcD.dAsz   ;
		WData.dIxx       = SrcD.dIxx   ;
		WData.dIyy       = SrcD.dIyy   ;
		WData.dIzz       = SrcD.dIzz   ;
		WData.dCyp       = SrcD.dCyp   ;
		WData.dCym       = SrcD.dCym   ;
		WData.dCzp       = SrcD.dCzp   ;
		WData.dCzm       = SrcD.dCzm   ;
		WData.dQyb       = SrcD.dQyb   ;
		WData.dQzb       = SrcD.dQzb   ;
		WData.dl0        = SrcD.dl0    ;
		WData.dPhi       = SrcD.dPhi   ;
		return WData;
	}

	static void Convert_A2W(const CArray<T_RPSC_MBAR,T_RPSC_MBAR> &SrcD, CArray<T_RPSC_MBAR_W,T_RPSC_MBAR_W&> &DestD)
	{
		DestD.RemoveAll();
		for (int i=0; i<SrcD.GetCount(); i++)
		{
			DestD.Add(Convert_A2W(SrcD[i]));
		}
	}

	static _DGN_RC_SECTION_CH_W Convert_A2W(const _DGN_RC_SECTION_CH &AData)
	{
		_DGN_RC_SECTION_CH_W WData;

		WData.dbf1   = AData.dbf1  ;
		WData.dhf1   = AData.dhf1  ;
		WData.dbf2   = AData.dbf2  ;
		WData.dhf2   = AData.dhf2  ;
		WData.db     = AData.db    ;
		WData.dh     = AData.dh    ;	  
		WData.dx     = AData.dx    ;
		WData.dh0    = AData.dh0   ;
		WData.das1   = AData.das1  ;
		WData.das    = AData.das   ;
		WData.dA     = AData.dA    ;
		WData.dAs    = AData.dAs   ;
		WData.dAs1   = AData.dAs1  ;
		WData.dAst   = AData.dAst  ;
		WData.dAsv1  = AData.dAsv1 ;
		WData.dPitt  = AData.dPitt ;
		WData.dAnglt = AData.dAnglt;
		WData.dat    = AData.dat   ; 
		WData.dAsb   = AData.dAsb  ; 
		WData.dPitb  = AData.dPitb ;
		WData.dAnglb = AData.dAnglb;
		WData.dAsh   = AData.dAsh  ;
		WData.dPith  = AData.dPith ;
		WData.dAnglh = AData.dAnglh;
		WData.dWt    = AData.dWt   ;
		WData.dBetaa = AData.dBetaa;
		WData.dAoh   = AData.dAoh  ;
		WData.dPh    = AData.dPh   ;
		WData.dd     = AData.dd    ;
		WData.dd1    = AData.dd1   ;
		WData.dAsy   = AData.dAsy  ;
		WData.dAsz   = AData.dAsz  ;
		WData.dIxx   = AData.dIxx  ;
		WData.dIyy   = AData.dIyy  ;
		WData.dIzz   = AData.dIzz  ;
		WData.dCyp   = AData.dCyp  ;
		WData.dCym   = AData.dCym  ;
		WData.dCzp   = AData.dCzp  ;
		WData.dCzm   = AData.dCzm  ;
		WData.dQyb   = AData.dQyb  ;
		WData.dQzb   = AData.dQzb  ;
		Convert_A2W(AData.rbar, WData.rbar);
		return WData;
	}


	static T_SPSC_SECT_W Convert_A2W(const T_SPSC_SECT &AData)
	{
		T_SPSC_SECT_W WData;

		WData.dyDim         = AData.dyDim       ; 
		WData.dzDim         = AData.dzDim       ; 
		WData.dyCen         = AData.dyCen       ; 
		WData.dzCen         = AData.dzCen       ; 
		WData.dArea         = AData.dArea       ; 
		WData.dAsy          = AData.dAsy        ; 
		WData.dAsz          = AData.dAsz        ; 
		WData.dIxx          = AData.dIxx        ; 
		WData.dIyy          = AData.dIyy        ; 
		WData.dIzz          = AData.dIzz        ; 
		WData.dSlt          = AData.dSlt        ; 
		WData.dSlb          = AData.dSlb        ; 
		WData.dSrt          = AData.dSrt        ; 
		WData.dSrb          = AData.dSrb        ; 
		WData.dSlt_s        = AData.dSlt_s      ; 
		WData.dSlb_s        = AData.dSlb_s      ; 
		WData.dSrt_s        = AData.dSrt_s      ; 
		WData.dSrb_s        = AData.dSrb_s      ; 
		WData.dYlt          = AData.dYlt        ; 
		WData.dYlb          = AData.dYlb        ; 
		WData.dYrt          = AData.dYrt        ; 
		WData.dYrb          = AData.dYrb        ; 
		WData.dYlt_s        = AData.dYlt_s      ; 
		WData.dYlb_s        = AData.dYlb_s      ; 
		WData.dYrt_s        = AData.dYrt_s      ; 
		WData.dYrb_s        = AData.dYrb_s      ; 
		WData.dGirderWidth  = AData.dGirderWidth; 

		return WData;
	}

	static _DGN_RBAR_CRC_W Convert_A2W(const _DGN_RBAR_CRC &SpscSect)
	{
		_DGN_RBAR_CRC_W WData;
		WData.dArea = SpscSect.dArea;
		WData.dDia  = SpscSect.dDia;
		WData.dy    = SpscSect.dy;
		WData.dz    = SpscSect.dz;
		WData.nPart = SpscSect.nPart;
		WData.strRbarNa = CT2W(SpscSect.strRbarNa);
		return WData;
	}

	 

	static void Convert_A2W(const CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> &SrcD, CArray<_DGN_RBAR_CRC_W,_DGN_RBAR_CRC_W&> &DestD)
	{
		DestD.RemoveAll();
		for (int i=0; i<SrcD.GetCount(); i++)
		{
			DestD.Add(Convert_A2W(SrcD[i]));
		}
	}


	static _DGN_TNDN_CRC_W Convert_A2W(const _DGN_TNDN_CRC &SpscSect)
	{
		_DGN_TNDN_CRC_W WData;
		WData.MatlK=SpscSect.MatlK;
		WData.strMatlName   = CT2W(SpscSect.strMatlName);
		WData.strTendonName = CT2W(SpscSect.strTendonName);
		WData.strTndnGrup   = CT2W(SpscSect.strTndnGrup);
		WData.KeyTdna   =SpscSect.KeyTdna  ;
		WData.dTndnNum  =SpscSect.dTndnNum ;
		WData.iTndnType =SpscSect.iTndnType;
		WData.iBondType =SpscSect.iBondType;
		WData.iLoadType =SpscSect.iLoadType;
		WData.iRelxType =SpscSect.iRelxType;
		WData.dEps      =SpscSect.dEps     ; 
		WData.dfy       =SpscSect.dfy      ;
		WData.dfpu      =SpscSect.dfpu     ; 
		WData.dfpy      =SpscSect.dfpy     ;
		WData.dArea     =SpscSect.dArea    ;
		WData.dy        =SpscSect.dy       ; 
		WData.dz        =SpscSect.dz       ;
		WData.dfpk      =SpscSect.dfpk     ; 
		WData.dfpdt     =SpscSect.dfpdt    ; 
		WData.dfpdc     =SpscSect.dfpdc    ;
		WData.dEffDia   =SpscSect.dEffDia  ;
		WData.dStressLL =SpscSect.dStressLL;
		for(int i=0; i<3; i++)
		{
			WData.dStress[i]=SpscSect.dStress[i];
			for(int j=0; j<3; j++)	
				WData.dStrLoss[i][j]=SpscSect.dStrLoss[i][j];
		}
		WData.dFp01         = SpscSect.dFp01       ;
		WData.dfp01k        = SpscSect.dfp01k      ;
		WData.dfpd_PT       = SpscSect.dfpd_PT     ;
		WData.dfpd_A        = SpscSect.dfpd_A      ;
		WData.dEpsi_pd      = SpscSect.dEpsi_pd    ;
		WData.dLength       = SpscSect.dLength     ;
		WData.iBoundaryNum  = SpscSect.iBoundaryNum;
		return WData;
	}


	static void Convert_A2W(const CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> &SrcD, CArray<_DGN_TNDN_CRC_W,_DGN_TNDN_CRC_W&> &DestD)
	{
		DestD.RemoveAll();
		for (int i=0; i<SrcD.GetCount(); i++)
		{
			DestD.Add(Convert_A2W(SrcD[i]));
		}
	}

 
	static T_MATL_ANALYSIS_W Convert_A2W(const T_MATL_ANALYSIS &AData)
	{
		T_MATL_ANALYSIS_W WData;
		WData.Elast               = AData.Elast         ;                
		WData.Poisson             = AData.Poisson       ;                
		WData.Thermal             = AData.Thermal       ;                
		WData.Density             = AData.Density       ;                
		WData.MassDensity         = AData.MassDensity   ;                
		WData.Elast_y             = AData.Elast_y       ;                
		WData.Poisson_y           = AData.Poisson_y     ;                
		WData.Thermal_y           = AData.Thermal_y     ;                
		WData.Elast_z             = AData.Elast_z       ;                
		WData.Poisson_z           = AData.Poisson_z     ;                
		WData.Thermal_z           = AData.Thermal_z     ;                
		WData.ShearModulus        = AData.ShearModulus  ;                
		WData.ShearModulus_y      = AData.ShearModulus_y;                
		WData.ShearModulus_z      = AData.ShearModulus_z;                
		WData.nIsoOrtho           = AData.nIsoOrtho     ;                
		WData.bUserElast          = AData.bUserElast    ;              
		return WData;
	}
	static T_MATL_DESIGN_KSCE05_W Convert_A2W(const T_MATL_DESIGN_KSCE05 &AData)
	{
		T_MATL_DESIGN_KSCE05_W WData;
		WData.dAlwS1    = AData.dAlwS1       ; 
		WData.dAlwS2    = AData.dAlwS2       ; 
		WData.dAlwS3    = AData.dAlwS3       ; 
		WData.dAlwS4    = AData.dAlwS4       ; 
		WData.dAlwS5    = AData.dAlwS5       ; 
		WData.dAlwS6    = AData.dAlwS6       ; 
		WData.dAlwS7    = AData.dAlwS7       ; 
		WData.dAlwS8    = AData.dAlwS8       ; 
		WData.dAlwS9    = AData.dAlwS9       ; 
		WData.dAlwS10   = AData.dAlwS10      ;
		WData.dMin_t1   = AData.dMin_t1      ;
		WData.dMin_t2   = AData.dMin_t2      ;
		WData.dMin_t3   = AData.dMin_t3      ;
		return WData;
	}
	static T_MATL_REBAR_W Convert_A2W(const T_MATL_REBAR &AData)
	{
		T_MATL_REBAR_W WData;
		WData.B_fy     = AData.B_fy;  
		WData.B_Elast  = AData.B_Elast;
		WData.B_Density= AData.B_Density;
		return WData;
	}
	static CRC_T_MATD_D_W ConvertMatd_A2W(const CRC_T_MATD_D &AData)
	{
		CRC_T_MATD_D_W WData;
		WData.MbarMatl.dRebar_AFu    =  AData.MbarMatl.dRebar_AFu         ;
		WData.MbarMatl.dRebar_AFt_D  =  AData.MbarMatl.dRebar_AFt_D       ;
		WData.MbarMatl.dRebar_AFt_T  =  AData.MbarMatl.dRebar_AFt_T       ;
		WData.MbarMatl.dRebar_AFt_EQ =  AData.MbarMatl.dRebar_AFt_EQ      ;
		WData.MbarMatl.dRebar_AFv    =  AData.MbarMatl.dRebar_AFv         ;
		WData.MbarMatl.dRebar_Es     =  AData.MbarMatl.dRebar_Es          ;
		//
		WData.SbarMatl.dRebar_AFu    =  AData.SbarMatl.dRebar_AFu         ;
		WData.SbarMatl.dRebar_AFt_D  =  AData.SbarMatl.dRebar_AFt_D       ;
		WData.SbarMatl.dRebar_AFt_T  =  AData.SbarMatl.dRebar_AFt_T       ;
		WData.SbarMatl.dRebar_AFt_EQ =  AData.SbarMatl.dRebar_AFt_EQ      ;
		WData.SbarMatl.dRebar_AFv    =  AData.SbarMatl.dRebar_AFv         ;
		WData.SbarMatl.dRebar_Es     =  AData.SbarMatl.dRebar_Es          ;
		return WData;
	}
	static T_RPSC_SBAR_W Convert_A2W(const T_RPSC_SBAR &AData)
	{
		T_RPSC_SBAR_W WData;
		WData.bSbarExist  =  AData.bSbarExist    ;    // 경사인장철근 정보 있으면 TRUE
		WData.dSbarPitch  =  AData.dSbarPitch    ;    // 경사인장철근의 간격
		WData.dSbarTheta  =  AData.dSbarTheta    ;    // 경사인장철근의 각도
		WData.dSbarAw     =  AData.dSbarAw       ;       // 경사인장철근의 배치량
		WData.bTndnExist  =  AData.bTndnExist    ;    // 전단강봉 정보 있으면 TRUE
		WData.dTndnDist   =  AData.dTndnDist     ;     // 전단강봉의 간격
		WData.dTndnTheta  =  AData.dTndnTheta    ;    // 전단강봉의 각도
		WData.dTndnAp     =  AData.dTndnAp       ;       // 전단강봉의 배치량
		WData.dTndnPs     =  AData.dTndnPs       ;       // 전단강봉의 유효인장력
		WData.bLbarExist  =  AData.bLbarExist    ;    // 횡방향철근 정보 있으면 TRUE
		WData.dLbarPitch  =  AData.dLbarPitch    ;    // 횡방향철근의 간격
		WData.dLbarAwt    =  AData.dLbarAwt      ;      // 횡방향철근의 Pitch당 철근 1개의 단면적
		WData.dLbarAlt    =  AData.dLbarAlt      ;      // 횡뱡향철근의 전단면적
		WData.bStirExist  =  AData.bStirExist    ;    // 6.6.0 추가: 전단철근 정보 있으면 TRUE
		WData.dStirPitch  =  AData.dStirPitch    ;    // 6.6.0 추가: 전단철근의 간격
		WData.dStirAw     =  AData.dStirAw       ;       // 6.6.0 추가: 전단철근의 배치량
		WData.dTndnFactor =  AData.dTndnFactor   ;   // 6.6.0 추가: 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3) 
		WData.bLbarFlg    =  AData.bLbarFlg      ;      // 6.9.2 추가: Acor계산시 플랜지 부분을 스트럽으로 감는것을 고려하는 변수
		WData.dLbarThick  =  AData.dLbarThick    ;  // 6.9.2 추가: 비틀림 스트럽에서 연단까지의 거리(피복두께)
		WData.bIncFC      =  AData.bIncFC        ;        // 6.9.2 추가: Include Flange/Cantilever
		return WData;
	}
	static T_MATL_DESIGN_W Convert_A2W(const T_MATL_DESIGN &AData)
	{
		T_MATL_DESIGN_W WData;
		WData.S_Fu          =AData.S_Fu        ; 
		WData.S_Fy1         =AData.S_Fy1       ;
		WData.S_Fy2         =AData.S_Fy2       ;
		WData.S_Fy3         =AData.S_Fy3       ;
		WData.S_Fy4         =AData.S_Fy4       ;
		WData.S_Fy5         =AData.S_Fy5       ; // add by Seungjun (`06.05.29) at 710
		WData.S_Fy6         =AData.S_Fy6       ; // add by Seungjun (`06.05.29) at 710
		WData.S_Aft         =AData.S_Aft       ;
		WData.S_Fy          =AData.S_Fy        ; 
		WData.S_Afv         =AData.S_Afv       ;
		WData.C_fc          =AData.C_fc        ; 
		WData.C_fci         =AData.C_fci       ;
		WData.S_Aft2        =AData.S_Aft2      ;
		WData.S_Aft3        =AData.S_Aft3      ;
		WData.S_Afv2        =AData.S_Afv2      ;
		WData.S_Afv3        =AData.S_Afv3      ;
		WData.bLambda       =AData.bLambda     ;
		WData.dLambda       =AData.dLambda     ;
		WData.strMatlName   =CT2W(AData.strMatlName) ;
		WData.dGamma_m      =AData.dGamma_m    ;
		WData.Matl_KSCE05   =Convert_A2W(AData.Matl_KSCE05);
		return WData;
	}
	static T_MATD_ALL_W Convert_A2W(const T_MATD_ALL &SpscSect)
	{
		T_MATD_ALL_W WData;

		WData.CodeName                =CT2W(SpscSect.CodeName         );
		WData.CodeMatlName            =CT2W(SpscSect.CodeMatlName     );
		WData.SubCodeName             =CT2W(SpscSect.SubCodeName      );
		WData.Analysis                =Convert_A2W(SpscSect.Analysis         );
		WData.AnalysisTopFlange       =Convert_A2W(SpscSect.AnalysisTopFlange);
		WData.AnalysisBotFlange       =Convert_A2W(SpscSect.AnalysisBotFlange);
		WData.AnalysisWeb             =Convert_A2W(SpscSect.AnalysisWeb      );
		WData.Design                  =Convert_A2W(SpscSect.Design           );
		WData.DesignTopFlange         =Convert_A2W(SpscSect.DesignTopFlange  );
		WData.DesignBotFlange         =Convert_A2W(SpscSect.DesignBotFlange  );
		WData.DesignWeb               =Convert_A2W(SpscSect.DesignWeb        );
		WData.bHybridFactor           =SpscSect.bHybridFactor    ;
		return WData;
	}
	
	static T_MATD_D_W Convert_A2W(const T_MATD_D &SpscSect)
	{
		T_MATD_D_W WData;
	 WData.Type                   = CT2W(SpscSect.Type)           ;
	 WData.Name                   = CT2W(SpscSect.Name)           ;
	 WData.Data1                  = Convert_A2W(SpscSect.Data1)                ;
	 WData.Data2                  = Convert_A2W(SpscSect.Data2)                ;
	 WData.Rebar_CodeName         = CT2W(SpscSect.Rebar_CodeName       );
	 WData.MainRebar_RebarName    = CT2W(SpscSect.MainRebar_RebarName  );
	 WData.SubRebar_RebarName     = CT2W(SpscSect.SubRebar_RebarName   );
	 WData.MainRebarData          = Convert_A2W(SpscSect.MainRebarData );
	 WData.SubRebarData           = Convert_A2W(SpscSect.SubRebarData  );
	 WData.bChangeSteel           = SpscSect.bChangeSteel         ;
	 WData.CRC                    = ConvertMatd_A2W(SpscSect.CRC) ;
	 WData.bServCheck             = SpscSect.bServCheck           ;
	 WData.dShortTerm             = SpscSect.dShortTerm           ;
	 WData.dLongTerm              = SpscSect.dLongTerm            ;
	 WData.Rebar_CodeName2        = CT2W(SpscSect.Rebar_CodeName2      );
	 WData.MainRebar_RebarName2   = CT2W(SpscSect.MainRebar_RebarName2 );
	 WData.SubRebar_RebarName2    = CT2W(SpscSect.SubRebar_RebarName2  );
	 WData.MainRebarData2         = Convert_A2W(SpscSect.MainRebarData2)       ;
	 WData.SubRebarData2          = Convert_A2W(SpscSect.SubRebarData2)       ;
		return WData;
	}
	static _PSC_MATL_ELEM_W Convert_A2W(const _PSC_MATL_ELEM &SpscSect)
	{
		_PSC_MATL_ELEM_W WData;
		WData.MatdD = Convert_A2W(SpscSect.MatdD);
		WData.dfci      =SpscSect.dfci    ;
		WData.dfci_s    =SpscSect.dfci_s  ;
		WData.dfck      =SpscSect.dfck    ; 
		WData.dftk      =SpscSect.dftk    ;
		WData.dfckp     =SpscSect.dfckp   ; 
		WData.dftkp     =SpscSect.dftkp   ;
		WData.dfcd      =SpscSect.dfcd    ;
		WData.dftd      =SpscSect.dftd    ;
		WData.dfskM     =SpscSect.dfskM   ; 
		WData.dfsdtM    =SpscSect.dfsdtM  ;
		WData.dfsdcM    =SpscSect.dfsdcM  ;
		WData.dfskS     =SpscSect.dfskS   ; 
		WData.dfsdtS    =SpscSect.dfsdtS  ; 
		WData.dfsdcS    =SpscSect.dfsdcS  ;
		WData.dfskD     =SpscSect.dfskD   ; 
		WData.dfsdtD    =SpscSect.dfsdtD  ; 
		WData.dfsdcD    =SpscSect.dfsdcD  ;
		for(int i=0; i<2; i++)
		{
			WData.dfpkV[i] = SpscSect.dfpkV[i] ; 
			WData.dfpdtV[i]= SpscSect.dfpdtV[i]; 
			WData.dfpdcV[i]= SpscSect.dfpdcV[i];
		}
		WData.dfcd_PT    = SpscSect.dfcd_PT     ;
		WData.dfcd_A     = SpscSect.dfcd_A      ;
		WData.dfyd_PT    = SpscSect.dfyd_PT     ;
		WData.dfyd_A     = SpscSect.dfyd_A      ;
		WData.dfywd_PT   = SpscSect.dfywd_PT    ;
		WData.dfywd_A    = SpscSect.dfywd_A     ;
		WData.dfcm       = SpscSect.dfcm        ;
		WData.dfctm      = SpscSect.dfctm       ;
		WData.dfctk      = SpscSect.dfctk       ;
		WData.dfcd_PT_s  = SpscSect.dfcd_PT_s   ;
		WData.dfcd_A_s   = SpscSect.dfcd_A_s    ;
		WData.dfyd_PT_s  = SpscSect.dfyd_PT_s   ;
		WData.dfyd_A_s   = SpscSect.dfyd_A_s    ;
		WData.dfywd_PT_s = SpscSect.dfywd_PT_s  ;
		WData.dfywd_A_s  = SpscSect.dfywd_A_s   ;
		WData.dfcm_s     = SpscSect.dfcm_s      ;
		WData.dfctm_s    = SpscSect.dfctm_s     ;
		WData.dfctk_s    = SpscSect.dfctk_s     ;
		return WData;
	}
	static _RPT_FLEX_DATA_W Convert_A2W(const _RPT_FLEX_DATA &AData)
	{
		_RPT_FLEX_DATA_W WData;
		WData.dx            =AData.dx         ;
		WData.dTst          =AData.dTst       ;
		WData.dTps          =AData.dTps       ;
		WData.dCc           =AData.dCc        ;
		WData.dCsc          =AData.dCsc       ;
		WData.dCps          =AData.dCps       ;
		WData.dTens         =AData.dTens      ;
		WData.dComp         =AData.dComp      ;
		WData.dTolerance    =AData.dTolerance ;
		return WData;
	}
	static void Convert_A2W(const _RPT_BDCY_BASE &AData, _RPT_BDCY_BASE_W &WData)
	{
		WData.bChk       = AData.bChk;
		WData.dxib       = AData.dxib;
		WData.dfcd       = AData.dfcd;
		WData.dfsdt      = AData.dfsdt;
		WData.dfsdc      = AData.dfsdc;
		WData.dxCal      = AData.dxCal;
		WData.dx         = AData.dx;
		WData.dxb        = AData.dxb;
		WData.db         = AData.db;
		WData.dbw        = AData.dbw;
		WData.dh         = AData.dh;
		WData.dh0        = AData.dh0;
		WData.dhf        = AData.dhf;
		WData.dat        = AData.dat;
		WData.dac        = AData.dac;
		WData.dast       = AData.dast;
		WData.dasc       = AData.dasc;
		WData.dMd        = AData.dMd;
		WData.dr0Md      = AData.dr0Md;
		WData.dMn        = AData.dMn;
		WData.dTndnComp  = AData.dTndnComp;
		WData.dMnCon     = AData.dMnCon ;
		WData.dMnBarC    = AData.dMnBarC;
		WData.dMnPsC     = AData.dMnPsC ;
		WData.dMnPsT     = AData.dMnPsT ;
		WData.dMnBarT    = AData.dMnBarT;
		WData.dMnPsTs    = AData.dMnPsTs;
		WData.dMnBarTs   = AData.dMnBarTs;
		WData.dMnPsCs    = AData.dMnPsCs;
		for (int i=0; i<AData.arFlex.GetCount(); i++)
		{
			WData.arFlex.Add(Convert_A2W(AData.arFlex[i]));
		}   
		for(int i=0; i<2; i++)
		{
			Convert_A2W  (AData.arRbar[i], WData.arRbar[i]);
			Convert_A2W(AData.arTndn[i], WData.arTndn[i]);
		}
	}


	static _RPT_TNDN_CUMS_W Convert_A2W(const _RPT_TNDN_CUMS &AData)
	{
		_RPT_TNDN_CUMS_W WData;
		WData.bSlope = AData.bSlope;
		WData.dAp    = AData.dAp;
		WData.dfpd   = AData.dfpd;
		WData.dThetap= AData.dThetap;
		WData.strTndnGroup = CT2W(AData.strTndnGroup);
		return WData;
	}
	static void Convert_A2W(const _RPT_CUMS_BASE &AData, _RPT_CUMS_BASE_W &WData)
	{
		WData.bChk    = AData.bChk;    
		WData.dalp1   = AData.dalp1;
		WData.dalp2   = AData.dalp2;
		WData.dalp3   = AData.dalp3;
		WData.dfcuk   = AData.dfcuk;
		WData.dftd    = AData.dftd;
		WData.dfsdt   = AData.dfsdt;
		WData.dfsv    = AData.dfsv;
		WData.db      = AData.db;
		WData.dbw     = AData.dbw;
		WData.dh      = AData.dh;
		WData.dh0     = AData.dh0;
		WData.dhf     = AData.dhf;
		WData.dAsv    = AData.dAsv;
		WData.dSv_s   = AData.dSv_s;
		WData.dThetas = AData.dThetas;
		WData.dAsb    = AData.dAsb;
		WData.dSv_b   = AData.dSv_b;
		WData.dMd     = AData.dMd;
		WData.dVd     = AData.dVd;
		WData.dr0Vd   = AData.dr0Vd;
		WData.dVn     = AData.dVn;
		WData.dVcs    = AData.dVcs;
		WData.dVsb    = AData.dVsb;
		WData.dVpb    = AData.dVpb;
		WData.dAp     = AData.dAp;
		WData.dApb    = AData.dApb;
		WData.dAst    = AData.dAst;
		WData.dP      = AData.dP;
		WData.dPcal   = AData.dPcal;
		WData.dRho    = AData.dRho;
		WData.dRhosv  = AData.dRhosv;
		WData.dm      = AData.dm;
		WData.dm_cal  = AData.dm_cal;
		WData.dC      = AData.dC;
		WData.iAsbN   = AData.iAsbN;
		WData.dShrSect     = AData.dShrSect;
		WData.dShrLoadSkip = AData.dShrLoadSkip;

		for(int i=0; i<AData.arRbarT.GetCount(); i++)
			WData.arRbarT.Add(Convert_A2W(AData.arRbarT[i]));
		for(int i=0; i<AData.arTndnCums.GetCount(); i++)
			WData.arTndnCums.Add(Convert_A2W(AData.arTndnCums[i]));
		
	}

	static void Convert_A2W(const _RPT_CRMT_BASE &AData, _RPT_CRMT_BASE_W &WData)
	{
		WData.bChk          = AData.bChk;    
		WData.dalp1         = AData.dalp1;
		WData.dalp2         = AData.dalp2;
		WData.dalp3         = AData.dalp3;
		WData.dfcuk         = AData.dfcuk;
		WData.dftd          = AData.dftd;
		WData.dfsdt         = AData.dfsdt;
		WData.dfsv          = AData.dfsv;
		WData.db            = AData.db;
		WData.dbw           = AData.dbw;
		WData.dh            = AData.dh;
		WData.dh0           = AData.dh0;
		WData.dhf           = AData.dhf;
		WData.dAsv          = AData.dAsv;
		WData.dSv_s         = AData.dSv_s;
		WData.dMd           = AData.dMd;
		WData.dVd           = AData.dVd;
		WData.dr0Vd         = AData.dr0Vd;
		WData.dVn           = AData.dVn;
		WData.dr0Td         = AData.dr0Td;
		WData.dTn           = AData.dTn;    
		WData.dAst          = AData.dAst;
		WData.dAp           = AData.dAp;
		WData.dApb          = AData.dApb;
		WData.dP            = AData.dP;
		WData.dPcal         = AData.dPcal;
		WData.dRho          = AData.dRho;
		WData.dRhosv        = AData.dRhosv;
		WData.dSTstress     = AData.dSTstress;
		WData.dTorSect      = AData.dTorSect;
		WData.dTorLoadSkip  = AData.dTorLoadSkip;
		WData.dAsv1         = AData.dAsv1;
		WData.dAslt         = AData.dAslt;
		WData.dSv_t         = AData.dSv_t; 
		WData.dBetaa        = AData.dBetaa;
		WData.dBetat        = AData.dBetat;
		WData.dWt           = AData.dWt;
		WData.dAcor         = AData.dAcor;
		WData.dUcor         = AData.dUcor;
		WData.dNp0          = AData.dNp0;
		WData.dep0          = AData.dep0;
		WData.dA0           = AData.dA0;
		WData.bPureTorsion  = AData.bPureTorsion;
		WData.dZeta         = AData.dZeta;
		WData.dZetaCal      = AData.dZetaCal;
		WData.bSolid        = AData.bSolid;

		for(int i=0; i<AData.arTndnCums.GetCount(); i++)
			WData.arTndnCums.Add(Convert_A2W(AData.arTndnCums[i]));
	}

	static _TB_PSC_SECT_W Convert_A2W(const _TB_PSC_SECT &AData)
	{
		_TB_PSC_SECT_W WData;
		WData.db       =      AData.db       ;
		WData.dh       =      AData.dh       ;
		WData.db_f     =      AData.db_f     ;
		WData.dbf      =      AData.dbf      ;
		WData.dh_f     =      AData.dh_f     ;
		WData.dhf      =      AData.dhf      ;
		WData.dAs      =      AData.dAs      ;
		WData.dA_s     =      AData.dA_s     ;
		WData.das      =      AData.das      ;
		WData.da_s     =      AData.da_s     ;
		WData.dAp      =      AData.dAp      ;
		WData.dA_p     =      AData.dA_p     ;
		WData.dApb     =      AData.dApb     ;
		WData.dA_pb    =      AData.dA_pb    ;
		WData.dAv      =      AData.dAv      ; 
		WData.dap      =      AData.dap      ;
		WData.da_p     =      AData.da_p     ; 
		WData.dapb     =      AData.dapb     ;
		WData.da_pb    =      AData.da_pb    ; 
		WData.dApbsina =      AData.dApbsina ;
		WData.dA_pbsina=      AData.dA_pbsina; 
		WData.dApbcosa =      AData.dApbcosa ;
		WData.dA_pbcosa=      AData.dA_pbcosa; 
		WData.dsv      =      AData.dsv      ; 
		WData.dArea    =      AData.dArea    ; 
		WData.dIyy     =      AData.dIyy     ;
		WData.dIzz     =      AData.dIzz     ;
		WData.dCzp     =      AData.dCzp     ;
		WData.dCzm     =      AData.dCzm     ;
		WData.dS0      =      AData.dS0      ;
		WData.dL0      =      AData.dL0      ;
		WData.dU       =      AData.dU       ;
		for(int i =0; i<4; i++)
		{
			WData.dpMomPos[i][0]  =AData.dpMomPos[i][0];
			WData.dpMomPos[i][1]  =AData.dpMomPos[i][1];
		}
		return WData;
	}
	static T_TENDON_LOSS_W Convert_A2W(const T_TENDON_LOSS &AData)
	{
		T_TENDON_LOSS_W WData;
		for (int i=0; i<6; i++) 
			WData.dTendonLoss[i] = AData.dTendonLoss[i] ;
		WData.dEffectiveNum = AData.dEffectiveNum;
		return WData;
	}


	static _DGN_FORC_CRC_W Convert_A2W(const _DGN_FORC_CRC &rData)
	{
		_DGN_FORC_CRC_W WData;
		WData.strType = CT2W(rData.strType);
		for(int i=0; i<cPOSI_CRC; i++)
		{
			WData.iRefNo[i] = rData.iRefNo[i];
			WData.dFxx[i]=rData.dFxx[i]; WData.dFyy[i]=rData.dFyy[i]; WData.dFzz[i]=rData.dFzz[i];
			WData.dMux[i]=rData.dMux[i]; WData.dMuy[i]=rData.dMuy[i]; WData.dMuz[i]=rData.dMuz[i];
			WData.dMbi[i]=rData.dMbi[i]; WData.dTst[i]=rData.dTst[i]; WData.dTwp[i]=rData.dTwp[i];
		}
		return WData;
	}
	
	static void Convert_A2W(const CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP> &SrcD, CArray<T_SEIS_LINK_DISP_W,T_SEIS_LINK_DISP_W> &DestD)
	{
		DestD.RemoveAll();
		for (int i=0; i<SrcD.GetCount(); i++)
		{
			T_SEIS_LINK_DISP_W WData;
			WData.dXDx = SrcD[i].dXDx;
			WData.dXDy = SrcD[i].dXDy;
			WData.dXHx = SrcD[i].dXHx;
			WData.dXHy = SrcD[i].dXHy;
			WData.LcomK = SrcD[i].LcomK;
			WData.LcomName = CT2W(SrcD[i].LcomName);
			DestD.Add(WData);
		}
	}

	static void Convert_A2W(const CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE>  &SrcD, CArray<T_SEIS_LINK_FORCE_W,T_SEIS_LINK_FORCE_W>  &DestD)
	{
		DestD.RemoveAll();
		ASSERT(sizeof(T_SEIS_LINK_FORCE) == sizeof(T_SEIS_LINK_FORCE_W));
		for (int i=0; i<SrcD.GetCount(); i++)
		{
			T_SEIS_LINK_FORCE_W WData;
			WData.dEhzdx = SrcD[i].dEhzdx;
			WData.dEhzdy = SrcD[i].dEhzdy;
			WData.dEhzex = SrcD[i].dEhzex;
			WData.dEhzey = SrcD[i].dEhzey;
			WData.dRb    = SrcD[i].dRb   ;
			WData.LcomK = SrcD[i].LcomK;
			WData.LcomName = CT2W(SrcD[i].LcomName);
			DestD.Add(WData);
		}
	}


};