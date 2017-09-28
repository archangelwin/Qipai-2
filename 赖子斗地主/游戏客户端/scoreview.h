#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONGLONG						m_lGameTax;							//��Ϸ˰��
	LONGLONG						m_lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][LEN_NICKNAME];//�û�����
	BYTE							m_bUserAdd[GAME_PLAYER];			//�û��ӱ�
	int								m_iOtherAdd[5];		//�����ӱ� 0�ܱ���1���ƣ�2��������3ը����4����
	WORD							m_wPlayTime;			//����
	LONGLONG						m_lGameResult;			//�ɼ�

	//��Դ����
protected:
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageWinLose;						//ʤ����ʶ
	CPngImage						m_ImageNumber;						//����
	CPngImage						m_PngBack;							//����
	CPngImage						m_PngWinLose;						//ʤ����ʶ
	CSkinResourceManager *          m_pSkinResourceManager;             //��Դ������

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetGameTax(LONGLONG lGameTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,BYTE *pUserAdd,int *iOtherAdd,WORD wPlayTime,LONGLONG lGameResult);
	//����
	void DrawNumber(CDC *pDC,int x,int y,int bNum);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
