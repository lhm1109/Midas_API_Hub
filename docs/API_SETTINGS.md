# API Settings & Client

API 테스트를 위한 설정 및 클라이언트 시스템

## 🔧 구성 요소

### 1. API Settings Store (`useApiSettingsStore`)

Zustand를 사용한 전역 상태 관리

```typescript
const { 
  baseUrl,        // API Base URL
  mapiKey,        // MAPI-Key
  isConnected,    // 연결 상태
  updateSettings, // 설정 업데이트
  setConnected    // 연결 상태 변경
} = useApiSettingsStore();
```

### 2. API Settings Modal

UI 컴포넌트로 설정 관리

- **Base URL 설정**: `https://moa-engineers.midasit.com:443/civil`
- **MAPI-Key 설정**: JWT 토큰
- **Connect/Disconnect**: 연결 테스트 및 상태 관리
- **Auto Connect on Startup**: 앱 시작 시 자동 연결

### 3. API Client (`useApiClient`)

API 요청을 위한 클라이언트

```typescript
const apiClient = useApiClient();

// GET 요청
const response = await apiClient?.get('/api/db/PROJINFO');

// POST 요청
const response = await apiClient?.post('/api/ope/LINEBMLD', {
  Argument: {
    LCNAME: "LIVE_LOAD",
    TYPE: "CONLOAD",
    // ...
  }
});

// PUT 요청
const response = await apiClient?.put('/api/db/PROJINFO', data);

// DELETE 요청
const response = await apiClient?.delete('/api/ope/LINEBMLD/1');
```

## 📦 파일 구조

```
src/
├── utils/
│   ├── apiSettings.ts      # 설정 로직 및 헬퍼 함수
│   └── apiClient.ts        # API 클라이언트
├── store/
│   └── useApiSettingsStore.ts  # Zustand 스토어
└── components/
    └── settings/
        └── ApiSettingsModal.tsx  # 설정 UI
```

## 🎯 사용 방법

### 1. 설정 열기

헤더의 "설정" 버튼 클릭

### 2. API 정보 입력

- **Base URL**: API 서버 주소
- **MAPI-Key**: 인증 키

### 3. 연결 테스트

"Connect" 버튼 클릭 → 상태가 "Connected"로 변경

### 4. API 요청 보내기

```typescript
import { useApiClient } from '@/utils/apiClient';

function MyComponent() {
  const apiClient = useApiClient();

  const handleTest = async () => {
    if (!apiClient) {
      alert('API가 연결되지 않았습니다');
      return;
    }

    const response = await apiClient.get('/api/db/PROJINFO');
    
    if (response.success) {
      console.log('Data:', response.data);
    } else {
      console.error('Error:', response.error);
    }
  };

  return <button onClick={handleTest}>Test API</button>;
}
```

## 🔐 헤더 구성

모든 요청에 자동으로 포함:

```
Content-Type: application/json
MAPI-Key: eyJ1ci...
```

## 💾 로컬 저장소

설정은 `localStorage`에 자동 저장:

```
Key: api_verification_settings
Value: {
  baseUrl: string,
  mapiKey: string,
  connectOnStartup: boolean,
  isConnected: boolean
}
```

## 🚀 자동 연결

"Connect API on Startup" 체크박스 활성화 시:
- 앱 시작 시 자동으로 API 연결 시도
- 실패 시 연결 해제 상태 유지

## ✨ 기능

- ✅ Base URL 및 MAPI-Key 관리
- ✅ 연결 상태 테스트
- ✅ 자동 연결 (선택 사항)
- ✅ 설정 저장 (localStorage)
- ✅ Copy 버튼 (URL, Key)
- ✅ Refresh 버튼 (연결 재시도)
- ✅ 헤더 자동 포함
- ✅ 타입 안전한 API 클라이언트

