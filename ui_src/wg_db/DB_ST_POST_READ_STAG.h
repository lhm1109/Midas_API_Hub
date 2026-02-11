#ifndef __DB_ST_POST_READ_STAG_H__
#define __DB_ST_POST_READ_STAG_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define _REAC_STAG_BN _REAC_STLD_BN
#define _LRAC_STAG_BN _LRAC_STLD_BN
#define _DISP_STAG_BN _DISP_STLD_BN 
#define _STRT_STAG_BN _STRT_STLD_BN
#define _STRB_STAG_BN _STRB_STLD_BN
#define _SIMB_STAG_BN _SIMB_STLD_BN
#define _FSIM_STAG_BN _FSIM_STLD_BN
#define _SRIM_STAG_BN _SRIM_STLD_BN
#define _STRM_STAG_BN _STRM_STLD_BN
#define _STRP_STAG_BN _STRP_STLD_BN
#define _STRS_STAG_BN _STRS_STLD_BN
#define _STRW_STAG_BN _STRW_STLD_BN
#define _STPN_STAG_BN _STPN_STLD_BN
#define _STAX_STAG_BN _STAX_STLD_BN
#define _ELNK_STAG_BN _ELNK_STLD_BN
#define _NLNK_STAG_BN _NLNK_STLD_BN
#define _SBCF_STAG_BN _SBCF_THIS_BN
#define _SIMC_STAG_BN _SIMC_THIS_BN
#define _STCM_STAG_BN _STCM_STLD_BN
#define _STCM_STAG_BN_WP _STCM_STLD_BN_WP
#define _RCBL_STAG_BN _RCBL_STLD_BN
#define _RCAE_STAG_BN _RCAE_STLD_BN
#define _PTSB_STAG_BN _SIMB_STLD_BN
#define _PTPF_STAG_BN _SIMB_STLD_BN


struct _TDLC_STAG_BN    // 텐던 장력 손실량
{
	double dTendonLoss[3]; //[0] 탄성변형, [1]크리프/건조수축 [2]Relaxation 에 의한 손실
};

struct _TDEG_STAG_BN
{
	double dStepId;         // 긴장된 스테이지,스텝 순번
	double dElongation[6];  // Tendon 수축량 I,J, 요소수축량 I,J, 두개합한값 I,J
};

struct _ELFM_STAG_BN
{
	double dElemId;
	double dStartMatAge; //각 시공단계에서의 시작 재령
	double dEndMatAge;   //각 시공단계에서의 마지막 재령
	double dStartElast;  //각 시공단계에서의 시작 재령의 탄성계수  
	double dEndElast;    //각 시공단계에서의 마지막 재령의 탄성계수  
	double dAddedCrSkStrain; //현단계까지 누적된 건조수축 변형량 
	double dCreepStrain; //부재의 생성시점을 기준에 재하되는 응력에 대한 크리프 변형률 
	void ConvertOut(T_ELFM_D& Data)
	{
		Data.Initialize();
		Data.dStartMatAge=dStartMatAge;
		Data.dEndMatAge=dEndMatAge;
		Data.dStartElast=dStartElast;
		Data.dEndElast=dEndElast;
		Data.dAddedCrSkStrain=dAddedCrSkStrain;
		Data.dCreepStrain=dCreepStrain;
	}
};

struct _BMPD_STAG_BN
{
	double dStiffness[2][9];   // [i,j] [Area, Ixx, Iyy, Izz, Cyp, Cym, Czp, Czm, WArea]
	double dCentTrans[2][2];   // [i,j] [dy, dz]
	void ConvertOut(T_BMPD_D& Data)
	{
		Data.Initialize();
		
		for(int i=0; i<2; i++)
		{
			Data.dStiffness[i][0] = dStiffness[i][0];
			Data.dStiffness[i][1] = dStiffness[i][1];
			Data.dStiffness[i][2] = dStiffness[i][2];
			Data.dStiffness[i][3] = dStiffness[i][3];
			Data.dStiffness[i][4] = dStiffness[i][4];
			Data.dStiffness[i][5] = dStiffness[i][5];
			Data.dStiffness[i][6] = dStiffness[i][6];
			Data.dStiffness[i][7] = dStiffness[i][7];
			Data.dStiffness[i][8] = dStiffness[i][8];
		}

		ASSERT(sizeof(dCentTrans) == sizeof(Data.dCentTrans));
		memcpy(Data.dCentTrans, dCentTrans, sizeof(dCentTrans));
	}
};

