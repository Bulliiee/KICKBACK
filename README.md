# C209 README.md

## 📝목차
1️⃣ [Description](#1️⃣-description)<br/>
2️⃣ [Game Mod](#2️⃣-game-mod)<br/>
3️⃣ [System Architecture](#3️⃣-system-architecture)<br/>
4️⃣ [Convention](#4️⃣-convention)<br/>

## 🏁 Kick Back
![alt text](./Image/image.png)

## 1️⃣ Description
카트를 이용한 다양한 게임 모드들을 준비하였고 각 모드에 따라 특색있는 요소들이 존재하며 팀원 혹은 개인의 승리를 목표로 하는 레이싱 게임입니다.

## 2️⃣ Game Mod

### 🍌 아이템모드
![alt text](./Images/image-1.png)
맵에 존재하는 장애물들을 피하고, 다양한 아이템을 이용하여 결승점에 먼저 도달하는 것이 목표인 게임 모드입니다.

### ⚽ 축구
![alt text](./Images/image-2.png)
축구 경기장에서 카트를 타고 축구를 진행하는 모드입니다.

### 🚥 스피드전
![alt text](./Images/image-3.png)
다른 모드들과 다르게 맵에 장애물과 아이템이 존재하지 않고 오로지 실력만으로 순위를 판가름할 수 있는 모드입니다.

## 3️⃣ System Architecture
![alt text](./Images/image-4.png)

## 4️⃣ Convention
![alt text](./Images/image-5.png)

### 브랜치 종류
- master: 최종 배포 브랜치
- develop: 다음 출시 버전을 개발하는 브랜치 (merge 대상 브랜치)
- test: CI/CD 및 기타 테스트용 브랜치

### 커밋 브랜치
- 개인 브랜치(영문 이름) -> Merge Request -> develop 브랜치 -> 개발완료 -> master 브랜치

### 팀 커밋 컨벤션
- <타입>[적용범위]: <설명>
  - ex) ssafy브랜치에 Feat[Common]: README.md udpate
- 적용범위
  1. Common: 공통 작업
  2. Live: 라이브 서버
  3. Business: 비즈니스 서버
  4. Auth: 웹 및 인증 서버
  5. Chat: 채팅 서버
  6. Web: 웹 프론트엔드
  7. Unity: 유니티 