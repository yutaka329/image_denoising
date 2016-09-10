
// MFC_Denoising_v0.2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Denoising_v0.2.h"
#include "MFC_Denoising_v0.2Dlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "./src/algorithm/rrdct.h"
#include "./src/algorithm/pyramid.h"
#include "./src/algorithm/noiseEstimate.h"
#include "./src/MatCImage.h"

using namespace std;
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_Denoising_v02Dlg dialog



CMFC_Denoising_v02Dlg::CMFC_Denoising_v02Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_Denoising_v02Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_Denoising_v02Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_patch_size);
	DDX_Control(pDX, IDC_COMBO2, m_distance);
	DDX_Control(pDX, IDC_COMBO3, m_sampling_type);
	DDX_Control(pDX, IDC_SLIDER1, m_threshold);
	DDX_Control(pDX, IDC_SRC_IMG, m_src_image);
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
	DDX_Control(pDX, IDC_DST_IMG, m_dst_image);
	DDX_Control(pDX, IDC_COMBO4, m_patch_size_1);
	DDX_Control(pDX, IDC_COMBO5, m_distance_1);
	DDX_Control(pDX, IDC_COMBO7, m_patch_size_2);
	DDX_Control(pDX, IDC_COMBO8, m_distance_2);
	DDX_Control(pDX, IDC_SLIDER2, m_threshold_1);
	DDX_Control(pDX, IDC_SLIDER3, m_threshold_2);
	DDX_Control(pDX, IDC_EDIT3, m_auto_threshold);
}

BEGIN_MESSAGE_MAP(CMFC_Denoising_v02Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDOK, &CMFC_Denoising_v02Dlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo8)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider3)
END_MESSAGE_MAP()


// CMFC_Denoising_v02Dlg message handlers

BOOL CMFC_Denoising_v02Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);


