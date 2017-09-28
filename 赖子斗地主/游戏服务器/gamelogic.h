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
#define GOOD_CARD_COUTN				38									//������Ŀ
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

#define MAX_TYPE_COUNT 254
struct tagOutCardTypeResult 
{
	int								cbCardType;							//�˿�����
	BYTE							cbCardTypeCount;					//������Ŀ
	BYTE							cbEachHandCardCount[MAX_TYPE_COUNT];//ÿ�ָ���
	BYTE							cbCardData[MAX_TYPE_COUNT][MAX_COUNT];//�˿�����
};


//�˿���Ϣ
struct tagHandCardInfo {
	BYTE						cbHandCardData[ MAX_COUNT ];				//�˿�����
	BYTE						cbHandCardCount;							//�˿���Ŀ
	tagOutCardTypeResult		CardTypeResult[ 14 + 1 ] ;					//��������

	//��ʼ����
	tagHandCardInfo( void ) {
		ZeroMemory( cbHandCardData, sizeof( cbHandCardData ) ) ;
		cbHandCardCount = 0;
		ZeroMemory( &CardTypeResult, sizeof( CardTypeResult ) );
	}
};

//���Ͷ���
typedef CWHArray< tagHandCardInfo * > tagHandCardInfoArray;

//ջ�ṹ
class tagStackHandCardInfo {

	//��������
public:

	//���캯��
	tagStackHandCardInfo( void ) { 
		m_HandCardInfoFreeArray.RemoveAll(); 
		m_HandCardInfoArray.RemoveAll();
	}

	//��������
	~tagStackHandCardInfo( void ) {

		//���ջ
		ClearAll();
	}

	//Ԫ��ѹջ
	void Push( tagHandCardInfo * pHandCardInfo ) {

		//�Ƿ��пռ�
		if ( 0 < m_HandCardInfoFreeArray.GetCount() ) {
			//��ȡ�ռ�
			tagHandCardInfo * pHandCardInfoFree = m_HandCardInfoFreeArray[ 0 ];
			m_HandCardInfoFreeArray.RemoveAt( 0 );

			//Ԫ�ظ�ֵ
			CopyMemory( pHandCardInfoFree->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pHandCardInfoFree->cbHandCardData ) );
			pHandCardInfoFree->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			CopyMemory( pHandCardInfoFree->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//ѹ��ջ��
			INT_PTR nECount = m_HandCardInfoArray.GetCount() ; 
			m_HandCardInfoArray.InsertAt( nECount, pHandCardInfoFree );
		}
		else {
			//����ռ�
			tagHandCardInfo * pNewHandCardInfo = new tagHandCardInfo ;

			//Ԫ�ظ�ֵ
			CopyMemory( pNewHandCardInfo->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pNewHandCardInfo->cbHandCardData ) );
			pNewHandCardInfo->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			CopyMemory( pNewHandCardInfo->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//ѹ��ջ��
			INT_PTR nECount = m_HandCardInfoArray.GetCount() ; 
			m_HandCardInfoArray.InsertAt( nECount, pNewHandCardInfo );
		}
		
	}

	//����ջ��
	void Pop() {

		//�ǿ��ж�
		if ( IsEmpty() ) return ;

		//��ȡԪ��
		INT_PTR nECount = m_HandCardInfoArray.GetCount() ;
		tagHandCardInfo * pTopHandCardInfo = m_HandCardInfoArray.GetAt( nECount - 1 );

		//�Ƴ�Ԫ��
		m_HandCardInfoArray.RemoveAt( nECount - 1 );

		//����ռ�
		m_HandCardInfoFreeArray.Add( pTopHandCardInfo );		
	}

	//��ʼջ
	void InitStack() {

		//����ռ�
		while ( 0 < m_HandCardInfoArray.GetCount() ) {
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoArray[ 0 ];
			m_HandCardInfoArray.RemoveAt( 0 );
			m_HandCardInfoFreeArray.Add( pHandCardInfo );
		}
	}

	//���ջ
	void ClearAll() {

		//�ͷ��ڴ�
		while ( 0 < m_HandCardInfoArray.GetCount() ) {
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoArray[ 0 ];
			delete pHandCardInfo;
			pHandCardInfo = NULL;
			m_HandCardInfoArray.RemoveAt( 0 );
		}

		//�ͷ��ڴ�
		while ( 0 < m_HandCardInfoFreeArray.GetCount() ) {
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoFreeArray[ 0 ];
			delete pHandCardInfo;
			pHandCardInfo = NULL;
			m_HandCardInfoFreeArray.RemoveAt( 0 );
		}
	}

	//��ȡջ��
	void GetTop( tagHandCardInfo * & pHandCardInfo ) {

		//�ǿ��ж�
		if ( IsEmpty() ) {
			ASSERT( false );
			return;
		}

		//��ȡԪ��
		INT_PTR nECount = m_HandCardInfoArray.GetCount() ;
		pHandCardInfo = m_HandCardInfoArray[ nECount - 1 ];
	}

	//���ж�
	bool IsEmpty() {
		return m_HandCardInfoArray.IsEmpty();
	}

	//��Ա����
private:
	tagHandCardInfoArray			m_HandCardInfoFreeArray;					//�˿���Ϣ
	tagHandCardInfoArray			m_HandCardInfoArray;						//�˿���Ϣ

};


//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����
	static const BYTE				m_cbGoodcardData[GOOD_CARD_COUTN];	//��������
	BYTE							m_bLaiZiCard;						//������
	
	//AI����
public:
	BYTE							m_cbAllCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE							m_cbLandScoreCardData[MAX_COUNT];	//�����˿�
	BYTE							m_cbUserCardCount[GAME_PLAYER];		//�˿���Ŀ
	WORD							m_wBankerUser;						//�������
	LONGLONG							m_lBankerOutCardCount ;				//���ƴ���
	tagStackHandCardInfo			m_StackHandCardInfo;				//ջ����

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
	//��ȡ���ͽ��
	int GetType(tagAnalyseResult AnalyseResult,const BYTE cbCardData[],BYTE cbCardCount,const BYTE NoChangeCard[]);

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�õ�����
	void GetGoodCardData(BYTE cbGoodCardData[NORMAL_COUNT]) ;
	//ɾ������
	bool RemoveGoodCardData(BYTE cbGoodcardData[NORMAL_COUNT], BYTE cbGoodCardCount, BYTE cbCardData[FULL_COUNT], BYTE cbCardCount) ;
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//����˿�
	BYTE GetRandomCard(void) ;

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��ȡ����
	BYTE GetLaiZiCard();
	//������Ŀ
	BYTE GetLaiZiCount(const BYTE cbHandCardData[], BYTE cbHandCardCount);
	//��������
	void SetLaiZiCard(BYTE bLaiZiCard){m_bLaiZiCard=GetCardValue(bLaiZiCard);};

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


#endif
