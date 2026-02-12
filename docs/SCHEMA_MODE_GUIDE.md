# 스키마 모드 설정 가이드

## 📋 개요

API Verification Platform에 **스키마 모드** 설정이 추가되었습니다. 사용자는 **개선 모드**와 **일반 모드** 중 선택할 수 있습니다.

**작업 일자**: 2026-01-16  
**버전**: 2.1

---

## 🎯 스키마 모드 종류

### 1️⃣ 개선 모드 (Enhanced Mode) - 기본값

**특징**:
- ✅ Original/Enhanced 2개 탭 제공
- ✅ YAML 파일 2개 로드: `original.yaml`, `enhanced.yaml`
- ✅ 스키마 타입 간 전환 및 비교 가능
- ✅ 고급 사용자를 위한 세밀한 제어

**UI**:
```
[Original] [Enhanced]
```

**YAML 로딩**:
- SpecTab: `schema_definitions/{psd_set}/table/original.yaml` + `enhanced.yaml`
- BuilderTab: `schema_definitions/{psd_set}/builder/original.yaml` + `enhanced.yaml`

---

### 2️⃣ 일반 모드 (Normal Mode)

**특징**:
- ✅ 단일 스키마만 표시 (탭 없음)
- ✅ YAML 파일 1개만 로드: `definition.yaml`
- ✅ 심플한 UI로 빠른 작업
- ✅ 일반 사용자를 위한 단순화된 인터페이스

**UI**:
```
Schema Editor
(탭 없음, 단일 뷰)
```

**YAML 로딩**:
- SpecTab: `schema_definitions/{psd_set}/table/definition.yaml`
- BuilderTab: `schema_definitions/{psd_set}/builder/definition.yaml`

---

## ⚙️ 설정 방법

### Settings Modal에서 변경

1. 우측 상단 **⚙️ Settings** 클릭
2. **스키마 모드** 드롭다운 선택:
   - `개선 모드 (Original/Enhanced 2탭)` ← 기본값
   - `일반 모드 (단일 스키마)`
3. **Save Settings** 클릭

### localStorage에 저장

```typescript
// Settings 객체
{
  baseUrl: string;
  mapiKey: string;
  commonHeaders: string;
  useAssignWrapper?: boolean;
  schemaMode?: 'enhanced' | 'normal'; // 🔥 NEW
}
```

```javascript
// 기본값
const defaultSettings = {
  ...
  schemaMode: 'enhanced' // 개선 모드가 기본값
};
```

---

## 📁 YAML 파일 구조

### 개선 모드 (Enhanced Mode)

```
schema_definitions/
└── civil_gen_definition/
    ├── table/
    │   ├── original.yaml      ← Original 탭
    │   └── enhanced.yaml      ← Enhanced 탭
    └── builder/
        ├── original.yaml      ← Original 스키마용
        └── enhanced.yaml      ← Enhanced 스키마용
```

### 일반 모드 (Normal Mode)

```
schema_definitions/
└── civil_gen_definition/
    ├── table/
    │   └── definition.yaml    ← 단일 파일
    └── builder/
        └── definition.yaml    ← 단일 파일
```

---

## 🔧 구현 상세

### 1. SpecTab 변경사항

**탭 표시/숨김**:
```typescript
// 개선 모드: 탭 표시
{settings?.schemaMode !== 'normal' && (
  <div>
    <button onClick={() => setSchemaView('original')}>Original</button>
    <button onClick={() => setSchemaView('enhanced')}>Enhanced</button>
  </div>
)}

// 일반 모드: 제목만 표시
{settings?.schemaMode === 'normal' && (
  <div>Schema Editor</div>
)}
```

**YAML 로딩**:
```typescript
const schemaTypeToLoad = settings?.schemaMode === 'normal' 
  ? 'definition'  // 일반 모드
  : schemaView === 'original' ? 'original' : 'enhanced'; // 개선 모드

loadCachedDefinition(psdSet, schemaTypeToLoad, 'table');
```

### 2. BuilderTab 변경사항

**YAML 로딩**:
```typescript
const schemaTypeToLoad = settings?.schemaMode === 'normal'
  ? 'definition' // 일반 모드
  : isNewEnhancedSchema ? 'enhanced' : 'original'; // 개선 모드

loadCachedDefinition(psdSet, schemaTypeToLoad, 'builder');
```

---

## 🎨 UI 차이점

### 개선 모드
```
┌─────────────────────────────────────┐
│ Schema View:                        │
│ [Original] [Enhanced]  (설명 텍스트) │
└─────────────────────────────────────┘
│ Schema Editor (Original)            │
│ - JSON Schema 편집                  │
│ - Visual Table                      │
└─────────────────────────────────────┘
```

### 일반 모드
```
┌─────────────────────────────────────┐
│ Schema Editor                       │
└─────────────────────────────────────┘
│ Schema Editor                       │
│ - JSON Schema 편집                  │
│ - Visual Table                      │
└─────────────────────────────────────┘
```

---

## 🚀 사용 시나리오

### 개선 모드 추천 대상
- 🎯 API 개발자
- 🎯 스키마 설계자
- 🎯 Original과 Enhanced 비교가 필요한 경우
- 🎯 고급 기능 (x-ui, x-transport) 사용

### 일반 모드 추천 대상
- 👤 일반 사용자
- 👤 빠른 테스트가 필요한 경우
- 👤 스키마 종류에 관심 없는 경우
- 👤 심플한 인터페이스 선호

---

## 📝 마이그레이션 가이드

### 기존 YAML 파일이 있는 경우

#### 방법 1: 개선 모드 유지 (권장)
```bash
# 기존 파일이 있다면 이름 변경
cd schema_definitions/civil_gen_definition/table/
cp definition.yaml original.yaml
cp definition.yaml enhanced.yaml  # 또는 별도 enhanced.yaml 작성
```

#### 방법 2: 일반 모드로 전환
```bash
# Settings에서 "일반 모드" 선택
# definition.yaml 파일만 유지
```

---

## 🔍 디버깅

### 로딩되는 YAML 확인

브라우저 콘솔에서:
```
🔄 Loading YAML definition: civil_gen_definition/definition (mode: normal)
✅ Initialized schema logic rules for civil_gen_definition/definition
✅ Loaded table definition from civil_gen_definition/definition
```

또는

```
🔄 Loading YAML definition: civil_gen_definition/original (mode: enhanced)
✅ Initialized schema logic rules for civil_gen_definition/original
✅ Loaded table definition from civil_gen_definition/original
```

---

## ⚠️ 주의사항

1. **YAML 파일 존재 확인**
   - 일반 모드: `definition.yaml` 필수
   - 개선 모드: `original.yaml`, `enhanced.yaml` 필수

2. **스키마 로직 규칙**
   - 각 모드에 맞는 YAML 파일에 적절한 규칙 정의 필요

3. **하위 호환성**
   - 기존 코드는 개선 모드가 기본값이므로 영향 없음
   - 일반 모드 사용 시에만 `definition.yaml` 필요

---

## 📚 관련 문서

- [Settings Guide](./API_SETTINGS.md)
- [Schema Definition Guide](./SCHEMA_DEFINITIONS.md)
- [YAML Configuration](../schema_definitions/README.md)

---

**마지막 업데이트**: 2026-01-16
