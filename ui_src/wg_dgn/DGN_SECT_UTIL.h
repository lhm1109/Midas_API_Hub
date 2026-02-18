
#ifndef __DGN_SECT_UTIL_H__
#define __DGN_SECT_UTIL_H__
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int nStiffenerSizeIndex = 8;

// About Steel Section's Stiffener
struct T_DGN_STL_STIFFENER_UNIT 
{
	CString strName;	// Stiffener Name
	bool bCalc;			// Include Stiffness?
	int nType;			// 0 : Flat,  1 : Tee, 2 : U-RIB
	int nPosition;		// 0:Both 1:Left 2:Right
	double dDist;
	double dSpace;
	double dSpace_L;	// nearby space left
	double dSpace_R;	// nearby space right
	double dArea;
	double dIyy;
	double dCzp;
	double dCzm;
	double dIzz;
	double dCyp;
	double dCym;
	double dSize[nStiffenerSizeIndex];

	T_DGN_STL_STIFFENER_UNIT()
	{
		Initialize();
	}

	void Initialize()
	{
		strName.Empty();
		bCalc		= true;
		nType		= 0;
		nPosition	= 0;
		dDist		= 0.0;
		dSpace		= 0.0;
		dSpace_L	= 0.0;
		dSpace_R	= 0.0;
		dArea		= 0.0;
		dIyy		= 0.0;
		dCzp		= 0.0;
		dCzm		= 0.0;
		dIzz		= 0.0;
		dCyp		= 0.0;
		dCym		= 0.0;

		for(int i = 0; i < nStiffenerSizeIndex; ++i){dSize[i] = 0.0;}
	}

	T_DGN_STL_STIFFENER_UNIT& operator =(const T_DGN_STL_STIFFENER_UNIT& crD)
	{
		strName		= crD.strName;
		bCalc		= crD.bCalc;
		nType		= crD.nType	 ;
		nPosition	= crD.nPosition;
		dDist		= crD.dDist	 ;
		dSpace		= crD.dSpace ;	
		dSpace_L	= crD.dSpace_L ;	
		dSpace_R	= crD.dSpace_R ;	
		dArea		= crD.dArea	 ;
		dIyy		= crD.dIyy	 ;
		dCzp		= crD.dCzp;
		dCzm		= crD.dCzm;
		dIzz		= crD.dIzz;
		dCyp		= crD.dCyp;
		dCym		= crD.dCym;

		for(int i = 0; i < nStiffenerSizeIndex; ++i){dSize[i] = crD.dSize[i];}

		return *this;
	}
};

struct T_DGN_STL_STIFFENER_PART
{
	CArray<T_DGN_STL_STIFFENER_UNIT, T_DGN_STL_STIFFENER_UNIT&> arStiffD;

	T_DGN_STL_STIFFENER_PART()
	{
		Initialize();
	}

	void Initialize()
	{
		arStiffD.RemoveAll();
	}

	T_DGN_STL_STIFFENER_PART& operator =(const T_DGN_STL_STIFFENER_PART& crD)
	{
		arStiffD.Copy(crD.arStiffD);

		return *this;
	}
};

struct T_DGN_STL_STIFFENER
{
	T_DGN_STL_STIFFENER_PART Flg_Top_L;
	T_DGN_STL_STIFFENER_PART Flg_Top_M;
	T_DGN_STL_STIFFENER_PART Flg_Top_R;
	T_DGN_STL_STIFFENER_PART Flg_Bot_L;
	T_DGN_STL_STIFFENER_PART Flg_Bot_M;
	T_DGN_STL_STIFFENER_PART Flg_Bot_R;

	T_DGN_STL_STIFFENER_PART Web_L;
	T_DGN_STL_STIFFENER_PART Web_R;

	T_DGN_STL_STIFFENER()
	{
		Initialize();
	}

	void Initialize()
	{
		Flg_Top_L.Initialize();
		Flg_Top_M.Initialize();
		Flg_Top_R.Initialize();
		Flg_Bot_L.Initialize();
		Flg_Bot_M.Initialize();
		Flg_Bot_R.Initialize();

		Web_L.Initialize();
		Web_R.Initialize();
	}

	T_DGN_STL_STIFFENER& operator =(const T_DGN_STL_STIFFENER& crD)
	{
		Flg_Top_L	=	crD.Flg_Top_L;
		Flg_Top_M	=	crD.Flg_Top_M;
		Flg_Top_R	=	crD.Flg_Top_R;
		Flg_Bot_L	=	crD.Flg_Bot_L;
		Flg_Bot_M	=	crD.Flg_Bot_M;
		Flg_Bot_R	=	crD.Flg_Bot_R;

		Web_L		= crD.Web_L;
		Web_R		= crD.Web_R;

		return *this;
	}
};