struct _WBPD_STAG_BN
{
	double dStiffness[2][10];   // [i,j] [Area, Ixx, Iyy, Izz, Iw, Cyp, Cym, Czp, Czm, WArea]
	double dCentTrans[2][2];   // [i,j] [dy, dz]
	void ConvertOut(T_BMPD_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dStiffness) == sizeof(Data.dStiffness));
		ASSERT(sizeof(dCentTrans) == sizeof(Data.dCentTrans));
		
		for(int i=0; i<2; i++)
		{
			Data.dStiffness[i][0] = dStiffness[i][0];
			Data.dStiffness[i][1] = dStiffness[i][1];
			Data.dStiffness[i][2] = dStiffness[i][2];
			Data.dStiffness[i][3] = dStiffness[i][3];
			Data.dStiffness[i][4] = dStiffness[i][5];
			Data.dStiffness[i][5] = dStiffness[i][6];
			Data.dStiffness[i][6] = dStiffness[i][7];
			Data.dStiffness[i][7] = dStiffness[i][8];
			Data.dStiffness[i][8] = dStiffness[i][9];
			Data.dStiffness[i][9] = dStiffness[i][4];
		}

		memcpy(Data.dCentTrans, dCentTrans, sizeof(Data.dCentTrans));
	}
};

struct _STSS_STAG_BN
{
	double dblStress[5][10][12];// [5지점][10개 단면지점][9개 성분]
															// 1.Axial X Normal Stress
															// 2.Moment-y X Normal Stress 
															// 3.Moment-z X Normal Stress       
															// 4.강봉 X Normal Stres
															// 5.Sum
															// 6.강봉 z Normal Stress
															// 7.Shear z Shear Stress
															// 8.Torsion Shear Stress  
															// 9.강봉 Shear Stress
															// 10.비틀림 제외 사인장 응력 
															// 11.비틀림 포함 사인장 응력 / 주응력 최대 
															// 12.주응력 최소 
	int    iEl;
	void ConvertOut(T_STSS_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<5; i++)
			for(int j=0; j<10; j++)
				for(int k=0; k<12; k++)
					Data.dblStress[i][j][k] = dblStress[i][j][k];
	}
};

struct _STSW_STAG_BN
{
	double dblStress[5][16][15];// [5지점][16개 단면지점][15개 성분]
	// 1.Axial stress
	// 2.My normal stress
	// 3.Mz normal stress
	// 4.강봉 normal stress
	// 5.warping normal stress
	// 6.Shear Torsion : Sig-xz(torsion)      // (<--로 변경) 6.normal stress summation. // MNET:XXXX-HSSHIM-20130820
	// 7.강봉 z Normal Stress
	// 8.pure torsion xy
	// 9.warping torsion xy
	//10.transvers shear xz
	//11.pure torsion xz
	//12.warping torsion xz
	//13.강봉 shear xz
	//14.summation shear xy
	//15.summation shear xz
 
	int    iEl;
	void ConvertOut(T_STSS_D& Data)
	{
		Data.Initialize();

		for(int i=0; i<5; i++)
			for(int j=0; j<16; j++)
			{
				Data.dblStress[i][j][0] = dblStress[i][j][0];
				Data.dblStress[i][j][1] = dblStress[i][j][1];
				Data.dblStress[i][j][2] = dblStress[i][j][2];
				Data.dblStress[i][j][3] = dblStress[i][j][3];
				Data.dblStress[i][j][4] = dblStress[i][j][5];
				// MNET:XXXX-HSSHIM-20130820.  Sum 자리에 Sig-xz(torsion 성분을 넣어주기로 협의함.)
//         double dSumSigXX = 0.0;
//         dSumSigXX += Data.dblStress[i][j][0];
//         dSumSigXX += Data.dblStress[i][j][1];
//         dSumSigXX += Data.dblStress[i][j][2];
//         dSumSigXX += Data.dblStress[i][j][3];
//         dSumSigXX += Data.dblStress[i][j][4];
//         Data.dblStress[i][j][4] = dSumSigXX;

				Data.dblStress[i][j][5] = dblStress[i][j][6];
				Data.dblStress[i][j][6] = dblStress[i][j][9];
				Data.dblStress[i][j][7] = dblStress[i][j][10];
				Data.dblStress[i][j][8] = dblStress[i][j][12];
				Data.dblStress[i][j][12] = dblStress[i][j][4];
				Data.dblStress[i][j][13] = dblStress[i][j][7];
				Data.dblStress[i][j][14] = dblStress[i][j][8];
				Data.dblStress[i][j][15] = dblStress[i][j][10];
				Data.dblStress[i][j][16] = dblStress[i][j][11];
				Data.dblStress[i][j][17] = dblStress[i][j][13];
				Data.dblStress[i][j][18] = dblStress[i][j][14];
			}
	}
};

struct _BTMS_STAG_BN_OLD
{
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]
	void ConvertOut(T_BTMS_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblLineStress) == sizeof(Data.dblLineStress));
		memcpy(Data.dblLineStress, dblLineStress, sizeof(Data.dblLineStress));
	}
};

