#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER					0									//��С����
#define ST_COUNT					1									//��Ŀ����

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT					20									//�����Ŀ
#define FULL_COUNT					54									//ȫ����Ŀ
#define BACK_COUNT					3									//������Ŀ
#define NORMAL_COUNT				17									//������Ŀ

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
//#define CT_ERROR					0x000000000									//��������
//#define CT_SINGLE					0x000000001									//��������
//#define CT_DOUBLE					0x000000002									//��������
//#define CT_THREE					0x000000004									//��������
//#define CT_SINGLE_LINE				0x000000008									//��������
//#define CT_DOUBLE_LINE				0x000000010									//��������
//#define CT_THREE_LINE				0x000000020									//��������
//#define CT_THREE_LINE_TAKE_ONE		0x000000040									//����һ��
//#define CT_THREE_LINE_TAKE_TWO		0x000000080									//����һ��
//#define CT_FOUR_LINE_TAKE_ONE		0x000000100									//�Ĵ�����
//#define CT_FOUR_LINE_TAKE_TWO		0x000000200									//�Ĵ�����
//#define CT_RUAN_BOMB				0x000000400									//��ը��
//#define CT_BOMB_CARD				0x000000800									//ը������
//#define CT_LAIZI_BOMB				0x000001000									//����ը��
//#define CT_MISSILE_CARD				0x000002000									//�������

#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINE				4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_LINE_TAKE_ONE		7									//����һ��
#define CT_THREE_LINE_TAKE_TWO		8									//����һ��
#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�����
#define CT_FOUR_LINE_TAKE_TWO		10									//�Ĵ�����
#define CT_RUAN_BOMB				11									//��ը��
#define CT_BOMB_CARD				12									//ը������
#define CT_LAIZI_BOMB				13									//����ը��
#define CT_MISSILE_CARD				14									//�������

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE							cbFourCardData[60];			//�����˿�
	BYTE							cbThreeCardData[200];			//�����˿�
	BYTE							cbDoubleCardData[200];		//�����˿�
	BYTE							cbSignedCardData[200];		//�����˿�
	bool							cbLaiZi;							//��������
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����
	BYTE							m_bLaiZiCard;						//������

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	int GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }
	//��������
	void SetLaiZiCard(BYTE bLaiZiCard){m_bLaiZiCard=GetCardValue(bLaiZiCard);};
	//��ȡ���ͽ��
	int GetType(tagAnalyseResult AnalyseResult,const BYTE cbCardData[],BYTE cbCardCount,const BYTE NoChangeCard[]);

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//�����˿�
	void SortCardListEx(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult,BYTE bChangeCard[]);
	//��ȡ����
	BYTE GetLaiZiCard();
	//������Ŀ
	BYTE GetLaiZiCount(const BYTE cbHandCardData[], BYTE cbHandCardCount);

	//�ڲ�����
public:
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����Ƚ�
	bool AnalysebCompare(const BYTE cbCardData[],BYTE cbCardCount,tagAnalyseResult & AnalyseResult,int dCardType);
	//����ת��	(��32���Ƶ�ʹ�����ת��Ϊһ��32λ����)
	int TransListToInt(int iOldType,int iNewType);
	//�任���˿�
	void ChangeCard(int iType,BYTE cbNextCard[],BYTE cbNextCount,BYTE bChangeCard[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
