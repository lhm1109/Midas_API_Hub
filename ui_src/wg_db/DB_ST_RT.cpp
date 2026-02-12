#include "stdafx.h"

#include "DB_ST_DT_UNIT.h"
#include "DB_ST_DT.h"
#include "DB_ST_DT_CASE.h"

#include "DB_ST_RT.h"

#include <math.h>

/////////////////////////////////////////
// T_STRB_D

void T_STRB_D::Max(T_STRB_D& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			if(Data.dblStress[i][k] > dblStress[i][k])dblStress[i][k]=Data.dblStress[i][k];
		}
	}
}
void T_STRB_D::Min(T_STRB_D& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			if(Data.dblStress[i][k] < dblStress[i][k])dblStress[i][k]=Data.dblStress[i][k];
		}
	}
}
void T_STRB_D::AbsMax(T_STRB_D& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=__max(fabs(dblStress[i][k]),fabs(Data.dblStress[i][k]));
		}
	}
}
void T_STRB_D::AbsMax(T_STRB_D& Data1, T_STRB_D& Data2)
{
	Initialize();
	nDOFType = (Data1.nDOFType = Data2.nDOFType)? Data1.nDOFType : 1; 
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=__max(fabs(Data1.dblStress[i][k]),fabs(Data2.dblStress[i][k]));
		}
	}
}
void T_STRB_D::AbsMaxwithSign(T_STRB_D& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=(fabs(dblStress[i][k]) >= fabs(Data.dblStress[i][k]) ? dblStress[i][k] : Data.dblStress[i][k]);
		}
	}
}
void T_STRB_D::AbsMaxwithSign(T_STRB_D& Data1, T_STRB_D& Data2)
{
	Initialize();
	nDOFType = (Data1.nDOFType = Data2.nDOFType)? Data1.nDOFType : 1; 
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=(fabs(Data1.dblStress[i][k]) >= fabs(Data2.dblStress[i][k]) ? Data1.dblStress[i][k] : Data2.dblStress[i][k]);
		}
	}
}
void T_STRB_D::Add(T_STRB_D& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]+=Data.dblForce[i][j];
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]+=Data.dblStress[i][k];
		}
	}
}
void T_STRB_D::Abs()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=fabs(dblForce[i][j]);
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
}
void T_STRB_D::Sqr()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
		}
	}
}
void T_STRB_D::Root()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]=sqrt(dblForce[i][j]);
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]=sqrt(dblStress[i][k]);
		}
	}
}
void T_STRB_D::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<18; j++)
		{
			if(j==12&&nDOFType!=2) break;
			dblForce[i][j]*=dFactor;
		}
		for(int k=0; k<80; k++)
		{
			if(k==24&&nDOFType!=2) break;
			dblStress[i][k]*=dFactor;
		}
	}
}
void T_STRB_D::calcMaximumCombinedStress()
{
	// Combined Stress
	for(int i=0; i<4; i++)
	{
		//            0    1    2    3     4     5     6     7    8    9    10    11    12    13
		//component : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj, 
		//            Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j
		//dblStress[i][16] = dblStress[i][0]+dblStress[i][4]+dblStress[i][5];  // -y, +z
		//dblStress[i][18] = dblStress[i][0]+dblStress[i][3]+dblStress[i][5];  // +y, +z
		//dblStress[i][20] = dblStress[i][0]+dblStress[i][3]+dblStress[i][6];  // +y, -z
		//dblStress[i][22] = dblStress[i][0]+dblStress[i][4]+dblStress[i][6];  // -y, -z
		//dblStress[i][17] = dblStress[i][7+0]+dblStress[i][7+4]+dblStress[i][7+5];  // -y, +z
		//dblStress[i][19] = dblStress[i][7+0]+dblStress[i][7+3]+dblStress[i][7+5];  // +y, +z
		//dblStress[i][21] = dblStress[i][7+0]+dblStress[i][7+3]+dblStress[i][7+6];  // +y, -z
		//dblStress[i][23] = dblStress[i][7+0]+dblStress[i][7+4]+dblStress[i][7+6];  // -y, -z
		const double dRegardZero = 1.0e-12;
		double cb1=dblStress[i][16], cb2=dblStress[i][17];
		double dEqualCompare = 0.0;
		for(int j=0; j<6; j+=2)
		{
			// MNET:XXXX-HSSHIM-20101214. 
			// 동일한 값을 가지고 비교할 때 앞에 것이 먼저 나오면 계속해서 앞에 것이 나오도록 수정.
			// Tappered Pipe의 경우 아주 작은 tolerance 차이를 두고 Comb2i와 Comb4i가 번갈아가면서 나와서 Display시에 combined stress 형상이 비뚤어지는 경우 수정 
			double dblStress_18_j = dblStress[i][18+j];
			double dblStress_19_j = dblStress[i][19+j];
			if(fabs(cb1)+dRegardZero < fabs(dblStress_18_j))
			{
				if (fabs(cb1) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_18_j)/fabs(cb1) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb1=dblStress_18_j;
				}
				else   cb1=dblStress_18_j;
			}
			if(fabs(cb2)+dRegardZero < fabs(dblStress_19_j))
			{
				if (fabs(cb2) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_19_j)/fabs(cb2) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb2=dblStress_19_j;
				}
				else   cb2=dblStress_19_j;
			}
		}
		dblStress[i][14] = cb1;
		dblStress[i][15] = cb2;
	}
}
double T_STRB_D::GetMaximumStress_Warping(int part, int node)
{
	//dblStress[i][24], dblStress[i][25], dblStress[i][26], dblStress[i][27] : warping normal stress(1,2,3,4) i
	//dblStress[i][44], dblStress[i][45], dblStress[i][46], dblStress[i][47] : warping normal stress(1,2,3,4) j
	double dMaximum=0.;
	// Combined Stress
	for(int i=0; i<4; i++)
	{
		if(i!=part) continue;
		const double dRegardZero = 1.0e-12;
		double dCombined=dblStress[i][24+node*20];
		double dEqualCompare = 0.0;
		for(int j=0; j<3; j++)
		{
			double dblStress_j = dblStress[i][25+node*20+j];
			if(fabs(dCombined)+dRegardZero < fabs(dblStress_j))
			{
				if (fabs(dCombined) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_j)/fabs(dCombined) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  dCombined=dblStress_j;
				}
				else   dCombined=dblStress_j;
			}
		}
		dMaximum = dCombined;
	}
	return dMaximum;
}
double T_STRB_D::GetMaximumStress_Torsional(int part, int node, int nType)
{
	int nStartIndex = 0;
	switch(nType)
	{
	case 1:   nStartIndex=28+node*20; break; //Ssy mt
	case 3:   nStartIndex=29+node*20; break; //Ssz mt
	case 2:   nStartIndex=36+node*20; break; //Ssy mw
	case 4:   nStartIndex=37+node*20; break; //Ssz mw
	default:  ASSERT(0); break;
	}
	//dblStress[i][28]~dblStress[i][43] : ST. Venant torsional stress (xy1, xz1, xy2, xz2..xy4,xz4) i, warping torsional stress(xy1, xz1, xy2, xz2..xy4,xz4) i
	//dblStress[i][48]~dblStress[i][63] : ST. Venant torsional stress (xy1, xz1, xy2, xz2..xy4,xz4) j, warping torsional stress(xy1, xz1, xy2, xz2..xy4,xz4) j
	double dMaximum=0.;
	// Combined Stress
	for(int i=0; i<4; i++)
	{
		if(i!=part) continue;
		const double dRegardZero = 1.0e-12;
		double dCombined=dblStress[i][nStartIndex];
		double dEqualCompare = 0.0;
		for(int j=1; j<4; j++)
		{
			double dblStress_j = dblStress[i][nStartIndex+j*2];
			if(fabs(dCombined)+dRegardZero < fabs(dblStress_j))
			{
				if (fabs(dCombined) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_j)/fabs(dCombined) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  dCombined=dblStress_j;
				}
				else   dCombined=dblStress_j;
			}
		}
		dMaximum = dCombined;
	}
	return dMaximum;
}
double T_STRB_D::GetMaximumCombinedStress_Torsional(int part, int node, int nType)
{
	int nStartIndex = 0;
	switch(nType)
	{
	case 5:   nStartIndex=64+node; break; //Combined Ssy
	case 6:   nStartIndex=72+node; break; //Combined Ssz
	default:  ASSERT(0); break;
	}
	//dblStress[i][64]~dblStress[i][79] : CombTau1i, CombTau1j .... CombTau8i, CombTau8j
	double dMaximum=0.;
	// Combined Stress
	for(int i=0; i<4; i++)
	{
		if(i!=part) continue;
		const double dRegardZero = 1.0e-12;
		double dCombined=dblStress[i][nStartIndex];
		double dEqualCompare = 0.0;
		for(int j=1; j<4; j++)
		{
			double dblStress_j = dblStress[i][nStartIndex+j*2];
			if(fabs(dCombined)+dRegardZero < fabs(dblStress_j))
			{
				if (fabs(dCombined) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_j)/fabs(dCombined) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  dCombined=dblStress_j;
				}
				else   dCombined=dblStress_j;
			}
		}
		dMaximum = dCombined;
	}
	return dMaximum;
}