//	_CrtDumpMemoryLeaks();
//	return TRUE;
	// TODO: Add extra initialization here
	patchSize[0] = 8;
	patchSize[1] = 4;
	patchSize[2] = 4;
	distance[0] = 1;
	distance[1] = 1;
	distance[2] = 1;
	threshold[0] = 0;
	threshold[1] = 0;
	threshold[2] = 0;
	samplingType = "lattice";
	// IDC_COMBO1
	//m_patch_size.AddString(_T("2"));   //take too much time
	m_patch_size.AddString(_T("4"));
	m_patch_size.AddString(_T("8"));
	m_patch_size.AddString(_T("16"));
	m_patch_size.SetCurSel(0);
	// IDC_COMBO2
	m_distance.AddString(_T("1"));
	m_distance.AddString(_T("2"));
	m_distance.AddString(_T("3"));
	m_distance.AddString(_T("4"));
	m_distance.AddString(_T("5"));
	m_distance.AddString(_T("6"));
	m_distance.AddString(_T("7"));
	m_distance.AddString(_T("8"));
	m_distance.AddString(_T("9"));
	m_distance.AddString(_T("10"));
	m_distance.AddString(_T("11"));
	m_distance.AddString(_T("12"));
	m_distance.AddString(_T("13"));
	m_distance.AddString(_T("14"));
	m_distance.AddString(_T("15"));
	m_distance.AddString(_T("16"));
	m_distance.SetCurSel(0);
	// IDC_COMBO3
	m_sampling_type.AddString(_T("lattice"));
	m_sampling_type.AddString(_T("poission"));
	m_sampling_type.AddString(_T("full"));
	m_sampling_type.SetCurSel(0);
	//IDC_SLIDER1
	m_threshold.SetRange(0,500);
	m_threshold.SetPos(0);
	m_threshold.SetTicFreq(10);
	threshold[0] = m_threshold.GetPos();

	// IDC_COMBO4
	m_patch_size_1.AddString(_T("4"));
	m_patch_size_1.AddString(_T("8"));
	m_patch_size_1.AddString(_T("16"));
	m_patch_size_1.SetCurSel(0);
	// IDC_COMBO5
	m_distance_1.AddString(_T("1"));
	m_distance_1.AddString(_T("2"));
	m_distance_1.AddString(_T("3"));
	m_distance_1.AddString(_T("4"));
	m_distance_1.AddString(_T("5"));
	m_distance_1.AddString(_T("6"));
	m_distance_1.AddString(_T("7"));
	m_distance_1.AddString(_T("8"));
	m_distance_1.AddString(_T("9"));
	m_distance_1.AddString(_T("10"));
	m_distance_1.AddString(_T("11"));
	m_distance_1.AddString(_T("12"));
	m_distance_1.AddString(_T("13"));
	m_distance_1.AddString(_T("14"));
	m_distance_1.AddString(_T("15"));
	m_distance_1.AddString(_T("16"));
	m_distance_1.SetCurSel(0);

	// IDC_COMBO7
	m_patch_size_2.AddString(_T("4"));
	m_patch_size_2.AddString(_T("8"));
	m_patch_size_2.AddString(_T("16"));
	m_patch_size_2.SetCurSel(0);
	// IDC_COMBO8
	m_distance_2.AddString(_T("1"));
	m_distance_2.AddString(_T("2"));
	m_distance_2.AddString(_T("3"));
	m_distance_2.AddString(_T("4"));
	m_distance_2.AddString(_T("5"));
	m_distance_2.AddString(_T("6"));
	m_distance_2.AddString(_T("7"));
	m_distance_2.AddString(_T("8"));
	m_distance_2.AddString(_T("9"));
	m_distance_2.AddString(_T("10"));
	m_distance_2.AddString(_T("11"));
	m_distance_2.AddString(_T("12"));
	m_distance_2.AddString(_T("13"));
	m_distance_2.AddString(_T("14"));
	m_distance_2.AddString(_T("15"));
	m_distance_2.AddString(_T("16"));
	m_distance_2.SetCurSel(0);
	//IDC_SLIDER2
	m_threshold_1.SetRange(0,500);
	m_threshold_1.SetPos(0);
	m_threshold_1.SetTicFreq(10);
	threshold[1] = m_threshold.GetPos();
	//IDC_SLIDER3
	m_threshold_2.SetRange(0,500);
	m_threshold_2.SetPos(0);
	m_threshold_2.SetTicFreq(10);
	threshold[2] = m_threshold.GetPos();

	// Src image
	string srcImagePath = "../noise.jpg";
	CString imagePath(srcImagePath.c_str());   //std::str to CString
	if(!PathFileExists(imagePath)) {
		return NULL;
	}
	CImage srcImage;
	srcImage.Load(imagePath);
	HBITMAP hBmp = srcImage.Detach();
	m_src_image.SetBitmap(hBmp);          
	//m_srcImg.SetBitmap(HBITMAP(srcImage));
	src = imread(srcImagePath);

	// IDC_PROGRESS1
	m_process.SetRange(0, 100);
	m_process.SetStep(10);
	m_process.SetPos(0);

	//noise estimation
	//Mat src = imread(srcImagePath);
	//NoiseEstimate noise(src);
	//auto_threshold = (int)(noise.getNoiseLevel() * 100);  //the parameter is arbitraty
	// display noise level
	//CString str;
	//str.Format(_T("%d"),auto_threshold);
	//SetDlgItemText(IDC_EDIT3, str);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFC_Denoising_v02Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFC_Denoising_v02Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFC_Denoising_v02Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_patch_size.GetCurSel();
	m_patch_size.GetLBText(nSel, str);
	patchSize[0] = _ttoi(str);
	//TRACE("block_size=%d\n", patchSize);
}


void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_distance.GetCurSel();
	m_distance.GetLBText(nSel, str);
	distance[0] = _ttoi(str);
	//TRACE("distance=%d\n", distance);
}


void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo3()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_sampling_type.GetCurSel();
	m_sampling_type.GetLBText(nSel, str);
	samplingType = str;
}

void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo4()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_patch_size_1.GetCurSel();
	m_patch_size_1.GetLBText(nSel, str);
	patchSize[1] = _ttoi(str);
}


