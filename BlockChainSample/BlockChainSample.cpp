// BlockChainSample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

//#include "pch.h"
#include <iostream>
#include "KISA_SHA256/KISA_SHA256.h"

typedef struct blockheader {
	BYTE previousBlockHash[257];
	int merkleRootHash;
};

typedef struct block {
	int blockSize;
	blockheader header;
	int transactionCount = 0;
	BYTE transaction[100];
};

int main()
{
	block Block[10];
	int select = 0;
	int total;

	printf("첫 거래내용 입력 : ");
	scanf("%s", Block[0].transaction);
	Block[0].transactionCount++;

	Block[0].blockSize = strlen((char*)Block[0].transaction);
	total = 1;

	while (total < 10) {
		printf("%d번째 거래내용 입력 : ", total + 1);
		scanf("%s", Block[total].transaction);
		Block[total].transactionCount = Block[total - 1].transactionCount + 1;

		Block[total].blockSize = strlen((char*)Block[total].transaction);
		SHA256_Encrypt(Block[total - 1].transaction, Block[total - 1].blockSize , Block[total].header.previousBlockHash);
		total++;
	}

	while (1) {
		printf("\n1. 거래내역 확인	2. 거래내역 변조	0. 종료\n");
		scanf("%d", &select);

		if (select == 0) {
			return 0;
		}

		else if (select == 1) {
			int i;

			printf("\n번호 선택(1~9)");
			scanf("%d", &select);

			BYTE EncryptCheck[257];
			SHA256_Encrypt(Block[select - 1].transaction, Block[select - 1].blockSize, EncryptCheck);

			for (i = 0; i < 256; i++) {
				if (Block[select].header.previousBlockHash[i] != EncryptCheck[i]) {
					break;
				}
			}
			if (i != 256) {
				printf("거래기록이 변조되었습니다\n");
			}
			else {
				printf("거래기록 : %s\n", Block[select - 1].transaction);
			}
		}
		else if (select == 2) {
			printf("\n번호 선택(1~9)");
			scanf("%d", &select);

			Block[select - 1].transaction[0] = (BYTE)"\0";
			printf("\n거래 내용 입력 : ");
			scanf("%s", Block[select - 1].transaction);
			printf("변조 완료\n");
		}
	}

	return 0;
}