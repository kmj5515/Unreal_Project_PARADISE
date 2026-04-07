# Development Roadmap (Portfolio Flow)

**최소 범위(시간 부족 시):** 파쿠르는 보류. 로비/매치 → 게임맵 전투·좀비·탈출 → **보상**까지 우선.

## 0) 현재 완료된 기반
- [x] UE5 C++ 프로젝트/소스 세팅, Git 연동
- [x] 멀티플레이 기본 세팅 (Listen 서버, 다중 PIE)
- [x] 기본 캐릭터 이동/점프/스프린트 + 애니메이션 연동
- [x] 무기 베이스/근접/원거리 클래스 및 장착 시스템
- [x] GAS 모듈 연결 + 기본 ASC/AttributeSet/GE 적용 흐름
- [ ] 파쿠르(점프키 베이스): **보류** — 시간 나면 재개
- [x] 로비 맵 제작
- [x] 메인메뉴 카메라 -> 게임시작 버튼 -> 대기실 카메라 전환

---

## 1) 로비/매치 진입 루프 (변경된 기획 기준)
목표: 데모 시작 30초 안에 "메인메뉴 -> 로비(생성/참가) -> 상점 확인 -> 게임맵 진입"이 보이게.

- [x] 메인메뉴 `GameStart` 클릭 시 로비 레벨 이동 (`OpenLevel`)
- [x] 로비에서 `CreateGameButton`으로 게임맵 호스팅 진입 (`OpenLevel(..., listen)`)
- [x] 로비에서 `JoinGameButton`으로 주소 입력 기반 참가 (`ClientTravel`)
- [x] 로비에서 상점 열기/닫기/복귀 동선 연결 (`OpenShopButton`, `CloseShopButton`, `BackToLobbyButton`)
- [x] 상점 실행 시 DT 기반 아이템 엔트리 자동 생성 (`RefreshShopItems`)

---

## 2) 게임맵 코어 플레이 (전투 최소)
목표: 게임 맵에 들어가면 "이동/공격"이 즉시 동작. (파쿠르 제외)

- [ ] 근접 공격 히트 판정 및 데미지 연동 최종 검증
- [ ] 원거리 공격 히트 판정 및 데미지 연동 최종 검증
- [x] HUD 최소 버전: 상호작용 프롬프트 위젯 표시 (C++ 구현, 위젯 BP 지정 필요)
- [ ] HUD 최소 버전: 체력 표시

---

## 3) 좀비 AI (소리 반응 + 추격/공격)
목표: 총/문/차 소리에 좀비가 반응하는 PvE 체감 확보.

- **초기 범위:** 평지 + NavMesh 기준 추격만 (파쿠르/비정형 지형 제외)
- [x] 좀비 AIController + Behavior Tree 기본 루프 (대기 → 탐지 → 추격 → 공격) — C++/BT 연동
- [x] Perception(청각) + 소음 이벤트 → 플레이어 타겟 추적
- [ ] 멀티 환경에서 좀비 타겟/추격/공격 동기화 최종 검증

---

## 4) 월드 인터랙션 목표 기능
목표: 맵 오브젝트가 게임플레이를 만든다는 인상을 주기.

- [x] 자동차 공격 시 사이렌 ON (C++: `AParadiseBrokenCarHitReactActor`, 사운드 에셋 지정 필요)
- [x] 문 타격 시 소리 이벤트 발생 (C++: `AParadiseDoorInteractableActor::HitSound`, 사운드 에셋 지정 필요)
- [x] 무기 히트 수신 공통 인터페이스 (`IParadiseWeaponHitReactable` — 맵 액터에서 `ReactToWeaponHit` 구현)
- [x] Enemy 타격 시 사운드 재생 (C++: `AParadiseEnemyCharacter::WeaponHitSound`, 사운드 에셋 지정 필요)
- [x] 소리 → 좀비 청각(Perception) 어그로 (노이즈 이벤트 + 타겟 필터)

---

## 5) 탈출/복귀/보상 루프 (최소에서도 우선)
목표: 한 판의 시작-진행-종료 + **보상**이 보이게.

- [x] 게임맵 탈출 트리거(탈출구 상호작용) 구현
- [x] 탈출 시 로비로 복귀 처리 (현재: `ClientTravel` 기반)
- [ ] 결과/보상 UI 최소 (성공 여부 + 획득량 표시)
- [ ] 보상 데이터 저장(임시: PlayerState/GameInstance 등)

---

## 6) 데모 우선순위 (시간 없을 때 최소)
파쿠르 없이 아래만 맞추면 시연 가능.

1. 로비에서 시작 → 게임맵 진입
2. 게임맵에서 이동 + 근접/원거리 전투만 안정화
3. 좀비 소리 반응·추격·공격 (이미 대부분 연결됨 → 폴리싱/멀티 검증)
4. 문/맵 오브젝트 소리 → 좀비 반응
5. 탈출 → (가능하면) 로비 복귀 → **보상 UI + 저장 최소**

===

## 7) UI 구현 순서 (로딩 UI 포함)
목표: MainMenu -> Lobby -> 지역 선택 -> 로딩 -> 인게임 -> 결과/보상까지 UI 동선 완성.

1. MainMenu UI
   - [x] `GameStart` 버튼 클릭 시 로비 레벨 이동
   - [x] (옵션) 종료 버튼
2. Lobby UI
   - [ ] `탐색` 버튼으로 지도 UI 오픈
   - [ ] `진행` 버튼 기본 비활성, 지역 선택 시 활성
   - [ ] 방장만 진행 가능하도록 권한 표시/처리

---

## 8) Lobby Shop 구현 현황 (C++ + BP 세팅)
목표: 로비에서 상점 오픈 시 DT 기반으로 아이템 목록 자동 생성.

- [x] `ULobbyUserWidget`에서 상점 오픈 버튼 클릭 시 Shop 위젯 오픈 (`AddToViewport`)
- [x] `ULobbyShopUserWidget` 닫기/로비 복귀 버튼 연결 (`CloseShopButton` / `BackToLobbyButton`)
- [x] 데이터 행 구조 추가: `FShopItemDataRow`
  - 필드: `ItemID`, `ItemName`, `Price`, `Icon`, `Description`
- [x] 엔트리 위젯 클래스 추가: `ULobbyShopItemEntryWidget`
  - 바인딩 이름: `IconImage`, `ItemNameText`, `PriceText`, `DescriptionText`
- [x] Shop 실행 시 DT에서 아이템 읽고 엔트리 자동 생성 (`RefreshShopItems`)
- [ ] 구매 로직(소지금 체크/구매 처리/RPC) 구현
