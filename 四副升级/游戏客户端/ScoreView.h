#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreViewInfo
{
	WORD							wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//������Ϣ
protected:
	WORD							m_wGameScore;						//��Ϸ�÷�
	WORD							m_wConcealTime;						//�۵ױ���
	WORD							m_wConcealScore;					//���ƻ���
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//������Ϣ
protected:
	LONGLONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][LEN_NICKNAME];//�û�����

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageCard;						//�˿�λͼ
	CBitImage						m_ImageWinLose;						//ʤ����־

	//��������
public:
	//���캯��
	CScoreView();
	//��������
	virtual ~CScoreView();
	
	//���ܺ���
public:
	//������Ϣ
	void SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//��꼤��
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
