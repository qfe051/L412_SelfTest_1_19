# Git Branch & Version Policy

## Version
- **v260105**

---

## Branch Policy

### 1. main 브랜치
- **새로운 프로젝트 시작 시 업로드**
- 안정화된 코드만 유지
- 배포 및 기준(reference) 브랜치

main
└─ 신규 프로젝트 초기 코드
---

### 2. develop 브랜치
- **개발 중인 프로젝트 업로드**
- 기능 추가, 수정, 테스트 작업 진행
- main 병합 전 작업 공간

develop
└─ 기능 개발 / 테스트 / 실험 코드
---

## Workflow Summary

1. 새로운 프로젝트 생성
   - `main` 브랜치에 초기 코드 업로드

2. 개발 진행
   - `develop` 브랜치에서 작업
   - 기능 추가 및 수정 반복

3. 안정화 후
   - 필요 시 `develop` → `main` 병합

---

## Notes
- `main` 브랜치는 항상 **빌드 가능 상태 유지**
- 실험적이거나 불안정한 코드는 `develop`에서만 관리