struct _BTMS_STAG_BN
{
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]
	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트
	void ConvertOut(T_BTMS_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblLineStress) == sizeof(Data.dblLineStress));
		memcpy(Data.dblLineStress, dblLineStress, sizeof(Data.dblLineStress));
		Data.dN=dN;
		Data.dMy=dMy;
		Data.dMz=dMz;
	}
};

struct _LOFF_STAG_BN
{
	double KeyElem;
	double KeyStag;
	double iStep;
	double KeyNodeI;
	double KeyNodeJ;
	double dPretension;
	double dLofForce;
	double dSum;
	double dLocalVector[3];
	double dAngle;
	double dElastic;
	double dArea;
	double dDisp_I[3];
	double dDisp_J[3];
	double dDeform;

	void Initialize()
	{
		KeyElem = 0.0;
		KeyStag = 0.0;
		iStep = 0.0;
		KeyNodeI = 0.0;
		KeyNodeJ = 0.0;
		for(int i=0; i<3; i++) dLocalVector[i] = 0.0;
		dAngle = 0.0;
		dElastic = 0.0;
		dArea = 0.0;
		for(int i = 0; i<3; i++) dDisp_I[i]=0.0;
		for(int i = 0; i<3; i++) dDisp_J[i]=0.0;
		dDeform=0.0;
		dPretension=0.0;
		dLofForce=0.0;
		dSum=0.0;
	}
};

struct _LOFB_STAG_BN
{
	double KeyElem;
	double KeyStag;
	double iStep;
	double KeyNodeI;
	double KeyNodeJ;
	double dForce_I[6];
	double dForce_J[6];
	double dDisp_I[6];
	double dDisp_J[6];

	void Initialize()
	{
		KeyElem = 0.0;
		KeyStag = 0.0;
		iStep = 0.0;
		KeyNodeI = 0.0;
		KeyNodeJ = 0.0;
		for(int i=0; i<6; i++) 
		{
			dForce_I[i] = 0.0;
			dForce_J[i] = 0.0;
			dDisp_I[i] = 0.0;
			dDisp_J[i] = 0.0;
		}
	}
};

struct _LOFW_STAG_BN
{
	double KeyElem;
	double KeyStag;
	double iStep;
	double KeyNodeI;
	double KeyNodeJ;
	double dForce_I[6];
	double dForce_J[6];
	double dBiMoment_I;
	double dBiMoment_J;
	double dDisp_I[6];
	double dDisp_J[6];
	double dRw_I;
	double dRw_J;
	
	void Initialize()
	{
		KeyElem = 0.0;
		KeyStag = 0.0;
		iStep = 0.0;
		KeyNodeI = 0.0;
		KeyNodeJ = 0.0;
		for(int i=0; i<6; i++) 
		{
			dForce_I[i] = 0.0;
			dForce_J[i] = 0.0;
			dDisp_I[i] = 0.0;
			dDisp_J[i] = 0.0;
		}
		dBiMoment_I = 0.0;
		dBiMoment_J = 0.0;
		dRw_I = 0.0;
		dRw_J = 0.0;
	}
};

struct _LOFP_STAG_BN
{
	double KeyElem;
	double SeqElem;
	double KeyStag;
	double iStep;
	double KeyNode[4];
	double dForce[4][6];
	double dDisp[4][6];

	void Initialize()
	{
		KeyElem = 0.0;
		SeqElem = 0.0;
		KeyStag = 0.0;
		iStep = 0.0;

		for(int i=0; i<4; i++)
		{
			KeyNode[i] = 0.0;

			for(int j=0; j<6; j++)
			{
				dForce[i][j] = 0.0;
				dDisp[i][j] = 0.0;
			}
		}
	}
};

struct _CFSI_STAG_7DOF_BN
{
	double dblForce[2][7]; // i,j-end // Fx,Fy,Fz,Mx,My,Mz
	double nNumElem;
	double nTypeElem;      // 1:Truss, 2:Beam Type
	void Initialize()
	{
		memset(this, 0, sizeof(_CFSI_STAG_7DOF_BN));
	}
	void ConvertOut(T_INMF_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)
		{
			Data.dElemForce[0][i]=dblForce[0][i];
			Data.dElemForce[1][i]=dblForce[1][i];
		}
		Data.ElemKey=(UINT)nNumElem;
		Data.nType=(int)nTypeElem;
	}
};

struct _CFSI_STAG_BN
{
	double dblForce[2][6]; // i,j-end // Fx,Fy,Fz,Mx,My,Mz
	double nNumElem;
	double nTypeElem;      // 1:Truss, 2:Beam Type
	void Initialize()
	{
		memset(this, 0, sizeof(_CFSI_STAG_BN));
	}
	void ConvertOut(T_INMF_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)
		{
			Data.dElemForce[0][i]=dblForce[0][i];
			Data.dElemForce[1][i]=dblForce[1][i];
		}
		Data.ElemKey=(UINT)nNumElem;
		Data.nType=(int)nTypeElem;
	}
};

#pragma pack()

#endif