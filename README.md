

기본 구조 세팅

접두어 KHBB

\- Kim Hyeong Baek Baseball

Game

ㄴGameModeBase : 중요 게임 로직 담당

ㄴGameStateBase : 모든 클라이언트에게 송출하는 기능 담당 (모든 클라이언트가 가지고 있는 액터)

Player

ㄴPlayerController 

ㄴPlayerState

UI

ㄴChatInput : 채팅과 정답을 입력할 수 있는 UI

ㄴNotification : 전체공지 UI

ㄴMemorySupport : 게임을 돕는 UI - 쓴 숫자 지우기

---

## 1\. bulid 파일에 UI 관련 모듈 추가 및 UI 제작

### 1-1 빌드에 모듈넣기

[##_Image|kage@c3KCDZ/dJMcabjcxq3/AAAAAAAAAAAAAAAAAAAAAJCKUX-imvB81csTQl9Wh00k6E1aApGqrvv3yrvk_TT0/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=qG9T%2BHk3Wcm6Vbqy3Xz%2BextYpDc%3D|CDM|1.3|{"originWidth":827,"originHeight":472,"style":"alignCenter","caption":"bulid.cs에 UI관련 모듈 추가"}_##]

### 1-2 UI 제작 및 바인딩

[##_Image|kage@bbYvPq/dJMcabQZ8c2/AAAAAAAAAAAAAAAAAAAAADXPbweQfw5jo23tbcGcx5LB2q9_OBtTQ7xBCjw1hXEO/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=OTjJg2N%2Bbo6sbcH1CAQGh%2BTX9Fc%3D|CDM|1.3|{"originWidth":542,"originHeight":277,"style":"alignCenter"}_##]

[##_Image|kage@JVKn2/dJMcaaxOx5a/AAAAAAAAAAAAAAAAAAAAANy7vDsmtxV1EA9VKh7BMwLfhjsr4e27WaUdRLUAgS8_/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=iWCOGyHpXr3fgXqu1FE4Y4MpuUw%3D|CDM|1.3|{"originWidth":475,"originHeight":158,"style":"alignCenter","caption":"두개를 나눳다."}_##]

[##_Image|kage@bAeUZU/dJMcafMF52O/AAAAAAAAAAAAAAAAAAAAAPdsxBy_pgSYbdQ0qQ-izpPQfMdaNq4Qw45bYstSBL_m/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=XY4ZYnLis%2BEx37e%2BP1i3X%2BDZAu4%3D|CDM|1.3|{"originWidth":1033,"originHeight":643,"style":"alignCenter","caption":"UI Cpp 부분"}_##]

두개가 있으니 두개를 델리게이트 바인딩.

[##_Image|kage@lUgA7/dJMcagkth8z/AAAAAAAAAAAAAAAAAAAAAE2rXn7HjXt9dJEibevt4zbsljdg0Nvy5zRS5z6Pw9RI/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=YMVE02Q51IUVlNLutXHxiXoxM%2B8%3D|CDM|1.3|{"originWidth":988,"originHeight":934,"style":"alignCenter","caption":"UI Cpp 부분"}_##][##_Image|kage@pMoNY/dJMcabKhlsa/AAAAAAAAAAAAAAAAAAAAACbz3QaZVykT-yMnwEvC3lzOvgnoINbseRWo-bXIXlR2/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=5ec7aoyj5As5YHmd9qxDeQ3VyPQ%3D|CDM|1.3|{"originWidth":1158,"originHeight":891,"style":"alignCenter","caption":"PlayerController cpp 부분"}_##]

---

## 2\. 채팅 구현

### 2-1 RPC

PlayerController에 RPC 선언 및 구현

채팅 쏴주는 기능 구현

먼저 강의에서 만든 함수라이브러리 추가!

더보기

```
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class KHBBFunctionLibrary
{

public:
	static void MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
	{
		if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString); //데디케이티드 서버를 위한 출력함수
			}
		}
	}

	//해당 월드의 NetMode 출력 
	static FString GetNetModeString(const AActor* InWorldContextActor)
	{
		FString NetModeString = TEXT("None");


		if (IsValid(InWorldContextActor) == true)
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("StandAlone");
				}
				else
				{
					NetModeString = TEXT("Server");
				}
			}
		}

		return NetModeString;
	}

	//해당 액터의 NetRole 출력
	static FString GetRoleString(const AActor* InActor)
	{
		FString RoleString = TEXT("None");

		if (IsValid(InActor) == true)
		{
			//Enum값을 String으로 바꿔주는 함수
			FString LocalRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetLocalRole());
			FString RemoteRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetRemoteRole());

			RoleString = FString::Printf(TEXT("%s / %s"), *LocalRoleString, *RemoteRoleString);
		}

		return RoleString;
	}

};
```

```
ClientRPCPrintMessageString
```

클라이언트에서 실행될 Print함수.

그냥 위에서 만든 MyPrint 해주기!

```
ServerRPCPrintMessageString
```

서버에서 실행될 Print함수.

모든 클라이언트에게 Print 해주기. Muticast말고 하나하나 보내야한다. (PlayerController는 각자꺼만 가지고 있어서)

**GetAllActorsOfClass : 담아주는거**

**for(TActorIterator<검사할클래스> It(GetWorld()) ; It ; ++It) :바로바로 검사하는거**

아오 함수이름 외우기 어렵네 숭악한 영미권놈들

UI에서 String입력 -> SetChattingMessage 호출! -> ServerRPC 호출! -> Server에서 ServerRPC 실행 -> ClientPrint 호출! -> Client에서 ClientRPC 실행! -> PrintChatMessage 호출! -> 글자 프린트

[##_Image|kage@tAQ5L/dJMcafFTcW6/AAAAAAAAAAAAAAAAAAAAAAjbsAU9udioiyoGzE5vcbBPCHkF1xDRRmlb2FaXcqpA/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=x81%2Bkhc37XbpQmCeHeIU%2BGjyp9g%3D|CDM|1.3|{"originWidth":1200,"originHeight":589,"style":"alignCenter","caption":"잘 된 다 !"}_##]

---

### 2-2 플레이어 이름 출력

PlayerState에 이름 등록!

Replicated 설정을 해준다

```
UPROPERTY(Replicated)
FString PlayerName;
```

SetLifeTimeRe... 뭐엿지

```
GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)
```

에 등록해준다!

GameModeBase

이제 GameModeBase에서 OnPostLogin 함수에서 들어온 순서대로 이름을 등록해준다!

PlayerController를 통해서 PlayerState에 접근!

PlayerState의 PlayerName을 수정해준다~

PlayerController

SetChattingMessage에서 PlayerState의 PlayerName을 가져와서 붙여준다

[##_Image|kage@bbH0ST/dJMcacJba0w/AAAAAAAAAAAAAAAAAAAAAFsRwhdGZWfhORWIDrR8o923WXioHAtcrNdt-O2jNtv7/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=iX6wQrG6XOkZJ5N8bZ9pKb4oJSY%3D|CDM|1.3|{"originWidth":753,"originHeight":563,"style":"alignCenter","caption":"잘 된 다!"}_##]

---

## 3\. 랜덤 번호 생성

채팅은 끝났고 본게임에 들어갈 시간이다.

## 3\. 랜덤 번호 생성

```
FString AKHBBGameModeBase::GenerateRandomNumberString()
```

아이디어

1~9까지 들어있는 상자에서 세개를 뽑아  FString에 넣는다.

사용할 변수들

```
TArray<int32> NumberContainer;
FString SecretNumber;
FMath::RandInit(FDateTime::Now().GetTicks()); //현재 시간을 시드로 사용
```

### 3-1 숫자 상자 만들기

```
for (int i = 1; i <= 9; i++)
{
    NumberContainer.Add(i);
}
```

### 3-2 뽑아 넣기

```
//숫자 컨테이너에서 세개 뽑아서 넣기
for (int i = 0 ; i < 2 ; i++ )
{
    int32 index = FMath::RandRange(0,NumberContainer.Num()-1);
    int32 RandomNum = NumberContainer[index];
    SecretNumber.Append(FString::FromInt(RandomNum));
    NumberContainer.RemoveAt(index);
}
```

여기서는 특정 인덱스를 지우는 RemoveAt(int32 index)를 사용했다.

그 외의 Remove들

더보기

-   **RemoveSingle(const ElementType& Item)** 일치하는 요소 중 첫 번째로 찾은 것 딱 하나만 지웁니다. 중복된 값이 있어도 하나만 지우고 싶을 때 사용합니다.
-   **RemoveSwap(const ElementType& Item)** 요소를 지운 후 뒤에 있는 요소들을 당겨오는 대신, 배열의 맨 마지막 요소를 지운 빈자리에 덮어씌웁니다. 배열의 순서가 뒤섞여도 상관없는 상황이라면 메모리 이동 비용이 없어 성능상 훨씬 유리합니다.
-   **RemoveAt(int32 Index)** 값이 아니라 \*\*특정 인덱스(위치)\*\*에 있는 요소를 지울 때 사용합니다.
-   **RemoveAll(Predicate)** 특정 값이 아니라 \*\*람다식(Lambda)\*\*을 이용해 원하는 조건에 맞는 요소들을 한 번에 싹 지울 때 매우 유용합니다.

---

## 4\. 정답 입력 받기 및 판단 로직

따로 만들어둔 정답창을 이용

UI -> Clinet에서 ServerRPC -> Server(Gamemodebase)  판단로직 -> Server에서 ClientRPC로 전파

Sever에서 할 일 

1\. Client에서 Answer칸에 입력된 문자열 받기

2\. 입력된 문자열이 세자리의 각자리 가 중복없는 숫자인지 확인

3\. 입력받은 숫자와 서버에 저장된 SecretNum 비교 후 결과

4\. 결과를 모든 플레이어 에게 전파하기

(5. 힌트창 갱신하기 PlayerState)

### 4-1  입력된 문자열이 세자리의 각자리 가 중복없는 숫자인지 확인

```
bool AKHBBGameModeBase::IsGuessNumberString(const FString& InNumberString)
```

1\. 비어있다면 컷

2\. 길이 3 아니면 컷

3\. 문자있거나 0 있으면 컷

4\. 중복 있으면 컷

살아남으면 판정로직을 돌릴 수 있음

\*TCHAR와 FChar의 차이

TCHAR : 우리가 쓰는 데이터 자료형(타입) ~char

FChar : 문자열에 관한 함수 라이브러리

더보기

### 1\. TCHAR (데이터 타입)

-   **역할:** 문자 하나를 저장하는 \*\*자료형(Data Type)\*\*입니다. C++의 기본 char나 wchar\_t와 같은 역할을 합니다.
-   **왜 쓸까요?:** 언리얼 엔진은 윈도우, 맥, 모바일 등 다양한 플랫폼에서 돌아가야 합니다. 플랫폼마다 문자를 처리하는 용량(1바이트, 2바이트 등)이 다른데, 언리얼 엔진이 환경에 맞게 자동으로 최적의 문자 타입으로 변환해 주도록 만든 일종의 '스마트한 문자형'입니다.
-   **사용 예시:**
-   C++
    
    ```
    TCHAR MyCharacter = TEXT('A'); // 글자 하나를 변수에 담습니다.
    ```
    
-   **참고:** 언리얼의 문자열인 FString은 사실 내부적으로 이 TCHAR들이 일렬로 모여있는 배열(Array)입니다.

### 2\. FChar (유틸리티 도구함)

-   **역할:** TCHAR에 담긴 문자가 숫자인지, 알파벳인지, 공백인지 판별하거나 대소문자를 변환해 주는 기능들을 모아둔 \*\*유틸리티 클래스(Utility Class)\*\*입니다.
-   **특징:** 스스로 데이터를 담을 수는 없고, 오직 static 함수들만 제공합니다.
-   **유용한 함수들:**
    -   FChar::IsDigit(C) : 숫자인가요? (0~9)
    -   FChar::IsAlpha(C) : 알파벳인가요? (a~z, A~Z)
    -   FChar::IsWhitespace(C) : 띄어쓰기나 줄바꿈인가요?
    -   FChar::ToUpper(C) : 대문자로 바꿔주세요!

---

### 4-2 판정 로직

```
FString ACXGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
```

스트라이크 카운트와 볼카운트를 이용해 판정

1\. 위치도 같고 (인덱스일치) 값도 같다면(TCHAR일치) 

\-> StrikeCount 증가

2\. 그게 아니라면(else)

\-> SecretNumber에 내가 뽑은 수가 들어있다면 볼!

---

## 5\. 흐름

UI에서 입력을 받음

\-> 1. 정답칸에 입력받음 -> PlayerController에서 ServerRPC호출 입력넘겨줌 -> Server에서 판단로직실행 -> 플레이어 정보와 함께 붙여서 Client에게 String 쏴줌.

\->2. 채팅칸에 입력받음 -> PalyerController에서 플레이어 이름 붙여서 ServerRPC호출 후 넘겨줌 -> Server에서 판단 로직 실행 하지만 플레이어 이름이 추가되었기 때문에 항상 false가 나옴-> 그냥 채팅으로 Client에게 줌

```
[ UI 입력 발생 ]
  │
  ├──▶ 1. 정답칸 입력
  │     └──▶ [PlayerController]: ServerRPC 호출 (입력한 숫자 전달)
  │          └──▶ [Server]: 정답 판별 로직 실행 (Strike/Ball 계산)
  │               └──▶ [Server]: "플레이어 이름 + 판별 결과"를 조립하여 Client들에게 전송 (ClientRPC)
  │
  └──▶ 2. 채팅칸 입력
        └──▶ [PlayerController]: 입력값에 "플레이어 이름"을 강제로 붙여서 ServerRPC 호출
             └──▶ [Server]: 정답 판별 로직 실행 
                  (이름이 섞여 있어 숫자가 아니므로 무조건 'False' 판정 발생)
                  └──▶ [Server]: 정답이 아니므로 일반 채팅 메시지로 취급하여 Client들에게 전송
```

지금은 하나의 함수

```
void AKHBBGameModeBase::PrintMessageToClient(const AKHBBPlayerController* TargetPlayers, const FString& MessageString)
```

로 클라이언트에게 전송하지만

요기서 좀더 안전하게 하려면 정답 함수와 채팅 함수를 나누어서 만들어야하지만 귀찮으니 패수 ㅎㅎ;

---

## 6\. 플레이어 횟수 제한

플레이어 스테이트에 최대 횟수와 현재 횟수 변수 만들고 Replicated

[##_Image|kage@cdFCjS/dJMcach6V4C/AAAAAAAAAAAAAAAAAAAAAHT_4Zrz0p0WYsV8sXRSofkLJsr90WvenxS2Azubfrr-/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=JPdU1fkuuKBPjh67vmuTOoCklWk%3D|CDM|1.3|{"originWidth":1252,"originHeight":457,"style":"alignCenter"}_##]

이걸 게임모드 베이스에서 사용할 예정이다.

[##_Image|kage@eweVBv/dJMcahwXHMp/AAAAAAAAAAAAAAAAAAAAAH7iZxvpjnoEiP3sUqTPT-TwXgusxH9wl0I4vdvIXDXR/img.png?credential=yqXZFxpELC7KVnFOS48ylbz2pIh7yKj8&amp;expires=1774969199&amp;allow_ip=&amp;allow_referer=&amp;signature=axR%2BLu5Vb%2FZUrbV7RCqpk01XYao%3D|CDM|1.3|{"originWidth":1187,"originHeight":105,"style":"alignCenter","caption":"결국 두개로 나눳다"}_##]

```
FString PlayerInfo = KHBBPlayerState->GetPlayerInfoString();
```

```
FString JudgeResultString = JudgeResult(SecretNumberString, MessageString);
```

```
FString AnswerJudge = PlayerInfo + MessageString + JudgeResultString;
```

이렇게 합쳐서 전송!

OutGeussCount 함수로 

---

## 7\. 게임 결과 판정

Strike Count가 3인 플레이어가 승리하게 하도록 만든다.

JudgeResult함수 내에서 호출할 수 있도록 만들기.

PlayerState에 bIsWin 변수를 추가

JudgeResult함수 흐름

매개변수로 입력한 플레이어와, bool biswin 을 받는다.

judgeresult에서 3strike를 하면 bIswin을 true로 하고 승리로직을 실행

승리로직 

모든 플레이어에게 특정 플레이어가 이겼다고 Print 

Notification UI에서 PlayerController의 NotificationText 변수와 UI에서 Notification의 변수를 틱마다 동일화 시켰다.

PlayerController 변수는 Replicated 되어있어서 서버에서 변경시 변화를 받을 수 있다.