//////////////////////////////////////////////////////////////////////////
// FSIM
void T_FSIM_D::Max(T_FSIM_D& Data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j] = Data.dblForce[i][j];
		}
		for (int k = 0; k < 10; k++)
		{
			if (Data.dblStress[i][k] > dblStress[i][k])dblStress[i][k] = Data.dblStress[i][k];
		}
	}
}
void T_FSIM_D::Min(T_FSIM_D& Data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j] = Data.dblForce[i][j];
		}
		for (int k = 0; k < 10; k++)
		{
			if (Data.dblStress[i][k] < dblStress[i][k])dblStress[i][k] = Data.dblStress[i][k];
		}
	}
}
void T_FSIM_D::AbsMax(T_FSIM_D& Data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = __max(fabs(dblForce[i][j]), fabs(Data.dblForce[i][j]));
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = __max(fabs(dblStress[i][k]), fabs(Data.dblStress[i][k]));
		}
	}
}
void T_FSIM_D::AbsMax(T_FSIM_D& Data1, T_FSIM_D& Data2)
{
	Initialize();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = __max(fabs(Data1.dblForce[i][j]), fabs(Data2.dblForce[i][j]));
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = __max(fabs(Data1.dblStress[i][k]), fabs(Data2.dblStress[i][k]));
		}
	}
}
void T_FSIM_D::AbsMaxwithSign(T_FSIM_D& Data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			dblForce[i][j] = (fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = (fabs(dblStress[i][k]) >= fabs(Data.dblStress[i][k]) ? dblStress[i][k] : Data.dblStress[i][k]);
		}
	}
}
void T_FSIM_D::AbsMaxwithSign(T_FSIM_D& Data1, T_FSIM_D& Data2)
{
	Initialize();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = (fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = (fabs(Data1.dblStress[i][k]) >= fabs(Data2.dblStress[i][k]) ? Data1.dblStress[i][k] : Data2.dblStress[i][k]);
		}
	}
}
void T_FSIM_D::Add(T_FSIM_D& Data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] += Data.dblForce[i][j];
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] += Data.dblStress[i][k];
		}
	}
}
void T_FSIM_D::Abs()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = fabs(dblForce[i][j]);
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = fabs(dblStress[i][k]);
		}
	}
}
void T_FSIM_D::Sqr()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = dblForce[i][j] * dblForce[i][j];
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = dblStress[i][k] * dblStress[i][k];
		}
	}
}
void T_FSIM_D::Root()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] = sqrt(dblForce[i][j]);
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] = sqrt(dblStress[i][k]);
		}
	}
}
void T_FSIM_D::Mult(double dFactor)
{
	if (dFactor == 1.)return;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dblForce[i][j] *= dFactor;
		}
		for (int k = 0; k < 10; k++)
		{
			dblStress[i][k] *= dFactor;
		}
	}
}
void T_FSIM_D::calcMaximumCombinedStress()
{
	// Combined Stress
	for (int i = 0; i < 4; i++)
	{
		//            0      1       2       3          8       9
		//component : Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j
		const double dRegardZero = 1.0e-12;
		double cb1 = dblStress[i][2], cb2 = dblStress[i][3]; // Init
		double dEqualCompare = 0.0;
		for (int j = 0; j < 6; j += 2)
		{
			// MNET:XXXX-HSSHIM-20101214. 
			// 동일한 값을 가지고 비교할 때 앞에 것이 먼저 나오면 계속해서 앞에 것이 나오도록 수정.
			// Tappered Pipe의 경우 아주 작은 tolerance 차이를 두고 Comb2i와 Comb4i가 번갈아가면서 나와서 Display시에 combined stress 형상이 비뚤어지는 경우 수정 
			double dblStress_1 = dblStress[i][4 + j];
			double dblStress_2 = dblStress[i][5 + j];
			if (fabs(cb1) + dRegardZero < fabs(dblStress_1))
			{
				if (fabs(cb1) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_1) / fabs(cb1) - 1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb1 = dblStress_1;
				}
				else   cb1 = dblStress_1;
			}
			if (fabs(cb2) + dRegardZero < fabs(dblStress_2))
			{
				if (fabs(cb2) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_2) / fabs(cb2) - 1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb2 = dblStress_2;
				}
				else   cb2 = dblStress_2;
			}
		}
		dblStress[i][0] = cb1;
		dblStress[i][1] = cb2;
	}
}

void T_FSIM_D::ConvertToStrb(T_STRB_D& StrbD)
{
	StrbD.nDOFType = 1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			StrbD.dblForce[i][j] = dblForce[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		StrbD.dblStress[i][14] = dblStress[i][0]; // CombStrs I
		StrbD.dblStress[i][15] = dblStress[i][1]; // CombStrs J
		StrbD.dblStress[i][16] = dblStress[i][2]; // CombStrs_1 I
		StrbD.dblStress[i][17] = dblStress[i][3]; // CombStrs_2 J
		StrbD.dblStress[i][18] = dblStress[i][4]; // CombStrs_1 I
		StrbD.dblStress[i][19] = dblStress[i][5]; // CombStrs_2 J
		StrbD.dblStress[i][20] = dblStress[i][6]; // CombStrs_1 I
		StrbD.dblStress[i][21] = dblStress[i][7]; // CombStrs_2 J
		StrbD.dblStress[i][22] = dblStress[i][8]; // CombStrs_1 I
		StrbD.dblStress[i][23] = dblStress[i][9]; // CombStrs_2 J
	}
}


//////////////////////////////////////////////////////////////////////////
// SRIM
void T_SRIM_D::Max(T_SRIM_D& Data)
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		if (Data.dblStress[i] > dblStress[i])dblStress[i] = Data.dblStress[i];
	}
}
void T_SRIM_D::Min(T_SRIM_D& Data)
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		if (Data.dblStress[i] < dblStress[i])dblStress[i] = Data.dblStress[i];
	}
}
void T_SRIM_D::AbsMax(T_SRIM_D& Data)
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = __max(fabs(dblStress[i]), fabs(Data.dblStress[i]));
	}
}
void T_SRIM_D::AbsMax(T_SRIM_D& Data1, T_SRIM_D& Data2)
{
	Initialize();
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = __max(fabs(Data1.dblStress[i]), fabs(Data2.dblStress[i]));
	}
}
void T_SRIM_D::AbsMaxwithSign(T_SRIM_D& Data)
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = (fabs(dblStress[i]) >= fabs(Data.dblStress[i]) ? dblStress[i] : Data.dblStress[i]);
	}
}
void T_SRIM_D::AbsMaxwithSign(T_SRIM_D& Data1, T_SRIM_D& Data2)
{
	Initialize();
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = (fabs(Data1.dblStress[i]) >= fabs(Data2.dblStress[i]) ? Data1.dblStress[i] : Data2.dblStress[i]);
	}
}
void T_SRIM_D::Add(T_SRIM_D& Data)
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] += Data.dblStress[i];
	}
}
void T_SRIM_D::Abs()
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = fabs(dblStress[i]);
	}
}
void T_SRIM_D::Sqr()
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = dblStress[i] * dblStress[i];
	}
}
void T_SRIM_D::Root()
{
	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] = sqrt(dblStress[i]);
	}
}
void T_SRIM_D::Mult(double dFactor)
{
	if (dFactor == 1.)return;

	for (int i = 0; i < SRIM_SIZE; i++)
	{
		dblStress[i] *= dFactor;
	}
}

/////////////////////////////////////////
// T_SCFP_D
// Self Constraint Beam Force &Stress
void T_SCFP_D::Max(T_SCFP_D& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(Data.dbForce[i][j] > dbForce[i][j]) dbForce[i][j]=Data.dbForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(Data.dbStress[i][k] > dbStress[i][k]) dbStress[i][k]=Data.dbStress[i][k];
		}
		for(int l=0; l<19; l++)
		{
			if(Data.dbLStress[i][l] > dbLStress[i][l]) dbLStress[i][l]=Data.dbLStress[i][l];
		}
	}
}
void T_SCFP_D::Min(T_SCFP_D& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(Data.dbForce[i][j] < dbForce[i][j]) dbForce[i][j]=Data.dbForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(Data.dbStress[i][k] < dbStress[i][k]) dbStress[i][k]=Data.dbStress[i][k];
		}
		for(int l=0; l<19; l++)
		{
			if(Data.dbLStress[i][l] < dbLStress[i][l]) dbLStress[i][l]=Data.dbLStress[i][l];
		}
	}
}
void T_SCFP_D::AbsMax(T_SCFP_D& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=__max(fabs(dbForce[i][j]),fabs(Data.dbForce[i][j]));
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=__max(fabs(dbStress[i][k]),fabs(Data.dbStress[i][k]));
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=__max(fabs(dbLStress[i][l]),fabs(Data.dbLStress[i][l]));
		}
	}
}
void T_SCFP_D::AbsMax(T_SCFP_D& Data1, T_SCFP_D& Data2)
{
	Initialize();
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=__max(fabs(Data1.dbForce[i][j]),fabs(Data2.dbForce[i][j]));
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=__max(fabs(Data1.dbStress[i][k]),fabs(Data2.dbStress[i][k]));
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=__max(fabs(Data1.dbLStress[i][l]),fabs(Data2.dbLStress[i][l]));
		}
	}
}
void T_SCFP_D::AbsMaxwithSign(T_SCFP_D& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=(fabs(dbForce[i][j]) >= fabs(Data.dbForce[i][j]) ? dbForce[i][j] : Data.dbForce[i][j]);
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=(fabs(dbStress[i][k]) >= fabs(Data.dbStress[i][k]) ? dbStress[i][k] : Data.dbStress[i][k]);
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=(fabs(dbLStress[i][l]) >= fabs(Data.dbLStress[i][l]) ? dbLStress[i][l] : Data.dbLStress[i][l]);
		}
	}
}
void T_SCFP_D::AbsMaxwithSign(T_SCFP_D& Data1, T_SCFP_D& Data2)
{
	Initialize();
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=(fabs(Data1.dbForce[i][j]) >= fabs(Data2.dbForce[i][j]) ? Data1.dbForce[i][j] : Data2.dbForce[i][j]);
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=(fabs(Data1.dbStress[i][k]) >= fabs(Data2.dbStress[i][k]) ? Data1.dbStress[i][k] : Data2.dbStress[i][k]);
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=(fabs(Data1.dbLStress[i][l]) >= fabs(Data2.dbLStress[i][l]) ? Data1.dbLStress[i][l] : Data2.dbLStress[i][l]);
		}
	}
}
void T_SCFP_D::Add(T_SCFP_D& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]+=Data.dbForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]+=Data.dbStress[i][k];
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]+=Data.dbLStress[i][l];
		}
	}
}
void T_SCFP_D::Abs()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=fabs(dbForce[i][j]);
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=fabs(dbStress[i][k]);
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=fabs(dbLStress[i][l]);
		}
	}
}
void T_SCFP_D::Sqr()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=dbForce[i][j]*dbForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=dbStress[i][k]*dbStress[i][k];
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=dbLStress[i][l]*dbLStress[i][l];
		}
	}
}
void T_SCFP_D::Root()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]=sqrt(dbForce[i][j]);
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]=sqrt(dbStress[i][k]);
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]=sqrt(dbLStress[i][l]);
		}
	}
}
void T_SCFP_D::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			dbForce[i][j]*=dFactor;
		}
		for(int k=0; k<11; k++)
		{
			dbStress[i][k]*=dFactor;
		}
		for(int l=0; l<19; l++)
		{
			dbLStress[i][l]*=dFactor;
		}
	}
}
void T_SCFP_D::calcMaximumCombinedStress()
{
	// Combined Stress
	for(int i=0; i<5; i++)
	{
		const double dRegardZero = 1.0e-12;
		double cb=dbStress[i][5];
		double dEqualCompare = 0.0;
		for(int j=0; j<4; j++)
		{
			double dbStress_j = dbStress[i][7+j];

			if(fabs(cb)+dRegardZero < fabs(dbStress_j))
			{
				if (fabs(cb) > dRegardZero)
				{
					dEqualCompare = fabs(dbStress_j)/fabs(cb) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb=dbStress_j;
				}
				else   cb=dbStress_j;
			}
		}
		dbStress[i][5] = cb;
	}
}

