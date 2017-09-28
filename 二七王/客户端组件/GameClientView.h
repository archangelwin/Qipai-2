#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CallCardWnd.h"
#include "CardControl.h"
#include "ConcealCardView.h"
#include "CallScoreWnd.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��ť��Ϣ
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_OUT_PROMPT				(WM_USER+102)						//��ʾ��Ϣ
#define IDM_SEND_CONCEAL			(WM_USER+103)						//������Ϣ

//���ܰ�ť
#define IDM_CONCEAL_CARD			(WM_USER+110)						//�鿴����
#define IDM_REQUEST_LEAVE			(WM_USER+111)						//�����뿪
#define IDM_LAST_TURN_CARD			(WM_USER+112)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+113)						//�йܿ���
#define IDM_GIVEUP_GAME				(WM_USER+114)						//�йܿ���

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��ʾ����
protected:
	bool							m_bDeasilOrder;						//����˳��
	bool							m_bWaitConceal;						//�ȴ���־
	bool							m_bLastTurnCard;					//�����˿�
	bool							m_bUserNoMain[GAME_PLAYER];			//�����û�
	BYTE							m_bMainCount[GAME_PLAYER];	
	BYTE							m_bDoubleCount[GAME_PLAYER];	
	LONGLONG							m_lWidth,m_lHeight;

	//��Ϸ����
protected:
	int								m_wPlayerScore[GAME_PLAYER];		//���Ե÷�
	bool							m_bGiveUpPlayer[GAME_PLAYER];
	WORD							m_wCardScore;						//��ҵ÷�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_bScore[GAME_PLAYER];				//�û��з�
	WORD							m_wCurrentCallScore;				//��ǰ�з�
	WORD							m_wUserCallScore;					//�û��з�

	//������Ϣ
protected:
	BYTE							m_cbCallColor;						//���ƻ�ɫ
	BYTE							m_cbCallCount;						//������Ŀ
	BYTE							m_cbMainValue;						//������ֵ
	WORD							m_wCallCardUser;					//�����û�
	BYTE							m_cbValueOrder[2];					//������ֵ

	//λ����Ϣ
protected:
	CSize							m_SizeBanker;						//ׯ�Ҵ�С
	CSize							m_SizeStation;						//״̬��С
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptAuto[GAME_PLAYER];				//�й�λ��
	CPoint							m_ptNoMain[GAME_PLAYER];			//����λ��

	//λͼ����
protected:
	CBitImageEx						m_ImageBack;						//������Դ
	CBitImageEx						m_ImageCenter;
	CBitImageEx						m_ImageBanker;						//ׯ����Դ
	CBitImageEx						m_ImageLastTurn;					//���ֱ�־
	CBitImageEx						m_ImageColorFlag;					//��ɫ��־
	CBitImageEx						m_ImageWaitConceal;					//�ȴ���Դ
	CBitImageEx						m_ImageStationTitle;				//״̬��Դ

	CBitImageEx						m_ImageNoMain;						//������Դ
	CPngImageEx						m_ImageNoCall;
	CPngImageEx						m_ImageNumber;						//������Դ
	CBitImageEx						m_ImageScoreCard;					//�÷���

	CPngImageEx						m_ImageMain1;
	CPngImageEx						m_ImageMain2;

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSendConceal;					//���װ�ť
	CSkinButton						m_btGiveUp;

	//���ܰ�ť
public:
	CSkinButton						m_btConcealCard;					//�鿴����
	//CSkinButton						m_btRuquestLeave;					//�����뿪
	CSkinButton						m_btLastTurnCard;					//�����˿�
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//�˿˿ؼ�
public:
	CCardControl					m_CardScore;						//�÷��˿�
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ
	CCardControl					m_BackCardControl;					//�����˿�


	CCardControl					m_GmCardControl[4];

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CCallScoreView					m_CallScoreView;					
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CCallCardWnd					m_CallCardWnd;						//���ƴ���
	CConcealCardView				m_ConcealCardView;					//���ƴ���

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//���غ���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//��Ϸ��Ϣ
public:
	//���õ÷�
	void SetCardScore(WORD wCardScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//���õȴ�
	void SetWaitConceal(bool bWaitConceal);
	//��������
	void SetLastTurnCard(bool bLastTurnCard);
	//��������
	void SetValueOrder(BYTE cbMainValue, BYTE cbValueMySelf, BYTE cbValueOther);
	//��������
	void SetCallCardInfo(WORD wCallCardUser, BYTE cbCallColor, BYTE cbCallCount);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, WORD bLandScore);
	//���ýз�
	void SetCurrentCallScore(WORD wCurrentCallScore);
	//��ҷ���
	void SetPlayerScore(int wPlayerScore[4]);
	//�����з�
	void SetGiveUpPlayer(bool bGiveUpPlayer[4]);
	void SetUserScore(WORD wUserScore[4]);
	//��������
	void SetNoMainUser(bool bNoMainUser[4]);

	void SetGMCard();
	//��������
	void SetMainCount(BYTE bMainCount[4],BYTE bDoubleCount[4]);

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);

	//�ڲ�����
private:
	//�滭����
	VOID DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);

	//��Ϣӳ��
protected:

	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif