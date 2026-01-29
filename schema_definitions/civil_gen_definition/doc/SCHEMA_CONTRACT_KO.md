# 📜 Enhanced Schema 작성 계약 규칙

> **이 문서는 Enhanced JSON Schema를 작성할 때 반드시 지켜야 하는 규칙입니다.**
> 
> 규칙을 위반하면 시스템(컴파일러, 빌더, 테이블)이 제대로 작동하지 않습니다!

---

## 🎯 핵심 원칙: Single Entity Schema

### 📌 규칙 1: 스키마는 **단일 엔티티**를 정의합니다

스키마는 **하나의 객체**(예: 하나의 Element, 하나의 Node)를 정의하며, **컬렉션이나 래퍼는 포함하지 않습니다.**

#### ❌ 잘못된 예시 (래퍼 포함)

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "BeamForceTable",
  "type": "object",
  "properties": {
    "Argument": {           // ❌ 스키마 안에 래퍼
      "type": "object",
      "properties": {
        "TABLE_NAME": { "type": "string" },
        "TABLE_TYPE": { "type": "string" }
      }
    }
  },
  "x-transport": {
    "body-root": "Argument"  // ❌ 중복 선언
  }
}
```

**문제점:**
- `Argument`가 스키마 내부에 이미 정의되어 있음
- `x-transport.body-root`로 또 `Argument`를 선언
- 시스템이 어느 것을 기준으로 처리해야 할지 모호함

#### ✅ 올바른 예시 (단일 엔티티)

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "BeamForceTable",
  "type": "object",
  "required": ["TABLE_TYPE"],
  "properties": {
    "TABLE_NAME": {         // ✅ 직접 최상위에
      "type": "string",
      "x-ui": {
        "label": "Table Name",
        "group": "General"
      }
    },
    "TABLE_TYPE": {         // ✅ 직접 최상위에
      "type": "string",
      "enum": ["BEAMFORCE", "BEAMFORCEVBM"],
      "x-ui": {
        "label": "Result Table Type",
        "group": "General"
      }
    }
  },
  "x-transport": {
    "body-root": "Argument"  // ✅ 전송 레이어에서만 래핑
  }
}
```

---

## 🚀 Transport vs Schema 분리

### 📌 규칙 2: 스키마와 전송 레이어는 별개입니다

**스키마가 정의하는 것:**
```json
{
  "TABLE_NAME": "My Table",
  "TABLE_TYPE": "BEAMFORCE"
}
```

**실제 HTTP 요청 구조:**
```json
{
  "Argument": {
    "TABLE_NAME": "My Table",
    "TABLE_TYPE": "BEAMFORCE"
  }
}
```

이 **래핑**은 `x-transport.body-root`로 처리됩니다:

```json
{
  "x-transport": {
    "uri": "post/TABLE",
    "methods": ["POST"],
    "body-root": "Argument"  // 시스템이 자동으로 래핑
  }
}
```

---

## 🔢 Multiple Instances (Assign Wrapper)

### 📌 규칙 3: 여러 인스턴스는 `x-transport`로 처리

#### 예시: ELEM API

**스키마는 하나의 Element만 정의:**
```json
{
  "title": "ELEM",
  "properties": {
    "TYPE": { "type": "string", "enum": ["BEAM", "WALL"] },
    "MATL": { "type": "integer" },
    "NODE": { "type": "array" }
  },
  "x-transport": {
    "body-root": "Assign"
  }
}
```

**실제 HTTP 요청 (여러 인스턴스):**
```json
{
  "Assign": {
    "1": { "TYPE": "BEAM", "MATL": 1, "NODE": [1, 2] },
    "2": { "TYPE": "WALL", "MATL": 1, "NODE": [3, 4, 5, 6] }
  }
}
```

**시스템이 자동으로:**
1. 스키마로 각 인스턴스(`"1"`, `"2"`) 검증
2. `x-transport.body-root`로 `Assign` 래퍼 추가
3. UI에서 인스턴스 관리 기능 제공

---

## 🏗️ 중첩 객체 처리

### 📌 규칙 4: 중첩 객체는 `type: "object"`로 정의

#### ✅ 올바른 중첩 구조

```json
{
  "properties": {
    "UNIT": {
      "type": "object",
      "description": "Response Unit Setting",
      "properties": {
        "FORCE": {
          "type": "string",
          "x-ui": { "label": "Force Unit" }
        },
        "DIST": {
          "type": "string",
          "x-ui": { "label": "Length Unit" }
        }
      },
      "x-ui": {
        "label": "Units",
        "group": "Unit"
      }
    }
  }
}
```

**JSON 결과:**
```json
{
  "UNIT": {
    "FORCE": "N",
    "DIST": "m"
  }
}
```

**HTTP 요청 (x-transport 적용):**
```json
{
  "Argument": {
    "UNIT": {
      "FORCE": "N",
      "DIST": "m"
    }
  }
}
```

---

## ❌ 흔한 실수와 해결 방법

### 실수 1: 래퍼를 스키마 안에 포함

```json
❌ 잘못됨:
{
  "properties": {
    "Assign": {  // 래퍼를 스키마에 포함
      "type": "object",
      "properties": {
        "TYPE": { "type": "string" }
      }
    }
  }
}

✅ 올바름:
{
  "properties": {
    "TYPE": { "type": "string" }  // 직접 정의
  },
  "x-transport": {
    "body-root": "Assign"  // 전송 레이어만 래핑
  }
}
```

### 실수 2: allOf에서 중첩 경로 사용

```json
❌ 잘못됨:
{
  "allOf": [
    {
      "if": {
        "properties": {
          "Argument": {
            "properties": {
              "OPT_CS": { "const": true }
            }
          }
        }
      },
      "then": {
        "required": ["Argument.STAGE_STEP"]  // 점 표기법
      }
    }
  ]
}

✅ 올바름:
{
  "allOf": [
    {
      "if": {
        "properties": {
          "OPT_CS": { "const": true }  // 직접 참조
        }
      },
      "then": {
        "required": ["STAGE_STEP"]  // 직접 참조
      }
    }
  ]
}
```

### 실수 3: required에 최상위 래퍼 포함

```json
❌ 잘못됨:
{
  "required": ["Argument"],
  "properties": {
    "Argument": {
      "required": ["TABLE_TYPE"],
      "properties": { ... }
    }
  }
}

✅ 올바름:
{
  "required": ["TABLE_TYPE"],
  "properties": {
    "TABLE_TYPE": { ... }
  },
  "x-transport": {
    "body-root": "Argument"
  }
}
```

---

## 🎨 x-* 확장 사용 규칙

### 📌 규칙 5: x-* 확장은 스키마 검증 외의 용도로만

#### ✅ 올바른 x-* 사용

```json
{
  "TYPE": {
    "type": "string",
    "enum": ["BEAM", "WALL"],  // ← 표준 JSON Schema 검증
    "x-ui": {                   // ← UI 렌더링 메타데이터
      "label": "Element Type",
      "group": "Common"
    }
  },
  
  "STYPE": {
    "type": "integer",
    "x-enum-by-type": {         // ← TYPE별 동적 enum (표준으로 표현 불가)
      "BEAM": [1, 2],
      "WALL": [1, 2]
    },
    "x-enum-labels-by-type": {  // ← UI 레이블
      "BEAM": { "1": "Fixed", "2": "Pinned" }
    }
  }
}
```

#### ❌ 잘못된 x-* 사용 (검증 로직을 x-*로만)

```json
❌ 잘못됨:
{
  "SECT": {
    "type": "integer",
    "x-required-for-types": ["BEAM", "WALL"]  // ❌ allOf로 해야 함
  }
}

✅ 올바름:
{
  "SECT": {
    "type": "integer",
    "x-ui": {
      "label": "Section No.",
      "visibleWhen": { "TYPE": ["BEAM", "WALL"] }
    }
  },
  "allOf": [
    {
      "if": {
        "properties": {
          "TYPE": { "enum": ["BEAM", "WALL"] }
        }
      },
      "then": {
        "required": ["SECT"]
      }
    }
  ]
}
```

---

## 📋 x-exclusive-keys (상호 배타적 입력)

### 📌 규칙 6: 여러 입력 방법 중 하나만 선택

#### 사용 예시

```json
{
  "NODE_ELEMS": {
    "type": "object",
    "description": "Node / Element Selection",
    "properties": {
      "KEYS": {
        "type": "array",
        "items": { "type": "integer" },
        "x-ui": { "label": "Specify Each ID" }
      },
      "TO": {
        "type": "string",
        "x-ui": { "label": "Specify ID Range" }
      },
      "STRUCTURE_GROUP_NAME": {
        "type": "string",
        "x-ui": { "label": "Structure Group Name" }
      }
    },
    "x-exclusive-keys": ["KEYS", "TO", "STRUCTURE_GROUP_NAME"],
    "x-ui": {
      "label": "Target Elements",
      "group": "Target"
    }
  }
}
```

**의미:**
- `KEYS`, `TO`, `STRUCTURE_GROUP_NAME` 중 **하나만** 입력 가능
- UI에서 라디오 버튼으로 표시
- 검증 레이어에서 한 개만 제공되었는지 확인

---

## 🔍 스키마 검증 체크리스트

### ✅ 스키마 작성 후 반드시 확인

- [ ] **최상위 properties가 실제 필드인가?** (래퍼 객체 없음)
- [ ] **x-transport.body-root가 올바른가?** (실제 API 엔드포인트 확인)
- [ ] **required 배열이 최상위에 있는가?** (중첩 안 됨)
- [ ] **allOf 조건이 최상위 필드를 참조하는가?** (점 표기법 없음)
- [ ] **x-ui가 UI 전용 메타데이터만 포함하는가?** (검증 로직 없음)
- [ ] **중첩 객체가 type: "object"로 정의되었는가?**
- [ ] **x-enum-by-type이 TYPE 필드를 참조하는가?**

---

## 📚 관련 문서

- **YAML 정의 방법**: [`YAML_GUIDE_KO.md`](./YAML_GUIDE_KO.md)
- **YAML 실전 예제**: [`YAML_PRACTICAL_EXAMPLES.md`](./YAML_PRACTICAL_EXAMPLES.md)
- **README**: [`README.md`](./README.md)

---

## 🎯 요약

| 구분 | 잘못된 방법 | 올바른 방법 |
|------|-------------|-------------|
| **스키마 구조** | 래퍼 포함 (`Argument` 중첩) | 단일 엔티티 (직접 필드) |
| **전송 레이어** | 스키마에 포함 | `x-transport.body-root` |
| **여러 인스턴스** | 스키마에 배열 | `x-transport` + UI 관리 |
| **required** | 중첩된 경로 | 최상위 필드명 |
| **allOf 조건** | 점 표기법 | 직접 필드 참조 |
| **검증 로직** | x-* 확장으로만 | 표준 JSON Schema 우선 |

---

**이 규칙을 따르면:**
- ✅ 컴파일러가 정확히 파싱
- ✅ 빌더가 올바른 UI 생성
- ✅ 테이블이 정확히 렌더링
- ✅ 검증이 제대로 작동

**규칙을 어기면:**
- ❌ 필드가 인식 안 됨
- ❌ UI가 깨짐
- ❌ 검증 실패
- ❌ API 호출 실패


