#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_LAND_SCORE				(WM_USER+103)						//�з���Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_LAND_AUTOPLAY			(WM_USER+105)                       //�й���Ϣ
#define IDM_SORT_HAND_CARD			(WM_USER+106)						//�����˿�
//
#define IDM_MING_CARD_STATE				(WM_USER+107)						//���ƿ�ʼ
#define IDM_MING_CARD				(WM_USER+108)						//����
#define IDM_QIANG_LAND				(WM_USER+109)						//������
#define IDM_MING_OUT				(WM_USER+110)						//��������
#define IDM_ADD						(WM_USER+111)						//�ӱ�

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONGLONG						m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������
	BYTE							m_bLaiZiCard;						//�����˿�
	bool							m_bLaiZiDisplay;					//��ʾ����
	BYTE							m_bRandCardX;
	BYTE							m_bRandCardY;

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bDeasilOrder;						//˳ʱ���
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	bool							m_bShowScore;						//����״̬
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ
	bool							m_bAdd[GAME_PLAYER];				//�ӱ���Ϣ
	bool							m_bAddStates[GAME_PLAYER];			//�ӱ�״̬
	bool							m_bLaiZi;							//���ӿ�

	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��

	INT								m_nXTimer;
	INT								m_nYTimer;

	//λͼ����
protected:
	CBitImage						m_ImageBomb;						//ը��ͼƬ
	CBitImage						m_ImageBack;						//������Դ
	CBitImage						m_ImageCenter;						//������Դ
	CBitImage						m_ImageBombEffect;					//ը��Ч��
	CBitImage						m_ImageHistoryScore;				//��ʷ����
	CBitImage						m_ImageLaiZiCard;					//���Ӷ���

	CPngImage						m_ImageScore;						//����ͼƬ
	CPngImage						m_ImageLand;						//ׯ��ͼƬ
	CPngImage						m_ImageUserTrustee;					//����й�
	CPngImage						m_ImageDouble;						//�ӱ�
	CPngImage						m_ImageNumber;						//����
	CPngImage						m_ImagePlayerRL;						//�û���
	CPngImage						m_ImagePlayerTB;						//�û���
	CPngImage						m_ImageLaiZi;						//���ӿ�

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOneScore;						//1 �ְ�ť
	CSkinButton						m_btTwoScore;						//2 �ְ�ť
	CSkinButton						m_btThreeScore;						//3 �ְ�ť
	CSkinButton						m_btGiveUpScore;					//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btSortCard;						//����ť
	CSkinButton						m_btAutoPlayOn;						// �����Զ��й�
	CSkinButton						m_btAutoPlayOff;					// �ر��Զ��й�
	CSkinButton						m_btScore;							//���ְ�ť
	//
	CSkinButton						m_btMingCardState;						//���ƿ�ʼ
	CSkinButton						m_btMingCard;						//����
	CSkinButton						m_btQiangLand;						//������
	CSkinButton						m_btNoQiang;						//����
	CSkinButton						m_btMingOut;						//��������
	CSkinButton						m_btAdd;							//�ӱ�
	CSkinButton						m_btNoAdd;							//����

	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[3];				//�˿���ͼ
	CCardControl					m_HandCardControl[3];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CSize							m_sizeHistory;						//���ִ�С

#ifdef VIDEO_GAME

	//��Ƶ���
private:
	CVideoServiceManager			m_VedioServiceManager;				//��Ƶ����
	CVideoServiceControl			m_DlgVedioService[GAME_PLAYER];		//��Ƶ���

#endif

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//������Ϣ
	virtual void AddGameTableMessage(LPCTSTR pszMessage,WORD wLen,WORD wType){};
	//�Ƿ��������
	virtual bool RealizeWIN7() { return true; }

	//���ܺ���
public:
	//ը������
	void SetBombTime(WORD wBombTime);
	//��������
	void SetBaseScore(LONGLONG lBaseScore);
	//��ʾ��ʾ
	void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�ڿӷ���
	void SetLandUser(WORD wChairID, BYTE bLandScore);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, BYTE bLandScore);
	//����˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//����й�
	void SetUserTrustee( WORD wChairID, bool bTrustee );
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
	//���üӱ�
	void SetAdd(WORD wAddUser,bool bAdd){m_bAdd[wAddUser]=bAdd;};
	//���üӱ�
	void SetAddStates(WORD wAddUser,bool bAddStates){m_bAddStates[wAddUser]=bAddStates;};
	//��������
	void SetLaiZiCard(BYTE bLaiZiCard){m_bLaiZiCard=bLaiZiCard;};
	//��ʾ����
	void SetLaiZiDisplay(bool bDisplay){m_bLaiZiDisplay=bDisplay;};
	//����
	void DrawNumber(CDC *pDC,int x,int y,int bNum);
	//���ӿ�
	void SetLaiZiKuang(bool bLaiZi){m_bLaiZi=bLaiZi;};
	//���Ӷ���֡
	void SetFrame(BYTE bx,BYTE by){m_bRandCardX=bx,m_bRandCardY=by;};
	//������ͼ
	void RefreshGameView();

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�зְ�ť
	afx_msg void OnOneScore();
	//�зְ�ť
	afx_msg void OnTwoScore();
	//������ť
	afx_msg void OnGiveUpScore();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�зְ�ť
	afx_msg void OnThreeScore();
	// �Զ��й�
    afx_msg void OnAutoPlayerOn();
    // ȡ���й�
    afx_msg void OnAutoPlayerOff();
	//���ƿ�ʼ
	afx_msg void OnMingCardState();
	//����
	afx_msg void OnMingCard();
	//������
	afx_msg void OnQiangLand();
	//��������
	afx_msg void OnNoQiangLand();
	//��������
	afx_msg void OnMingOut();
	//�ӱ�
	afx_msg void OnAdd();
	//���ӱ�
	afx_msg void OnNoAdd();
	//����ť
	VOID OnBnClickedSortCard();
	//���ְ�ť
	VOID OnBnClickedScore();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