/////////////////////////////////////////
// T_SBCF_D
void T_SBCF_D::Max(T_SBCF_D& Data)
{
#if 1
	if (Data.bStressTable) // (CTestEnvMgr::GetTestEnvST(_T("Test_Envelop")) == _T("yes"))
	{
		MaxMod(Data);
		return;
	}
	int i,j,k,l;
	double *pForce = nullptr;
	double *pForceD = nullptr;
	double *pStress = nullptr;
	double *pStressD = nullptr;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
			{
				pForce = &dblForce[i][j][k][0];
				pForceD = &Data.dblForce[i][j][k][0];
				if (pForceD[k] >= pForce[k])
	{
					//for (l = 0; l < 9; l++)
					//	dblForce[i][j][k][l] = Data.dblForce[i][j][k][l];
					pForce[0] = pForceD[0];
					pForce[1] = pForceD[1];
					pForce[2] = pForceD[2];
					pForce[3] = pForceD[3];
					pForce[4] = pForceD[4];
					pForce[5] = pForceD[5];
					pForce[6] = pForceD[6];
					pForce[7] = pForceD[7];
					pForce[8] = pForceD[8];
	}

				pStress = &dblStress[i][j][k][0];
				pStressD = &Data.dblStress[i][j][k][0];
				if (k == 5)  // Bend +-y
				{
					if (pStressD[3] > pStress[3])
					{
						for (l = 0; l < 40; l++)
							pStress[l] = pStressD[l];
					}
				}
				else if (k == 4) // Bend +-z
				{
					if (pStressD[5] > pStress[5])
					{
						for (l = 0; l < 40; l++)
							pStress[l] = pStressD[l];
					}
				}
				else
				{
					if (pStressD[k] > pStress[k])
					{
						for (l = 0; l < 40; l++)
							pStress[l] = pStressD[l];
					}
				}
			}
	
			#if 0
			for (i = 0; i < 4; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < 9; k++)
	{
		if(k==5)  // Bend +-y
		{
			if(Data.dblStress[i][j][k][3] > dblStress[i][j][k][3])
			{
								for (l = 0; l < 40; l++)
									dblStress[i][j][k][l] = Data.dblStress[i][j][k][l];
			}
		}
		else if(k==4) // Bend +-z
		{
			if(Data.dblStress[i][j][k][5] > dblStress[i][j][k][5])
			{
								for (l = 0; l < 40; l++)
									dblStress[i][j][k][l] = Data.dblStress[i][j][k][l];
			}
		}
		else
		{
			double d1 = Data.dblStress[i][j][k][k];
			double d2 = dblStress[i][j][k][k];
			if(d1 > d2)
			{
					for(l=0; l<40; l++)dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
			}
		}
			#endif
			
#else
	int i,j,k,l;
	for(i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				if(Data.dblForce[i][j][k][k] >= dblForce[i][j][k][k])
				{
					for(l=0; l<9; l++)
						dblForce[i][j][k][l]=Data.dblForce[i][j][k][l];
	}

	for(i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
			{
				if(k==5)  // Bend +-y
				{
					if(Data.dblStress[i][j][k][3] > dblStress[i][j][k][3])
					{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
					}
				}
				else if(k==4) // Bend +-z
				{
					if(Data.dblStress[i][j][k][5] > dblStress[i][j][k][5])
					{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
					}
				}
				else
				{
					if(Data.dblStress[i][j][k][k] > dblStress[i][j][k][k])
					{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
					}
				}
			}
#endif
// 	int nDof = (b7thDOF)? 9 : 6;
// 	double dMaxVal1 =0.0;
// 	double dMaxVal2 =0.0;
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(l=0; l<nDof; l++)
// 	{
// 		dMaxVal1 = -1.0e12;
// 		dMaxVal2 = -1.0e12;
// 		for(k=0; k<nDof; k++)
// 	{  
// 			dMaxVal1 = max(dMaxVal1, Data.dblForce[i][j][k][l]);
// 			dMaxVal2 = max(dMaxVal2, dblForce[i][j][k][l]);
// 			}
// 		for(k=0; k<nDof; k++) dblForce[i][j][k][l] = max(dMaxVal1, dMaxVal2);
// 		}
// 
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(l=0; l<40; l++)
// 		{
// 		dMaxVal1 = -1.0e12;
// 		dMaxVal2 = -1.0e12;
// 		for(k=0; k<nDof; k++)
// 			{
// 			dMaxVal1 = max(dMaxVal1, Data.dblStress[i][j][k][l]);
// 			dMaxVal2 = max(dMaxVal2, dblStress[i][j][k][l]);
// 			}
// 
// 		for(k=0; k<nDof; k++) dblStress[i][j][k][l] = max(dMaxVal1, dMaxVal2);
// 		}

}

void T_SBCF_D::Min(T_SBCF_D& Data)
{
	if (Data.bStressTable) // (CTestEnvMgr::GetTestEnvST(_T("Test_Envelop")) == _T("yes"))
	{
		MinMod(Data);
		return;
	}

	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for (k = 0; k < 9; k++)
			{
	if(Data.dblForce[i][j][k][k] < dblForce[i][j][k][k])
	{
					for (l = 0; l < 9; l++)
						dblForce[i][j][k][l] = Data.dblForce[i][j][k][l];
				}
	}

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
	{  
		if(k==5) // Bend +-y
		{
			if(Data.dblStress[i][j][k][3] < dblStress[i][j][k][3])
			{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
			}
		}
		else if(k==4) // Bend +-z
		{
			if(Data.dblStress[i][j][k][5] < dblStress[i][j][k][5])
			{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
			}
		}
		else
		{
			if(Data.dblStress[i][j][k][k] < dblStress[i][j][k][k])
			{
						for(l=0; l<40; l++)
							dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
			}
		}
	}

// 	int nDof = (b7thDOF)? 9 : 6;
// 
// 	double dMinVal1 = 0.0;
// 	double dMinVal2 = 0.0;
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(l=0; l<nDof; l++)
// 	{
// 		dMinVal1 = 1.0e12;
// 		dMinVal2 = 1.0e12;
// 		for(k=0; k<nDof; k++)
// 		{
// 			dMinVal1 = min(dMinVal1, Data.dblForce[i][j][k][l]);
// 			dMinVal2 = min(dMinVal2, dblForce[i][j][k][l]);
// 		}
// 
// 		for(k=0; k<nDof; k++) dblForce[i][j][k][l] = min(dMinVal1, dMinVal2);
// 	}
// 
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(l=0; l<40; l++)
// 	{
// 		dMinVal1 = 1.0e12;
// 		dMinVal2 = 1.0e12;
// 		for(k=0; k<nDof; k++)
// 		{
// 			dMinVal1 = min(dMinVal1, Data.dblStress[i][j][k][l]);
// 			dMinVal2 = min(dMinVal2, dblStress[i][j][k][l]);
// 		}
// 
// 		for(k=0; k<nDof; k++) dblStress[i][j][k][l] = min(dMinVal1, dMinVal2);
// 	}

}

void T_SBCF_D::MaxMod(T_SBCF_D& Data)
{
	int i, j, k, l;
	for (i = 0; i < 4; i++)for (j = 0; j < 2; j++)for (k = 0; k < 9; k++)
		if (Data.dblForce[i][j][k][k] >= dblForce[i][j][k][k])
		{
			for (l = 0; l < 9; l++)dblForce[i][j][k][l] = Data.dblForce[i][j][k][l];
		}

	for (i = 0; i < 4; i++)for (j = 0; j < 2; j++)for (k = 0; k < 9; k++)
	{
		double d1 = Data.dblStress[i][j][k][k];
		double d2 = dblStress[i][j][k][k];
		if (d1 > d2)
		{
			for (l = 0; l < 40; l++)dblStress[i][j][k][l] = Data.dblStress[i][j][k][l];
		}
	}
}


void T_SBCF_D::MinMod(T_SBCF_D& Data)
{
	int i, j, k, l;
	for (i = 0; i < 4; i++)for (j = 0; j < 2; j++)for (k = 0; k < 9; k++)
		if (Data.dblForce[i][j][k][k] < dblForce[i][j][k][k])
		{
			for (l = 0; l < 9; l++)dblForce[i][j][k][l] = Data.dblForce[i][j][k][l];
		}

	for (i = 0; i < 4; i++)for (j = 0; j < 2; j++)for (k = 0; k < 9; k++)
	{
		if (Data.dblStress[i][j][k][k] < dblStress[i][j][k][k])
		{
			for (l = 0; l < 40; l++)dblStress[i][j][k][l] = Data.dblStress[i][j][k][l];
		}
	}
}

void T_SBCF_D::ModSbcfStress(T_SBCF_D& Data)
{
	// k열의 의미 변경 
	// 현재의 SBCF의 stress는 Fx, Fy, Fz, My, Mz에 의한 stress 임
	// 이를 응력 기준으로 변경함.
	// 
	// (기존)0=Fx, 1=Fy, 2=Fz, 3=Mx, 4=My, 5=Mz, 6=Bi
	// (변경)  Sx,   Sy,   Sz,  Byp,  Bym,  Bzp,  Bzm,

	int i, j, k, l;
	for (i = 0; i < 4; i++)for (j = 0; j < 2; j++)for (k = 0; k < 9; k++)
	{
		if (k == 5)      // Mz에 의한 stress =>  Byp,  Bym
		{
			for (l = 0; l < 40; l++)
			{
				dblStress[i][j][3][l] = Data.dblStress[i][j][k][l];
				dblStress[i][j][4][l] = Data.dblStress[i][j][k][l];
			}
		}
		else if (k == 4) // My에 의한 stress =>  Bzp,  Bzm
		{
			for (l = 0; l < 40; l++)
			{
				dblStress[i][j][5][l] = Data.dblStress[i][j][k][l];
				dblStress[i][j][6][l] = Data.dblStress[i][j][k][l];
			}
		}
	}
}

// void T_SBCF_D::AbsMax(T_SBCF_D& Data)
// {
// 	int i,j,k,l;
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
// 	if(fabs(Data.dblForce[i][j][k][k]) > fabs(dblForce[i][j][k][k]))
// 	{
// 		for(l=0; l<9; l++)dblForce[i][j][k][l]=fabs(Data.dblForce[i][j][k][l]);
// 	}
// 
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
// 	{
// 		if(k==5) // Bend +-y
// 		{
// 			if(fabs(Data.dblStress[i][j][k][3]) > fabs(dblStress[i][j][k][3]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data.dblStress[i][j][k][l]);
// 			}
// 		}
// 		else if(k==4) // Bend +-z 
// 		{
// 			if(fabs(Data.dblStress[i][j][k][5]) > fabs(dblStress[i][j][k][5]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data.dblStress[i][j][k][l]);
// 			}
// 		}
// 		else
// 		{
// 			if(fabs(Data.dblStress[i][j][k][k]) > fabs(dblStress[i][j][k][k]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data.dblStress[i][j][k][l]);
// 			}
// 		}
// 	}
// }
 
void T_SBCF_D::AbsMax(T_SBCF_D& Data1, T_SBCF_D& Data2)
{
	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
	{
		if(fabs(Data1.dblForce[i][j][k][k]) >= fabs(Data2.dblForce[i][j][k][k]))
			for(l=0; l<9; l++)dblForce[i][j][k][l]=fabs(Data1.dblForce[i][j][k][l]);
		else
			for(l=0; l<9; l++)dblForce[i][j][k][l]=fabs(Data2.dblForce[i][j][k][l]);
	}

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
	{
		// Axial
		int k=0;
		if(fabs(Data1.dblStress[i][j][k][0]) >= fabs(Data2.dblStress[i][j][k][0]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
		}
		// Shear-y
		k=1;
		if(fabs(Data1.dblStress[i][j][k][1]) >= fabs(Data2.dblStress[i][j][k][1]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
		}
		// Shear-z
		k=2;
		if(fabs(Data1.dblStress[i][j][k][2]) >= fabs(Data2.dblStress[i][j][k][2]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
		}
		// Bend +-y
		k=5;
		if(fabs(Data1.dblStress[i][j][k][3]) >= fabs(Data2.dblStress[i][j][k][3]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
		}
		// Bend +-z
		k=4;
		if(fabs(Data1.dblStress[i][j][k][5]) >= fabs(Data2.dblStress[i][j][k][5]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
		}
	}

//     for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
//     {
//       if(fabs(Data1.dblStress[i][j][k][k]) > fabs(Data2.dblStress[i][j][k][k]))
//         for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data1.dblStress[i][j][k][l]);
//       else
//         for(l=0; l<40; l++)dblStress[i][j][k][l]=fabs(Data2.dblStress[i][j][k][l]);
//     }
}
 
// void T_SBCF_D::AbsMaxwithSign(T_SBCF_D& Data)
// {
// 	int i,j,k,l;
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
// 	if(fabs(dblForce[i][j][k][k]) < fabs(Data.dblForce[i][j][k][k]))
// 	{
// 		for(l=0; l<9; l++)dblForce[i][j][k][l]=Data.dblForce[i][j][k][l];
// 	}
// 
// 	for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
// 	{
// 		if(k==5) // Bend +-y
// 		{
// 			if(fabs(dblStress[i][j][k][3]) < fabs(Data.dblStress[i][j][k][3]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
// 			}
// 		}
// 		else if(k==4) // Bend +-z 
// 		{
// 			if(fabs(dblStress[i][j][k][5]) < fabs(Data.dblStress[i][j][k][5]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
// 			}
// 		}
// 		else
// 		{
// 			if(fabs(dblStress[i][j][k][k]) < fabs(Data.dblStress[i][j][k][k]))
// 			{
// 				for(l=0; l<40; l++)dblStress[i][j][k][l]=Data.dblStress[i][j][k][l];
// 			}
// 		}
// 	}
// }
 
void T_SBCF_D::AbsMaxwithSign(T_SBCF_D& Data1, T_SBCF_D& Data2)
{
	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
	{
		if(fabs(Data1.dblForce[i][j][k][k]) >= fabs(Data2.dblForce[i][j][k][k]))
		{
			for(l=0; l<9; l++)dblForce[i][j][k][l]=Data1.dblForce[i][j][k][l];
		}
		else
		{
			for(l=0; l<9; l++)dblForce[i][j][k][l]=Data2.dblForce[i][j][k][l];
		}
	}
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
	{
		// Axial
		 k=0;
		if(fabs(Data1.dblStress[i][j][k][0]) >= fabs(Data2.dblStress[i][j][k][0]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
		}
		// Shear-y
		k=1;
		if(fabs(Data1.dblStress[i][j][k][1]) >= fabs(Data2.dblStress[i][j][k][1]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
		}
		// Shear-z
		k=2;
		if(fabs(Data1.dblStress[i][j][k][2]) >= fabs(Data2.dblStress[i][j][k][2]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
		}
		// Bend +-y
		k=5;
		if(fabs(Data1.dblStress[i][j][k][3]) >= fabs(Data2.dblStress[i][j][k][3]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
		}
		// Bend +-z
		k=4;
		if(fabs(Data1.dblStress[i][j][k][5]) >= fabs(Data2.dblStress[i][j][k][5]))
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
		}
		else
		{
			for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
		}
	}
//     for(int i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
//     {
//       if(fabs(Data1.dblStress[i][j][k][k]) >= fabs(Data2.dblStress[i][j][k][k]))
//       {
//         for(l=0; l<40; l++)dblStress[i][j][k][l]=Data1.dblStress[i][j][k][l];
//       }
//       else
//       {
//         for(l=0; l<40; l++)dblStress[i][j][k][l]=Data2.dblStress[i][j][k][l];
//       }
//     }
}

void T_SBCF_D::Add(T_SBCF_D& Data)
{
#if 1
	unsigned int i, j, k;
	double *pStress = nullptr;
	double *pStressD = nullptr;
	double *pForce = nullptr;
	double *pForceD = nullptr;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
			{
				//for (l = 0; l < 9; l++)
				//	dblForce[i][j][k][l] += Data.dblForce[i][j][k][l];
#if 1
				pForce = &dblForce[i][j][k][0];
				pForceD = &Data.dblForce[i][j][k][0];
				pForce[0] += pForceD[0];
				pForce[1] += pForceD[1];
				pForce[2] += pForceD[2];
				pForce[3] += pForceD[3];
				pForce[4] += pForceD[4];
				pForce[5] += pForceD[5];
				pForce[6] += pForceD[6];
				pForce[7] += pForceD[7];
				pForce[8] += pForceD[8];
#else
				dblForce[i][j][k][0] += Data.dblForce[i][j][k][0];
				dblForce[i][j][k][1] += Data.dblForce[i][j][k][1];
				dblForce[i][j][k][2] += Data.dblForce[i][j][k][2];
				dblForce[i][j][k][3] += Data.dblForce[i][j][k][3];
				dblForce[i][j][k][4] += Data.dblForce[i][j][k][4];
				dblForce[i][j][k][5] += Data.dblForce[i][j][k][5];
				dblForce[i][j][k][6] += Data.dblForce[i][j][k][6];
				dblForce[i][j][k][7] += Data.dblForce[i][j][k][7];
				dblForce[i][j][k][8] += Data.dblForce[i][j][k][8];
#endif

				//for (l = 0; l < 40; l++)
#if 0
				for (l = 0; l < 40; )
				{
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l]; ++l;
				}
				// Combi. 다시 계산.
				double dblStressMax01 = (fabs(dblStress[i][j][k][8]) >= fabs(dblStress[i][j][k][9])) ? dblStress[i][j][k][8] : dblStress[i][j][k][9];
				double dblStressMax02 = (fabs(dblStress[i][j][k][10]) >= fabs(dblStress[i][j][k][11])) ? dblStress[i][j][k][10] : dblStress[i][j][k][11];
				dblStress[i][j][k][7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;
#else	
				pStress = &dblStress[i][j][k][0];
				pStressD = &Data.dblStress[i][j][k][0];
				{
					pStress[0] += pStressD[0];
					pStress[1] += pStressD[1];
					pStress[2] += pStressD[2];
					pStress[3] += pStressD[3];
					pStress[4] += pStressD[4];
					pStress[5] += pStressD[5];
					pStress[6] += pStressD[6];
					pStress[7] += pStressD[7];
					pStress[8] += pStressD[8];
					pStress[9] += pStressD[9];

					pStress[10] += pStressD[10];
					pStress[11] += pStressD[11];
					pStress[12] += pStressD[12];
					pStress[13] += pStressD[13];
					pStress[14] += pStressD[14];
					pStress[15] += pStressD[15];
					pStress[16] += pStressD[16];
					pStress[17] += pStressD[17];
					pStress[18] += pStressD[18];
					pStress[19] += pStressD[19];

					pStress[20] += pStressD[20];
					pStress[21] += pStressD[21];
					pStress[22] += pStressD[22];
					pStress[23] += pStressD[23];
					pStress[24] += pStressD[24];
					pStress[25] += pStressD[25];
					pStress[26] += pStressD[26];
					pStress[27] += pStressD[27];
					pStress[28] += pStressD[28];
					pStress[29] += pStressD[29];

					pStress[30] += pStressD[30];
					pStress[31] += pStressD[31];
					pStress[32] += pStressD[32];
					pStress[33] += pStressD[33];
					pStress[34] += pStressD[34];
					pStress[35] += pStressD[35];
					pStress[36] += pStressD[36];
					pStress[37] += pStressD[37];
					pStress[38] += pStressD[38];
					pStress[39] += pStressD[39];
				}
				// Combi. 다시 계산.
				double dblStressMax01 = (fabs(pStress[8]) >= fabs(pStress[9])) ? pStress[8] : pStress[9];
				double dblStressMax02 = (fabs(pStress[10]) >= fabs(pStress[11])) ? pStress[10] : pStress[11];
				pStress[7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;
#endif
				
			}

	/*
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 9; k++)
			{
				for (l = 0; l < 40; l++)
				{
					dblStress[i][j][k][l] += Data.dblStress[i][j][k][l];
				}
				// Combi. 다시 계산.
				double dblStressMax01 = (fabs(dblStress[i][j][k][8]) >= fabs(dblStress[i][j][k][9])) ? dblStress[i][j][k][8] : dblStress[i][j][k][9];
				double dblStressMax02 = (fabs(dblStress[i][j][k][10]) >= fabs(dblStress[i][j][k][11])) ? dblStress[i][j][k][10] : dblStress[i][j][k][11];
				dblStress[i][j][k][7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;
			}
		}
	}
	*/
#else
	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<9; l++)
		dblForce[i][j][k][l]+=Data.dblForce[i][j][k][l];

	for( i = 0; i<4; i++)
	{
		for(j=0; j<2; j++)
		{
			for(k=0; k<9; k++)
			{
				for(l=0; l<40; l++)
				{
					dblStress[i][j][k][l]+=Data.dblStress[i][j][k][l];
				}
				// Combi. 다시 계산.
				double dblStressMax01 = (fabs(dblStress[i][j][k][8])  >= fabs(dblStress[i][j][k][9]) )? dblStress[i][j][k][8] : dblStress[i][j][k][9];
				double dblStressMax02 = (fabs(dblStress[i][j][k][10]) >= fabs(dblStress[i][j][k][11]))? dblStress[i][j][k][10] : dblStress[i][j][k][11];
				dblStress[i][j][k][7] = (fabs(dblStressMax01)  >= fabs(dblStressMax02) )? dblStressMax01 : dblStressMax02;
			}
		}
	}
#endif
}
void T_SBCF_D::Abs()
{
	int i,j,k,l;
	for(i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<9; l++)
		dblForce[i][j][k][l]=fabs(dblForce[i][j][k][l]);

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<40; l++)
		dblStress[i][j][k][l]=fabs(dblStress[i][j][k][l]);
}
void T_SBCF_D::Sqr()
{
	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<9; l++)
		dblForce[i][j][k][l]=dblForce[i][j][k][l]*dblForce[i][j][k][l];

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<40; l++)
		dblStress[i][j][k][l]=dblStress[i][j][k][l]*dblStress[i][j][k][l];
}
void T_SBCF_D::Root()
{
	int i,j,k,l;
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<9; l++)
		dblForce[i][j][k][l]=sqrt(dblForce[i][j][k][l]);

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<40; l++)
		dblStress[i][j][k][l]=sqrt(dblStress[i][j][k][l]);
}
void T_SBCF_D::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	unsigned int i,j,k;
	double *pForce, *pStress;
#if 1
	// Loop 최적화 
	for (i = 0; i < 4; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
			{
				pForce = &dblForce[i][j][k][0];
				pStress = &dblStress[i][j][k][0];
				//for (l = 0; l < 9; l++)
				//	dblForce[i][j][k][l] *= dFactor;
				//dblForce_ijm = dblForce[i][j][k][0];
				pForce[0] *= dFactor;
				pForce[1] *= dFactor;
				pForce[2] *= dFactor;
				pForce[3] *= dFactor;
				pForce[4] *= dFactor;
				pForce[5] *= dFactor;
				pForce[6] *= dFactor;
				pForce[7] *= dFactor;
				pForce[8] *= dFactor;

				//for (l = 0; l < 40; l++)
				//	dblStress[i][j][k][l] *= dFactor;
				pStress[ 0] *= dFactor;
				pStress[ 1] *= dFactor;
				pStress[ 2] *= dFactor;
				pStress[ 3] *= dFactor;
				pStress[ 4] *= dFactor;
				pStress[ 5] *= dFactor;
				pStress[ 6] *= dFactor;
				pStress[ 7] *= dFactor;
				pStress[ 8] *= dFactor;
				pStress[ 9] *= dFactor;
				pStress[10] *= dFactor;
				pStress[11] *= dFactor;
				pStress[12] *= dFactor;
				pStress[13] *= dFactor;
				pStress[14] *= dFactor;
				pStress[15] *= dFactor;
				pStress[16] *= dFactor;
				pStress[17] *= dFactor;
				pStress[18] *= dFactor;
				pStress[19] *= dFactor;
				pStress[20] *= dFactor;
				pStress[21] *= dFactor;
				pStress[22] *= dFactor;
				pStress[23] *= dFactor;
				pStress[24] *= dFactor;
				pStress[25] *= dFactor;
				pStress[26] *= dFactor;
				pStress[27] *= dFactor;
				pStress[28] *= dFactor;
				pStress[29] *= dFactor;
				pStress[30] *= dFactor;
				pStress[31] *= dFactor;
				pStress[32] *= dFactor;
				pStress[33] *= dFactor;
				pStress[34] *= dFactor;
				pStress[35] *= dFactor;
				pStress[36] *= dFactor;
				pStress[37] *= dFactor;
				pStress[38] *= dFactor;
				pStress[39] *= dFactor;
			}
	/*
	for (i = 0; i < 4; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
				for (l = 0; l < 40; l++)
					dblStress[i][j][k][l] *= dFactor;
					*/
#else
	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<9; l++)
		dblForce[i][j][k][l]*=dFactor;

	for( i = 0; i<4; i++)
		for(j=0; j<2; j++)
			for(k=0; k<9; k++)
				for(l=0; l<40; l++)
		dblStress[i][j][k][l]*=dFactor;
#endif
}
void T_SBCF_D::SetCombinedStress()
{
	// Combined Stress
	unsigned int i,j,k;
	for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
	{
		dblStress[i][j][k][16] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][5];  // -y, +z
		dblStress[i][j][k][18] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][5];  // +y, +z
		dblStress[i][j][k][20] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][6];  // +y, -z
		dblStress[i][j][k][22] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][6];  // -y, -z
		dblStress[i][j][k][17] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+5];  // -y, +z
		dblStress[i][j][k][19] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+5];  // +y, +z
		dblStress[i][j][k][21] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+6];  // +y, -z
		dblStress[i][j][k][23] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+6];  // -y, -z

// 		// SBCF의 조합응력은 이미 계산된 값을 사용하기로 함(CSectInfo::Cal_StrsSbcf)
// 		dblStress[i][j][k][16] = dblStress[i][j][k][8];   // -y, +z
// 		dblStress[i][j][k][18] = dblStress[i][j][k][9];   // +y, +z
// 		dblStress[i][j][k][20] = dblStress[i][j][k][10];  // +y, -z
// 		dblStress[i][j][k][22] = dblStress[i][j][k][11];  // -y, -z
// 		dblStress[i][j][k][17] = dblStress[i][j][k][8];   // -y, +z
// 		dblStress[i][j][k][19] = dblStress[i][j][k][9];   // +y, +z
// 		dblStress[i][j][k][21] = dblStress[i][j][k][10];  // +y, -z
// 		dblStress[i][j][k][23] = dblStress[i][j][k][11];  // -y, -z
	}
}

#if 1
void T_SBCF_D::calcMaximumCombinedStress()
{
	// Combined Stress
	unsigned int i,j,k,l;
	const double dRegardZero = 1.0e-12;
	double cb1(0.0);
	double cb2(0.0);
	double dEqualCompare(0.0);
	double dblStress_18_j(0.0);
	double dblStress_19_j(0.0);
	double fabs_cb1(0.0), fabs_cb2(0.0);
	for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
	{
		//            0    1    2    3     4     5     6     7    8    9    10    11    12    13
		//component : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj, 
		//            Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j
		//dblStress[i][j][k][16] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][5];  // -y, +z
		//dblStress[i][j][k][18] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][5];  // +y, +z
		//dblStress[i][j][k][20] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][6];  // +y, -z
		//dblStress[i][j][k][22] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][6];  // -y, -z
		//dblStress[i][j][k][17] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+5];  // -y, +z
		//dblStress[i][j][k][19] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+5];  // +y, +z
		//dblStress[i][j][k][21] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+6];  // +y, -z
		//dblStress[i][j][k][23] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+6];  // -y, -z
		//const double dRegardZero = 1.0e-12;
		cb1=dblStress[i][j][k][16], cb2=dblStress[i][j][k][17];
		fabs_cb1 = fabs(cb1); fabs_cb2 = fabs(cb2);
		dEqualCompare = 0.0;
		for(l=0; l<6; l+=2)
		{
			// 동일한 값을 가지고 비교할 때 앞에 것이 먼저 나오면 계속해서 앞에 것이 나오도록 수정.
			// Tappered Pipe의 경우 아주 작은 tolerance 차이를 두고 Comb2i와 Comb4i가 번갈아가면서 나와서 Display시에 combined stress 형상이 비뚤어지는 경우 수정 
			dblStress_18_j = dblStress[i][j][k][18+l];
			dblStress_19_j = dblStress[i][j][k][19+l];
			if(fabs_cb1+dRegardZero < fabs(dblStress_18_j))
			{
				if (fabs_cb1 > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_18_j)/fabs_cb1 -1.0;
					if (fabs(dEqualCompare) > dRegardZero)
					{
						cb1 = dblStress_18_j;
						fabs_cb1 = fabs(cb1);
					}
				}
				else
				{
					cb1 = dblStress_18_j;
					fabs_cb1 = fabs(cb1);
				}
			}
			if(fabs_cb2+dRegardZero < fabs(dblStress_19_j))
			{
				if (fabs_cb2 > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_19_j)/fabs_cb2 -1.0;
					if (fabs(dEqualCompare) > dRegardZero)
					{
						cb2 = dblStress_19_j;
						fabs_cb2 = fabs(cb2);
					}
				}
				else
				{
					cb2 = dblStress_19_j;
					fabs_cb2 = fabs(cb2);
				}
			}
		}
		dblStress[i][j][k][14] = cb1;
		dblStress[i][j][k][15] = cb2;

