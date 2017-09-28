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
	int								wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
	bool							bAddConceal;						//�Ƿ�ׯ��
	BYTE							bEndStatus;
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//������Ϣ
protected:
	int								m_wGameScore;						//��Ϸ�÷�
	WORD							m_wLandUser;
	WORD							m_wLandScore;
	WORD							m_wConcealTime;						//�۵ױ���
	WORD							m_wConcealScore;					//���ƻ���
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];				//�����˿�
	bool							m_bAddConceal;
	BYTE							m_bEndStatus;

	//������Ϣ
protected:
	LONGLONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	LONGLONG							m_lKingScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][LEN_NICKNAME];//�û�����

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageCard;						//�˿�λͼ
	CBitImage						m_ImageWinLose;						//ʤ����־

	CSkinButton						m_btOkStart;
	CGameFrameViewGDI *				m_pSinkWindow;						//�ص�����
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
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,LONGLONG lKingScore);
	//���ýз�
	void SetLandScore(WORD wLandUser,WORD wLandScore);
	void SetSinkWindow(CGameFrameViewGDI * pSinkWindow){m_pSinkWindow = pSinkWindow;}
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	BOOL OnCommand(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif