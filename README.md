# PARADISE

Unreal Engine 5 C++ 기반 멀티플레이 서바이벌 액션 포트폴리오 프로젝트입니다.  
로비에서 매치를 생성/참가하고, 게임맵에서 전투-소리 어그로 AI-탈출 루프를 플레이하는 구조를 목표로 구현했습니다.

## 프로젝트 개요

- **장르:** 3D 멀티플레이 서바이벌 액션 (PvE 중심)
- **엔진:** Unreal Engine 5 (C++)
- **핵심 루프:** Main Menu -> Lobby -> In-Game Combat -> Escape -> Lobby Return
- **개발 포인트:** C++ 기반 시스템 설계 + Blueprint 연동 + 멀티플레이 동기화

## 담당 구현 (핵심)

- 멀티플레이 기본 세팅 (Listen Server, 다중 PIE 테스트)
- 캐릭터 이동/점프/스프린트 입력 처리 및 애니메이션 연동
- 무기 베이스/근접/원거리 구조 및 장착 시스템
- GAS 기반 ASC/AttributeSet/GameplayEffect 기본 적용 흐름
- Enemy AIController + Behavior Tree + Blackboard + Perception(청각) 기반 추격/공격 루프
- 월드 인터랙션(문/차량 타격 소리, 무기 히트 리액션 인터페이스) 구현
- 로비/상점 UI 동선 및 DataTable 기반 상점 엔트리 자동 생성
- 탈출 인터랙션 및 로비 복귀 흐름 구현

## 주요 기능

### 1) 로비/매치 흐름
- Main Menu에서 로비 진입
- 로비에서 방 생성(`listen`) / 주소 기반 참가(`ClientTravel`)
- 상점 열기/닫기/복귀 UI 플로우 연결

### 2) 전투 시스템
- 근접/원거리 공격 구조 분리
- 서버 권한 기준 데미지 적용 흐름 구성
- 공격 몽타주 재생과 트레이스 연동

### 3) AI 전투/추격
- 소리 이벤트 기반 타겟 탐지(청각 Perception)
- 거리 기반 `ShouldAttack` 상태 갱신
- Behavior Tree에서 추격/공격 전환

### 4) 월드 반응형 오브젝트
- 문/차량 타격 시 소리 이벤트 발생
- 소리 이벤트가 AI 어그로로 이어지는 연쇄 동작
- 공통 인터페이스(`IParadiseWeaponHitReactable`)로 히트 반응 통합

## 트러블슈팅 사례

### 1) 원격 클라이언트 스프린트 러버밴딩
- **문제:** 호스트는 정상인데 클라이언트 2/3에서 달리기 시 걷기로 보정
- **원인:** `MaxWalkSpeed`를 클라이언트 로컬에서만 변경
- **해결:** 서버 RPC(`ServerSetRunning`)로 서버 권한에서 속도 변경

### 2) AI 행동 충돌(공격/이동 전환 불안정)
- **문제:** AI가 공격/추격 사이에서 부자연스럽게 전환
- **원인:** 서비스에서 이동 명령(`MoveToActor`/`StopMovement`) 과호출
- **해결:** 이동 상태를 확인해 불필요한 재호출 제한, BT 조건 분리(`ShouldAttack`)

## 현재 상태

### 완료
- 로비 진입/방 생성/참가
- 기본 전투 루프(입력/공격 재생/데미지 적용 경로)
- AI 청각 탐지 및 추격/공격 전환 기반
- 탈출 인터랙션 및 로비 복귀

### 진행 중/보완 예정
- 전투 판정 폴리싱(근접/원거리 디테일)
- 멀티 환경 AI 동기화 검증 강화
- 결과/보상 UI 및 저장 루프
- 상점 구매 로직(RPC/재화 처리)

## 실행 및 테스트

1. UE5에서 프로젝트 열기
2. PIE를 멀티플레이 모드(Listen + Clients)로 실행
3. Main Menu -> Lobby -> Create/Join -> In-Game 진입
4. 전투/AI 반응/탈출/복귀 흐름 확인

## 기술 스택

- **Engine:** Unreal Engine 5
- **Language:** C++
- **Gameplay:** GAS, AI Perception, Behavior Tree, Blackboard
- **UI:** UMG + C++ Widget 로직
- **Network:** UE 멀티플레이 RPC/Replication

## 참고

이 README는 포트폴리오 제출을 위한 요약 문서입니다.  
세부 구현은 `Paradise/Source/Paradise` 하위 C++ 코드와 BP 에셋 설정을 함께 확인해야 합니다.
