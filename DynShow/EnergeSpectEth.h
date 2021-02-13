#if !defined(AFX_ENERGESPECTETH_H__A27986F5_1D68_42D9_8B96_88EA5CE34072__INCLUDED_)
#define AFX_ENERGESPECTETH_H__A27986F5_1D68_42D9_8B96_88EA5CE34072__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnergeSpectEth.h : header file
//
//#include "FileSave.h"


/////////////////////////////////////////////////////////////////////////////
// CEnergeSpectEth dialog

class CEnergeSpectEth : public CDialog
{
// Construction
public:
	CEnergeSpectEth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnergeSpectEth)
	enum { IDD = IDD_ENERGESPECT_SHOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnergeSpectEth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnDraw64EnergeSpectrum();
	void OnDrawZoomInEnergeSpect(int nblock,int num);
	void OnLoadEthFile();
	void OnSaveEthFile();
	void OnFastCalcEthLowHighLimit();


//	CFileSave  filesave;
	BOOL fileoper_Newfile;
	int fileoper_iblocksel;

		//-------��64���ײ�����--------//
	BOOL  m_bEnergeSpectOperate_En;
	int	m_iEnergeSpectrum64[8][64][64];  //8:��ţ�64�����ţ�64����ֵ

	BOOL m_bEnergeSpectSelEn; //����64Сͼѡ��ʹ��
	BOOL m_bEnergeSpectZoomInMode; //���׵ķŴ�ģʽ
	int m_iEnergeSpectShowNum;	//���׷Ŵ���ʾ����

	int m_iEnergeSpectEth_Low[8][64];	//�������� block�����ţ�
	int m_iEnergeSpectEth_Hight[8][64];
	int m_iEnergeSpectCurBlockNo;
	int m_iEnergeSpectCurSynNo; //��ǰ�����
	BOOL m_bEnergeSpectMove_Eth_Low;
	BOOL m_bEnergeSpectMove_Eth_Hight;




	CPoint m_Oldpoint;
	CPoint m_Newpoint;

protected:

	// Generated message map functions
	//{{AFX_MSG(CEnergeSpectEth)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENERGESPECTETH_H__A27986F5_1D68_42D9_8B96_88EA5CE34072__INCLUDED_)