// 		// SBCF의 조합응력은 이미 계산된 값을 사용하기로 함(CSectInfo::Cal_StrsSbcf)
// 		dblStress[i][j][k][14] = dblStress[i][j][k][7];
// 		dblStress[i][j][k][15] = dblStress[i][j][k][7];
	}
}
#else
void T_SBCF_D::calcMaximumCombinedStress()
{
	// Combined Stress
	int i,j,k,l;
	for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<9; k++)
	{
		//            0    1    2    3     4     5     6     7    8    9    10    11    12    13
		//component : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj, 
		//            Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j
		//dblStress[i][j][k][16] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][5];  // -y, +z
		//dblStress[i][j][k][18] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][5];  // +y, +z
		//dblStress[i][j][k][20] = dblStress[i][j][k][0]+dblStress[i][j][k][3]+dblStress[i][j][k][6];  // +y, -z
		//dblStress[i][j][k][22] = dblStress[i][j][k][0]+dblStress[i][j][k][4]+dblStress[i][j][k][6];  // -y, -z
		//dblStress[i][j][k][17] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+5];  // -y, +z
		//dblStress[i][j][k][19] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+5];  // +y, +z
		//dblStress[i][j][k][21] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+3]+dblStress[i][j][k][7+6];  // +y, -z
		//dblStress[i][j][k][23] = dblStress[i][j][k][7+0]+dblStress[i][j][k][7+4]+dblStress[i][j][k][7+6];  // -y, -z
		const double dRegardZero = 1.0e-12;
		double cb1=dblStress[i][j][k][16], cb2=dblStress[i][j][k][17];
		double dEqualCompare = 0.0;
		for(l=0; l<6; l+=2)
		{
			// 동일한 값을 가지고 비교할 때 앞에 것이 먼저 나오면 계속해서 앞에 것이 나오도록 수정.
			// Tappered Pipe의 경우 아주 작은 tolerance 차이를 두고 Comb2i와 Comb4i가 번갈아가면서 나와서 Display시에 combined stress 형상이 비뚤어지는 경우 수정 
			double dblStress_18_j = dblStress[i][j][k][18+l];
			double dblStress_19_j = dblStress[i][j][k][19+l];
			if(fabs(cb1)+dRegardZero < fabs(dblStress_18_j))
			{
				if (fabs(cb1) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_18_j)/fabs(cb1) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb1=dblStress_18_j;
				}
				else   cb1=dblStress_18_j;
			}
			if(fabs(cb2)+dRegardZero < fabs(dblStress_19_j))
			{
				if (fabs(cb2) > dRegardZero)
				{
					dEqualCompare = fabs(dblStress_19_j)/fabs(cb2) -1.0;
					if (fabs(dEqualCompare) > dRegardZero)  cb2=dblStress_19_j;
				}
				else   cb2=dblStress_19_j;
			}
		}
		dblStress[i][j][k][14] = cb1;
		dblStress[i][j][k][15] = cb2;

// 		// SBCF의 조합응력은 이미 계산된 값을 사용하기로 함(CSectInfo::Cal_StrsSbcf)
// 		dblStress[i][j][k][14] = dblStress[i][j][k][7];
// 		dblStress[i][j][k][15] = dblStress[i][j][k][7];
	}
}
#endif
void T_SBCF_D::ConvertToSbcf(T_STRB_D& DataStrb)
{
	Initialize();
	int i,j,k;
	for(i = 0; i<4; i++)
	for(j=0; j<2; j++)
	{
		for(k=0; k<6; k++)dblForce[i][j][0][k]=DataStrb.dblForce[i][j*6+k];
		for(k=0; k<3; k++)dblForce[i][j][0][k+6]=DataStrb.dblForce[i][12+j+k*2];
		for(k=1; k<9; k++)memcpy(dblForce[i][j][k], dblForce[i][j][0], sizeof(dblForce[i][j][0]));
	}

	for(i = 0; i<4; i++)
	for(j=0; j<2; j++)
	{
		for(k=0; k<7;  k++)dblStress[i][j][0][   k]=DataStrb.dblStress[i][j*7+k];
		for(k=0; k<5;  k++)dblStress[i][j][0][7 +k]=DataStrb.dblStress[i][14+j+k*2];
		for(k=0; k<20; k++)dblStress[i][j][0][12+k]=DataStrb.dblStress[i][24+j*20+k];
		for(k=0; k<8;  k++)dblStress[i][j][0][32+k]=DataStrb.dblStress[i][64+j+k*2];
		for(k=1; k<9; k++)memcpy(dblStress[i][j][k], dblStress[i][j][0], sizeof(dblStress[i][j][0]));
	}
}
void T_SBCF_D::ConvertToStrb(T_STRB_D& StrbD,BOOL bMax)  // Force 만 사용
{
	StrbD.Initialize(); 
	int i,j,k;
		
	for(i = 0; i<4; i++)
	for(j=0; j<2; j++)
	{
		// 대각값이 최대값
		for(k=0; k<6; k++) StrbD.dblForce[i][j*6+k] = dblForce[i][j][k][k];
		for(k=0; k<3; k++) StrbD.dblForce[i][12+j+k*2] = dblForce[i][j][k][k];
	}
}

void T_SBCF_D::ConvertToStrbMxMn(T_SBCF_D& SbcfMxD,T_SBCF_D& SbcfMnD,T_STRB_D& StrbMxD,T_STRB_D &StrbMnD)
{
	StrbMxD.Initialize(); 
	StrbMnD.Initialize();

	int i,j,k,l;
	double dMnValue,dMxValue;
		
	// Force
	for(i = 0; i<4; i++)for(j=0; j<2; j++)
	{
		for(k=0; k<6; k++)
		{
			StrbMnD.dblForce[i][j*6+k] = __min(SbcfMnD.dblForce[i][j][k][k], SbcfMxD.dblForce[i][j][k][k]);
			StrbMxD.dblForce[i][j*6+k] = __max(SbcfMnD.dblForce[i][j][k][k], SbcfMxD.dblForce[i][j][k][k]);
		}
		for(k=0; k<3; k++)
		{
			StrbMnD.dblForce[i][12+j+k*2] = __min(SbcfMnD.dblForce[i][j][k][k], SbcfMxD.dblForce[i][j][k][k]);
			StrbMxD.dblForce[i][12+j+k*2] = __max(SbcfMnD.dblForce[i][j][k][k], SbcfMxD.dblForce[i][j][k][k]);
		}
	}
		
	// Stress
	// SBCF : Sx , Sy, Sz, Byp, Bym, Bzp, Bzm, Combi, Comb1, Comb2, Comb3, Comb4     
	// STRB : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj,
	//        Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j

	for(i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<40; k++)
	{
		dMnValue = SbcfMxD.dblStress[i][j][0][k];
		dMxValue = SbcfMxD.dblStress[i][j][0][k];
		for(l=0; l<9; l++)
		{
			dMnValue = min(SbcfMxD.dblStress[i][j][l][k],dMnValue);
			dMxValue = max(SbcfMxD.dblStress[i][j][l][k],dMxValue);
		}
		for(l=0; l<9; l++)
		{
			dMnValue = min(SbcfMnD.dblStress[i][j][l][k],dMnValue);
			dMxValue = max(SbcfMnD.dblStress[i][j][l][k],dMxValue);
		}
		if(k < 7)
		{
			StrbMnD.dblStress[i][j*7+k] = dMnValue;
			StrbMxD.dblStress[i][j*7+k] = dMxValue;
		}
		else if(k < 12)
		{
			StrbMnD.dblStress[i][14+(k-7)*2+j] = dMnValue;
			StrbMxD.dblStress[i][14+(k-7)*2+j] = dMxValue;
		}
		else if(k < 32)
		{
			StrbMnD.dblStress[i][12+j*20+k] = dMnValue;
			StrbMxD.dblStress[i][12+j*20+k] = dMxValue;
		}
		else
		{
			StrbMnD.dblStress[i][64+(k-32)*2+j] = dMnValue;
			StrbMxD.dblStress[i][64+(k-32)*2+j] = dMxValue;
		}
	}
}

void T_CJFF_D::Max(T_CJFF_D& Data)
{
	int matrixSize = nDof * nElem;

	// 대각 원소 비교 및 행 복사
	for (int i = 0; i < matrixSize; ++i) 
	{
		double diagA = Data.adForce[i * matrixSize + i];
		double diagB =      adForce[i * matrixSize + i];

		if (diagA >= diagB) 
		{
			for (int j = 0; j < matrixSize; ++j) 
			{
			   adForce[i * matrixSize + j] = Data.adForce[i * matrixSize + j];
			}
		}
	}
}
void T_CJFF_D::Min(T_CJFF_D& Data)
{
	int matrixSize = nDof * nElem;

	// 대각 원소 비교 및 행 복사
	for (int i = 0; i < matrixSize; ++i)
	{
		double diagA = Data.adForce[i * matrixSize + i];
		double diagB =      adForce[i * matrixSize + i];

		if (diagA < diagB)
		{
			for (int j = 0; j < matrixSize; ++j)
			{
				adForce[i * matrixSize + j] = Data.adForce[i * matrixSize + j];
			}
		}
	}
}
void T_CJFF_D::AbsMax(T_CJFF_D& Data1, T_CJFF_D& Data2)
{
	int matrixSize = nDof * nElem;

	// 대각 원소 비교 및 행 복사
	for (int i = 0; i < matrixSize; ++i)
	{
		double diagA = Data1.adForce[i * matrixSize + i];
		double diagB = Data2.adForce[i * matrixSize + i];

		if (fabs(diagA) >= fabs(diagB))
		{
			for (int j = 0; j < matrixSize; ++j)
				adForce[i * matrixSize + j] = fabs(Data1.adForce[i * matrixSize + j]);
		}
		else
		{
			for (int j = 0; j < matrixSize; ++j)
				adForce[i * matrixSize + j] = fabs(Data2.adForce[i * matrixSize + j]);
		}
	}
}
void T_CJFF_D::AbsMaxwithSign(T_CJFF_D& Data1, T_CJFF_D& Data2)
{
	int matrixSize = nDof * nElem;

	// 대각 원소 비교 및 행 복사
	for (int i = 0; i < matrixSize; ++i)
	{
		double diagA = Data1.adForce[i * matrixSize + i];
		double diagB = Data2.adForce[i * matrixSize + i];

		if (fabs(diagA) >= fabs(diagB))
		{
			for (int j = 0; j < matrixSize; ++j)
				adForce[i * matrixSize + j] = Data1.adForce[i * matrixSize + j];
		}
		else
		{
			for (int j = 0; j < matrixSize; ++j)
				adForce[i * matrixSize + j] = Data2.adForce[i * matrixSize + j];
		}
	}
}
void T_CJFF_D::Add(T_CJFF_D& Data)
{
	for (int i = 0; i < nSize; i++)adForce[i] += Data.adForce[i];
}
void T_CJFF_D::Abs()
{
	for (int i = 0; i < nSize; i++)adForce[i] = fabs(adForce[i]);
}
void T_CJFF_D::Sqr()
{
	for (int i = 0; i < nSize; i++)adForce[i] = adForce[i] * adForce[i];
}
void T_CJFF_D::Root()
{
	for (int i = 0; i < nSize; i++)adForce[i] = sqrt(adForce[i]);
}
void T_CJFF_D::Mult(double dFactor)
{
	if (dFactor == 1.)return;
	for (int i = 0; i < nSize; i++)adForce[i] *= dFactor;
}


/////////////////////////////////////////
// T_SPCF_DL
void T_SPCF_DL::Max(T_SPCF_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(Data.dForce[i][j][j] >= dForce[i][j][j])
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = Data.dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::Min(T_SPCF_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(Data.dForce[i][j][j] < dForce[i][j][j])
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = Data.dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::AbsMax(T_SPCF_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(fabs(Data.dForce[i][j][j]) > fabs(dForce[i][j][j]))
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = fabs(Data.dForce[i][j][k]);
			}
		}
	}
}

void T_SPCF_DL::AbsMax(T_SPCF_DL& Data1, T_SPCF_DL& Data2)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(fabs(Data1.dForce[i][j][j]) >= fabs(Data2.dForce[i][j][j]))
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = fabs(Data1.dForce[i][j][k]);
			}
			else
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = fabs(Data2.dForce[i][j][k]);
			}
		}
	}
}

void T_SPCF_DL::AbsMaxwithSign(T_SPCF_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(fabs(Data.dForce[i][j][j]) > fabs(dForce[i][j][j]))
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = Data.dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::AbsMaxwithSign(T_SPCF_DL& Data1, T_SPCF_DL& Data2)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(fabs(Data1.dForce[i][j][j]) >= fabs(Data2.dForce[i][j][j]))
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = Data1.dForce[i][j][k];
			}
			else
			{
				for(int k=0; k<8; k++) dForce[i][j][k] = Data2.dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::Add(T_SPCF_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<8; k++)
			{
				dForce[i][j][k] += Data.dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::Abs()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<8; k++)
			{
				dForce[i][j][k] = fabs(dForce[i][j][k]);
			}
		}
	}
}

void T_SPCF_DL::Sqr()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<8; k++)
			{
				dForce[i][j][k] = dForce[i][j][k] * dForce[i][j][k];
			}
		}
	}
}