void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo7()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_patch_size_2.GetCurSel();
	m_patch_size_2.GetLBText(nSel, str);
	patchSize[2] = _ttoi(str);
}


void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo5()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_distance_1.GetCurSel();
	m_distance_1.GetLBText(nSel, str);
	distance[1] = _ttoi(str);
}


void CMFC_Denoising_v02Dlg::OnCbnSelchangeCombo8()
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel;
	nSel = m_distance_2.GetCurSel();
	m_distance_2.GetLBText(nSel, str);
	distance[2] = _ttoi(str);
}


void CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	threshold[1] = m_threshold_1.GetPos();
	
	CString str;
	str.Format(_T("%d"),threshold[1]);
	SetDlgItemText(IDC_EDIT2, str);
}


void CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	threshold[2] = m_threshold_2.GetPos();
	
	CString str;
	str.Format(_T("%d"),threshold[2]);
	SetDlgItemText(IDC_EDIT6, str);
}


void CMFC_Denoising_v02Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//@TN
	//	*pResult = 0;
	// TODO: Add your control notification handler code here
	//*pResult = 0;

	threshold[0] = m_threshold.GetPos();
	
	CString str;
	str.Format(_T("%d"),threshold[0]);
	SetDlgItemText(IDC_EDIT1, str);
}


void CMFC_Denoising_v02Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

	m_process.SetPos(0);   //设置当前进度0

	m_process.StepIt();   //first step of process control
	m_process.StepIt();   //second step of process control

	// 执行 RRDCT 去噪算法
	//RandomizedRedundantDCT rrdct;
	//PyramidDenoising pyrDen();
	
	wstring wsampling = samplingType.GetBuffer();
	string sampling;
	sampling.resize((int)wsampling.length(),' ');
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wsampling.c_str(),(int)wsampling.length(),(LPSTR)sampling.c_str(),(int)wsampling.length(),NULL,NULL);

	//int bs = patchSize;
	//Size psize = Size(bs, bs);
	//int d = distance;
	//float sigma = (float)threshold;

	RandomizedRedundantDCT::BASIS dctbasis;
	RandomizedRedundantDCT::SAMPLING samplingType;
	dctbasis = RandomizedRedundantDCT::BASIS::DCT;
	if (sampling == "lattice") {
		samplingType = RandomizedRedundantDCT::SAMPLING::LATTICE;
	}
	else if (sampling == "poission") {
		samplingType = RandomizedRedundantDCT::SAMPLING::POISSONDISK;
	}
	else {
		samplingType = RandomizedRedundantDCT::SAMPLING::FULL;
	}

	vector<Size> v_patch_size;
	vector<float> v_sigma;
	vector<int> v_d;
	//initial datas
	for(int i=0; i< LAY_NUM-1; i++) {
		int bs = patchSize[i];
		Size psize = Size(bs, bs);
		int d = distance[i];
		float sigma = (float)threshold[i];                      //manual setting
		//float sigma =  auto_threshold / (i+1);                      //auto setting
		v_patch_size.push_back(psize);
		v_sigma.push_back(sigma);
		v_d.push_back(d);
	}

	m_process.StepIt();   //third step of process control
	m_process.StepIt();   //forth step of process control
	m_process.StepIt();   //fifth step of process control
	m_process.StepIt();   //sixth step of process control

	PyramidDenoising pd(LAY_NUM, v_sigma, v_patch_size, v_d);
	pd(src, dest, samplingType, dctbasis);

	m_process.StepIt();   //seventh step of process control
	m_process.StepIt();   //eighth step of process control

	//imshow("original", src);
	//imshow("denoised", dest);

	//dest.type() = CV_16SC3
	dest.convertTo(dest, CV_8UC3);
	// 显示结果
	MatCImage mat_cimage;
	mat_cimage.MatToCImage(dest, dstImage);
	HBITMAP hBmp = dstImage.Detach();
	m_dst_image.SetBitmap(hBmp);

	m_process.StepIt();   //ninth step of process control
	m_process.StepIt();   //tenth step of process control

	// 保存图片
	
	imwrite("../results/denoised.png", dest);  //
}

