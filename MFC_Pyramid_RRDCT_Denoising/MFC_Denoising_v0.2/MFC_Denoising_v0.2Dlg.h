
// MFC_Denoising_v0.2Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

#define LAY_NUM 4

// CMFC_Denoising_v02Dlg dialog
class CMFC_Denoising_v02Dlg : public CDialogEx
{
// Construction
public:
	CMFC_Denoising_v02Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFC_DENOISING_V02_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	Mat src;
	Mat dest;
public:

	int patchSize[LAY_NUM];
	int distance[LAY_NUM];
	CString samplingType; 
	int threshold[LAY_NUM];
	int auto_threshold;

	CComboBox m_patch_size;
	CComboBox m_distance;
	CSliderCtrl m_threshold;
	
	CComboBox m_patch_size_1;
	CComboBox m_distance_1;
	CComboBox m_patch_size_2;
	CComboBox m_distance_2;
	CSliderCtrl m_threshold_1;
	CSliderCtrl m_threshold_2;
	CComboBox m_sampling_type;
	
	//CImage srcImage;
	CImage dstImage;

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_src_image;
	CProgressCtrl m_process;
	afx_msg void OnBnClickedOk();
	CStatic m_dst_image;

	
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo8();

	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_auto_threshold;
};