void T_SPCF_DL::Root()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<8; k++)
			{
				dForce[i][j][k] = sqrt(dForce[i][j][k]);
			}
		}
	}
}

void T_SPCF_DL::Mult(double dFactor)
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<8; k++)
			{
				dForce[i][j][k] *= dFactor;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// T_SIMC_D
void T_SIMC_D::Max(T_SIMC_D& Data)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
		if (Data.dblForce[i][j][j] >= dblForce[i][j][j])
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = Data.dblForce[i][j][k];
		}
}

void T_SIMC_D::Min(T_SIMC_D& Data)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
		if (Data.dblForce[i][j][j] < dblForce[i][j][j])
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = Data.dblForce[i][j][k];
		}
}

void T_SIMC_D::AbsMax(T_SIMC_D& Data)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
		if (fabs(Data.dblForce[i][j][j]) > fabs(dblForce[i][j][j]))
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = fabs(Data.dblForce[i][j][k]);
		}
}

void T_SIMC_D::AbsMax(T_SIMC_D& Data1, T_SIMC_D& Data2)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
	{
		if (fabs(Data1.dblForce[i][j][j]) >= fabs(Data2.dblForce[i][j][j]))
			for (k = 0; k < 6; k++)dblForce[i][j][k] = fabs(Data1.dblForce[i][j][k]);
		else
			for (k = 0; k < 6; k++)dblForce[i][j][k] = fabs(Data2.dblForce[i][j][k]);
	}
}

void T_SIMC_D::AbsMaxwithSign(T_SIMC_D& Data)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
		if (fabs(dblForce[i][j][j]) < fabs(Data.dblForce[i][j][j]))
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = Data.dblForce[i][j][k];
		}
}

void T_SIMC_D::AbsMaxwithSign(T_SIMC_D& Data1, T_SIMC_D& Data2)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)
	{
		if (fabs(Data1.dblForce[i][j][j]) >= fabs(Data2.dblForce[i][j][j]))
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = Data1.dblForce[i][j][k];
		}
		else
		{
			for (k = 0; k < 6; k++)dblForce[i][j][k] = Data2.dblForce[i][j][k];
		}
	}
}

void T_SIMC_D::Add(T_SIMC_D& Data)
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)for (k = 0; k < 6; k++)
		dblForce[i][j][k] += Data.dblForce[i][j][k];
}

void T_SIMC_D::Abs()
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)for (k = 0; k < 6; k++)
		dblForce[i][j][k] = fabs(dblForce[i][j][k]);
}

void T_SIMC_D::Sqr()
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)for (k = 0; k < 6; k++)
		dblForce[i][j][k] = dblForce[i][j][k] * dblForce[i][j][k];
}

void T_SIMC_D::Root()
{
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)for (k = 0; k < 6; k++)
		dblForce[i][j][k] = sqrt(dblForce[i][j][k]);
}

void T_SIMC_D::Mult(double dFactor)
{
	if (dFactor == 1.)return;
	int i, j, k;
	for (i = 0; i < 2; i++)for (j = 0; j < 6; j++)for (k = 0; k < 6; k++)
		dblForce[i][j][k] *= dFactor;
}

void T_SIMC_D::ConvertToSimc(T_SIMB_D& DataSimb)
{
	Initialize();
	int i, j;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 6; j++)dblForce[i][0][j] = DataSimb.dblForce[i * 6 + j];
		for (j = 1; j < 6; j++)memcpy(dblForce[i][j], dblForce[i][0], sizeof(dblForce[i][0]));
	}
}

void T_SIMC_D::ConvertToSimb(T_SIMB_D& SimbD, BOOL bMax)  // Force 만 사용
{
	SimbD.Initialize();
	int i, j;

	for (i = 0; i < 2; i++)
	{
		// 대각값이 최대값
		for (j = 0; j < 6; j++) SimbD.dblForce[i * 6 + j] = dblForce[i][j][j];
	}
}

void T_SIMC_D::ConvertToSimbMxMn(T_SIMC_D& SimcMxD, T_SIMC_D& SimcMnD, T_SIMB_D& SimbMxD, T_SIMB_D& SimbMnD)
{
	SimbMxD.Initialize();
	SimbMnD.Initialize();

	int i, j;
	// Force
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 6; j++)
		{
			SimbMnD.dblForce[i * 6 + j] = __min(SimcMnD.dblForce[i][j][j], SimcMxD.dblForce[i][j][j]);
			SimbMxD.dblForce[i * 6 + j] = __max(SimcMnD.dblForce[i][j][j], SimcMxD.dblForce[i][j][j]);
		}
	}
}

void T_SIMC_D::ConvertToSbcf(T_SBCF_D& SbcfD)
{
	//SbcfD.dblForce[4][2][9][9];      // 4 part / i,j /[9]  Fx, Fy, Fz, Mx, My, Mz, Bi, St, Wt

	// 1) 초기화
	SbcfD.Initialize();

	// 2) 경계 복사
	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 6; ++j)
		{
			SbcfD.dblForce[0][0][i][j] = dblForce[0][i][j]; // start
			SbcfD.dblForce[3][1][i][j] = dblForce[1][i][j]; // end
		}

	// 3) 나머지 전부 선형보간: t = (k + a) / 4.0
	//    (0,0) -> t=0  /  (3,1) -> t=1
	for (int k = 0; k < 4; ++k)
	{
		for (int a = 0; a < 2; ++a)
		{
			// 이미 경계에 정확히 맞춘 위치는 스킵
			if ((k == 0 && a == 0) || (k == 3 && a == 1))
				continue;

			const double t = (static_cast<double>(k) + static_cast<double>(a)) / 4.0;

			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					const double s = dblForce[0][i][j]; // start
					const double e = dblForce[1][i][j]; // end
					SbcfD.dblForce[k][a][i][j] = (1.0 - t) * s + t * e;
				}
			}
		}
	}

}



/////////////////////////////////////////
// T_SBCS_D
void T_SBCS_D::Max(T_SBCS_D& Data)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		if(Data.dblForce[m][i][j][6] > dblForce[m][i][j][6])
		{
			for(k=0; k<10; k++)dblForce[m][i][j][k]=Data.dblForce[m][i][j][k];
		}
	}
}
void T_SBCS_D::Min(T_SBCS_D& Data)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		if(Data.dblForce[m][i][j][6] < dblForce[m][i][j][6])
		{
			for(k=0; k<10; k++)dblForce[m][i][j][k]=Data.dblForce[m][i][j][k];
		}
	}
}
void T_SBCS_D::AbsMax(T_SBCS_D& Data)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		if(fabs(Data.dblForce[m][i][j][6]) > fabs(dblForce[m][i][j][6]))
		{
			for(k=0; k<10; k++)dblForce[m][i][j][k]=fabs(Data.dblForce[m][i][j][k]);
		}
	}
}
void T_SBCS_D::AbsMax(T_SBCS_D& Data1, T_SBCS_D& Data2)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		{
			if(fabs(Data1.dblForce[m][i][j][6]) > fabs(Data2.dblForce[m][i][j][6]))
				for(k=0; k<10; k++)dblForce[m][i][j][k]=fabs(Data1.dblForce[m][i][j][k]);
			else
				for(k=0; k<10; k++)dblForce[m][i][j][k]=fabs(Data2.dblForce[m][i][j][k]);
		}
	}
}
void T_SBCS_D::AbsMaxwithSign(T_SBCS_D& Data)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		if(fabs(dblForce[m][i][j][6]) < fabs(Data.dblForce[m][i][j][6]))
		{
			for(k=0; k<10; k++)dblForce[m][i][j][k]=Data.dblForce[m][i][j][k];
		}
	}
}
void T_SBCS_D::AbsMaxwithSign(T_SBCS_D& Data1, T_SBCS_D& Data2)
{
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		{
			if(fabs(Data1.dblForce[m][i][j][6]) >= fabs(Data2.dblForce[m][i][j][6]))
				for(k=0; k<10; k++)dblForce[m][i][j][k]=Data1.dblForce[m][i][j][k];
			else
				for(k=0; k<10; k++)dblForce[m][i][j][k]=Data2.dblForce[m][i][j][k];
		}
	}
}
void T_SBCS_D::Add(T_SBCS_D& Data)
{
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<10; k++)
		dblForce[m][i][j][k]+=Data.dblForce[m][i][j][k];
}
void T_SBCS_D::Abs()
{
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<10; k++)
		dblForce[m][i][j][k]=fabs(dblForce[m][i][j][k]);
}
void T_SBCS_D::Sqr()
{
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<10; k++)
		dblForce[m][i][j][k]=dblForce[m][i][j][k]*dblForce[m][i][j][k];
}
void T_SBCS_D::Root()
{
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<10; k++)
		dblForce[m][i][j][k]=sqrt(dblForce[m][i][j][k]);
}
void T_SBCS_D::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)for(j=0; j<2; j++)for(k=0; k<10; k++)
		dblForce[m][i][j][k]*=dFactor;
}
void T_SBCS_D::ConvertToSbcs(T_STRB_D& DataStrb)
{
	Initialize();
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)
		{
			for(j=0; j<2; j++)for(k=0; k<6; k++)
				dblForce[m][i][j][k]=DataStrb.dblForce[m][j*6+k];

			dblForce[m][i][0][6]=DataStrb.dblStress[m][16+2*i];      // Combined Stress i-end
			dblForce[m][i][1][6]=DataStrb.dblStress[m][17+2*i];      // Combined Stress j-end

			for(k=0; k<3; k++)
			{
				dblForce[m][i][0][7+k]=DataStrb.dblForce[m][12+k*2];
				dblForce[m][i][1][7+k]=DataStrb.dblForce[m][12+k*2+1];
			}
		}
	}
}

/////////////////////////////////////////
// T_BCSF_D
void T_BCSF_D::ConvertToBcsfMax(T_SBCS_D& Data)
{
	Initialize();
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)
	{
		if(i==0)
		{
			for(j=0; j<2; j++)for(k=0; k<10; k++)
				dblForce[m][j][k]=Data.dblForce[m][i][j][k];				
		}
		else
		{
			for(j=0; j<2; j++)
			if(Data.dblForce[m][i][j][6] > dblForce[m][j][6])
			{
				for(k=0; k<10; k++)dblForce[m][j][k]=Data.dblForce[m][i][j][k];
			}
		}
	}
}
void T_BCSF_D::ConvertToBcsfMin(T_SBCS_D& Data)
{
	Initialize();
	int i,j,k,m;
	for(m=0; m<4; m++)for( i = 0; i<4; i++)
	{
		if(i==0)
		{
			for(j=0; j<2; j++)for(k=0; k<10; k++)
				dblForce[m][j][k]=Data.dblForce[m][i][j][k];				
		}
		else
		{
			for(j=0; j<2; j++)
			if(Data.dblForce[m][i][j][6] < dblForce[m][j][6])
			{
				for(k=0; k<10; k++)dblForce[m][j][k]=Data.dblForce[m][i][j][k];
			}
		}
	}
}
void T_BCSF_D::ConvertToBcsfAbsMax(T_SBCS_D& Data1, T_SBCS_D& Data2)
{
	Initialize();
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		{
			if(fabs(Data1.dblForce[m][i][j][6]) > fabs(Data2.dblForce[m][i][j][6]))
				for(k=0; k<10; k++)dblForce[m][j][k]=fabs(Data1.dblForce[m][i][j][k]);
			else
				for(k=0; k<10; k++)dblForce[m][j][k]=fabs(Data2.dblForce[m][i][j][k]);
		}
	}
}
void T_BCSF_D::ConvertToBcsfAbsMaxwithSign(T_SBCS_D& Data1, T_SBCS_D& Data2)
{
	Initialize();
	int i,j,k,m;
	for(m=0; m<4; m++)
	{
		for( i = 0; i<4; i++)for(j=0; j<2; j++)
		{
			if(fabs(Data1.dblForce[m][i][j][6]) >= fabs(Data2.dblForce[m][i][j][6]))
				for(k=0; k<10; k++)dblForce[m][j][k]=Data1.dblForce[m][i][j][k];
			else
				for(k=0; k<10; k++)dblForce[m][j][k]=Data2.dblForce[m][i][j][k];
		}
	}
}

/////////////////////////////////////////
// T_STRM_DL
// plane stress
void T_STRM_DL::Max(T_STRM_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
				if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5 && Data.dblStress[i][k] > dblStress[i][k])
			{
				dblStress[i][k]=Data.dblStress[i][k];
				if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
			}
		}
	}
}
void T_STRM_DL::Min(T_STRM_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
				if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5 && Data.dblStress[i][k] < dblStress[i][k])
			{
				dblStress[i][k]=Data.dblStress[i][k];
				if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
			}
		}
	}
}
void T_STRM_DL::AbsMax(T_STRM_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)
			{
				if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
				{
					dblStress[i][k]=fabs(Data.dblStress[i][k]);
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
				}
				else
				{
					dblStress[i][k]=fabs(dblStress[i][k]);
					if(k ==  3)dblStress[i][ 5]=dblStress[i][ 5];
				}
			}
		}
	}
}
void T_STRM_DL::AbsMax(T_STRM_DL& Data1, T_STRM_DL& Data2)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)
			{
				if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
				{
					dblStress[i][k]=fabs(Data2.dblStress[i][k]);
					if(k ==  3)dblStress[i][ 5]=Data2.dblStress[i][ 5];
				}
				else
				{
					dblStress[i][k]=fabs(Data1.dblStress[i][k]);
					if(k ==  3)dblStress[i][ 5]=Data1.dblStress[i][ 5];
				}
			}
		}
	}
}
void T_STRM_DL::AbsMaxwithSign(T_STRM_DL& Data, double dZero/*=1.e-10*/) // fixed by mylee
{
	double a, b;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
			{
				a = dblForce[i][j]; b = Data.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
				// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
			}
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)
			{
				a = dblStress[i][k]; b = Data.dblStress[i][k];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
				// if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
				}
				/*
				else
				{
					dblStress[i][k]=dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=dblStress[i][ 5];
				}
				*/
			}
		}
	}
}
void T_STRM_DL::AbsMaxwithSign(T_STRM_DL& Data1, T_STRM_DL& Data2, double dZero/*=1.e-10*/) // fixed by mylee
{
	double a, b;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
			{
				a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				//dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
			}
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)
			{
				// if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
				a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
				{
					dblStress[i][k]=Data2.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data2.dblStress[i][ 5];
				}
				else
				{
					dblStress[i][k]=Data1.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data1.dblStress[i][ 5];
				}
			}
		}
	}
}
void T_STRM_DL::Add(T_STRM_DL& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)dblStress[i][k]+=Data.dblStress[i][k];
		}
		dblStress[i][ 5]=0.;
	}
}
void T_STRM_DL::Abs()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=fabs(dblForce[i][j]);
		}
		for(int k=0; k<8; k++)dblStress[i][k]=fabs(dblStress[i][k]);
	}
}
void T_STRM_DL::Sqr()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
		}
		dblStress[i][ 5]=0.;
	}
}
void T_STRM_DL::Root()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)dblStress[i][k]=sqrt(dblStress[i][k]);
		}
		dblStress[i][ 5]=0.;
	}
}
void T_STRM_DL::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]*=dFactor;
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)dblStress[i][k]*=dFactor;
		}
	}
}
void T_STRM_DL::Multiple(T_STRM_DL Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]*=Data.dblForce[i][j];
		}
		for(int k=0; k<8; k++)
		{
			if(k != 5)dblStress[i][k]*=Data.dblStress[i][k];
		}
	}
}
void T_STRM_DL::calcPrincipalStress()
{
	double cc, bb, cr;
	double sigma[4];
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<4; j++)sigma[j]=0.;
		if(dblStress[i][0] == 0. && dblStress[i][1] == 0. && dblStress[i][2] == 0.)goto END;
		cc = (dblStress[i][0]+dblStress[i][1])/2.;
		bb = (dblStress[i][0]-dblStress[i][1])/2.;
		cr = sqrt(pow(bb,2)+pow(dblStress[i][2],2));
		sigma[0] = cc+cr;
		sigma[1] = cc-cr;
		if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][2]) <= 1.e-10)sigma[2] = 0.;
		else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
		{
			if(dblStress[i][2] >= 0.)sigma[2]=45.;
			if(dblStress[i][2] < 0.)sigma[2]=-45.;
		}
		else
			sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][2],bb);
		sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
		dblStress[i][3]=sigma[0];
		dblStress[i][4]=sigma[1];
		dblStress[i][5]=sigma[2];
		dblStress[i][6]=sigma[3];

		//MaxShear
		dblStress[i][7] = __max(__max( fabs(dblStress[i][3]-dblStress[i][4])/2., fabs(dblStress[i][3])/2. ), fabs(dblStress[i][4])/2. ) ;
	}
}

/////////////////////////////////////////
// T_STRM_DG
void T_STRM_DG::Max(T_STRM_DG& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
				if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8 && Data.dblStress[i][k] > dblStress[i][k])
			{
				dblStress[i][k]=Data.dblStress[i][k];
				if(k == 6)dblStress[i][ 8]=Data.dblStress[i][ 8];
			}
		}
	}
}
void T_STRM_DG::Min(T_STRM_DG& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
				if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8 && Data.dblStress[i][k] < dblStress[i][k])
			{
				dblStress[i][k]=Data.dblStress[i][k];
				if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
			}
		}
	}
}
void T_STRM_DG::AbsMax(T_STRM_DG& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)
			{
				if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
				{
					dblStress[i][k]=fabs(Data.dblStress[i][k]);
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
				}
				else
				{
					dblStress[i][k]=fabs(dblStress[i][k]);
					if(k ==  6)dblStress[i][ 8]=dblStress[i][ 8];
				}
			}
		}
	}
}
void T_STRM_DG::AbsMax(T_STRM_DG& Data1, T_STRM_DG& Data2)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)
			{
				if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
				{
					dblStress[i][k]=fabs(Data2.dblStress[i][k]);
					if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
				}
				else
				{
					dblStress[i][k]=fabs(Data1.dblStress[i][k]);
					if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
				}
			}
		}
	}
}
void T_STRM_DG::AbsMaxwithSign(T_STRM_DG& Data, double dZero/*=1.e-10*/) // fixed by mylee
{
	double a, b;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
			{
				a = dblForce[i][j]; b = Data.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
				// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
			}
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)
			{
				// if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
				a = dblStress[i][k]; b = Data.dblStress[i][k];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
				}
				/*
				else
				{
					dblStress[i][k]=dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=dblStress[i][ 8];
				}
				*/
			}
		}
	}
}
void T_STRM_DG::AbsMaxwithSign(T_STRM_DG& Data1, T_STRM_DG& Data2, double dZero/*=1.e-10*/) // fixed by mylee
{
	double a, b;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)
			{
				a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				//dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
			}
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)
			{
				// if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
				a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
				{
					dblStress[i][k]=Data2.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
				}
				else
				{
					dblStress[i][k]=Data1.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
				}
			}
		}
	}
}
void T_STRM_DG::Add(T_STRM_DG& Data)
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)dblStress[i][k]+=Data.dblStress[i][k];
		}
		dblStress[i][ 8]=0.;
	}
}
void T_STRM_DG::Abs()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=fabs(dblForce[i][j]);
		}
		for(int k=0; k<11; k++)dblStress[i][k]=fabs(dblStress[i][k]);
	}
}
void T_STRM_DG::Sqr()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
		}
		dblStress[i][ 8]=0.;
	}
}
void T_STRM_DG::Root()
{
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)dblStress[i][k]=sqrt(dblStress[i][k]);
		}
		dblStress[i][ 8]=0.;
	}
}
void T_STRM_DG::Mult(double dFactor)
{
	if(dFactor == 1.)return;
	for(int i=0; i<5; i++)
	{
		if(i < 4)
		{
			for(int j=0; j<3; j++)dblForce[i][j]*=dFactor;
		}
		for(int k=0; k<11; k++)
		{
			if(k != 8)dblStress[i][k]*=dFactor;
		}
	}
}
void T_STRM_DG::calcPrincipalStress()
{
	double cc, bb, cr;
	double sigma[4];
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<4; j++)sigma[j]=0.;
		if(dblStress[i][0] == 0. && dblStress[i][1] == 0. && dblStress[i][3] == 0.)goto END;
		cc = (dblStress[i][0]+dblStress[i][1])/2.;
		bb = (dblStress[i][0]-dblStress[i][1])/2.;
		cr = sqrt(pow(bb,2)+pow(dblStress[i][3],2));
		sigma[0] = cc+cr;
		sigma[1] = cc-cr;
		if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][3]) <= 1.e-10)sigma[2] = 0.;
		else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
		{
			if(dblStress[i][3] >= 0.)sigma[2]=45.;
			if(dblStress[i][3] < 0.)sigma[2]=-45.;
		}
		else
			sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][3],bb);
		sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
		dblStress[i][6]=sigma[0];
		dblStress[i][7]=sigma[1];
		dblStress[i][8]=sigma[2];
		dblStress[i][9]=sigma[3];
			
		//MaxShear
		dblStress[i][10] = __max(__max( fabs(dblStress[i][6]-dblStress[i][7])/2., fabs(dblStress[i][6])/2. ), fabs(dblStress[i][7])/2. ) ;
	}
}

