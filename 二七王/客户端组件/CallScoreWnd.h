#ifndef CALL_SCORE_VIEW_HEAD_FILE
#define CALL_SCORE_VIEW_HEAD_FILE

#pragma once
#include "Stdafx.h"
#include "Resource.h"

#define IDC_CALL_SCORE			700								//�����з�
#define IDC_GIVE_UP				800								//�����з�

#define IDM_LAND_SCORE			(WM_USER+700)					//�з���Ϣ

//////////////////////////////////////////////////////////////////////////


//������ͼ��
class CCallScoreView : public CWnd
{
	//������Ϣ
protected:
	LONGLONG							m_lCurrentCallScore;
	CSkinButton						m_btGiveUp;
	CSkinButton						m_btScore[25];
	CGameFrameViewGDI *				m_pSinkWindow;						//�ص�����
	//��Դ����
protected:
	CBitImage						m_ImageBack;						//����ͼ��

	//��������
public:
	//���캯��
	CCallScoreView();
	//��������
	virtual ~CCallScoreView();
	
	//���ܺ���
public:
	void SetCurrentCallScore(LONGLONG lScore);
	void SetSinkWindow(CGameFrameViewGDI * pSinkWindow){m_pSinkWindow = pSinkWindow;}
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif