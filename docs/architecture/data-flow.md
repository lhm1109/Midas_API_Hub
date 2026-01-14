# 데이터 플로우

**API Verification System - Data Flow Documentation**

---

## 📋 목차

1. [엔드포인트 선택 플로우](#엔드포인트-선택-플로우)
2. [Builder 폼 렌더링 플로우](#builder-폼-렌더링-플로우)
3. [API 테스트 실행 플로우](#api-테스트-실행-플로우)
4. [버전 저장 플로우](#버전-저장-플로우)
5. [테스트 케이스 관리 플로우](#테스트-케이스-관리-플로우)
6. [락 관리 플로우](#락-관리-플로우)
7. [YAML 기반 렌더링 플로우](#yaml-기반-렌더링-플로우)

---

## 엔드포인트 선택 플로우

### 전체 흐름

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. User Action                                                   │
│    APIListPanel에서 엔드포인트 클릭                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 2. Event Handler (APIListPanel)                                  │
│    handleSelectEndpoint(endpoint)                                │
│      → ProjectsView.onSelect(endpoint)                           │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 3. ProjectsView                                                  │
│    handleSelectEndpoint(endpoint)                                │
│      ├─ setSelectedEndpoint(endpoint)                            │
│      ├─ acquireLock(endpoint.id)                                 │
│      ├─ loadVersions(endpoint.id)                                │
│      └─ loadTestCases(endpoint.id)                               │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 4. Lock Acquisition                                              │
│    apiClient.acquireEndpointLock(endpointId, userId)             │
│      ↓ POST /api/locks/endpoint/:id/lock                         │
│      ↓ Body: { userId, expiresAt: now + 5 minutes }             │
│                                                                   │
│    Backend: locks.js                                             │
│      ├─ Check existing lock                                      │
│      │   SELECT * FROM endpoint_locks WHERE endpoint_id = :id    │
│      ├─ If locked by another user → 409 Conflict                 │
│      └─ If available → INSERT / UPDATE lock                      │
│          INSERT INTO endpoint_locks                              │
│          VALUES (:id, :userId, NOW())                            │
│          ON CONFLICT UPDATE locked_at = NOW()                    │
│                                                                   │
│    Response: { success: true, lock: {...} }                      │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 5. Load Versions                                                 │
│    apiClient.getVersions(endpointId)                             │
│      ↓ GET /api/versions/endpoint/:id                            │
│                                                                   │
│    Backend: versions.js                                          │
│      SELECT * FROM api_versions                                  │
│      WHERE endpoint_id = :id                                     │
│      ORDER BY version_number DESC                                │
│                                                                   │
│    Response: [                                                   │
│      {                                                            │
│        id, version_number, request_schema,                       │
│        response_schema, examples, manual_content, created_at     │
│      },                                                           │
│      ...                                                          │
│    ]                                                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 6. Load Test Cases                                               │
│    apiClient.getTestCases(endpointId)                            │
│      ↓ GET /api/test-cases/endpoint/:id                          │
│                                                                   │
│    Backend: testCases.js                                         │
│      SELECT * FROM test_cases                                    │
│      WHERE endpoint_id = :id                                     │
│      ORDER BY created_at DESC                                    │
│                                                                   │
│    Response: [                                                   │
│      { id, name, request_body, created_at },                     │
│      ...                                                          │
│    ]                                                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 7. Store Update (Zustand)                                        │
│    useAppStore.setState({                                        │
│      selectedEndpoint: endpoint,                                 │
│      endpointLock: lock,                                         │
│      versions: versionsData,                                     │
│      testCases: testCasesData,                                   │
│      currentVersion: versionsData[0] || null                     │
│    })                                                             │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 8. Component Re-render                                           │
│    - ProjectsView: 엔드포인트 정보 표시                         │
│    - VersionTab: 버전 목록 표시                                 │
│    - ManualTab: 문서 내용 로드                                   │
│    - SpecTab: 스펙 테이블 렌더링                                │
│    - BuilderTab: 폼 생성                                         │
│    - RunnerTab: 요청/응답 초기화                                │
└─────────────────────────────────────────────────────────────────┘
```

### 코드 예시

```typescript
// APIListPanel.tsx
const handleSelectEndpoint = (endpoint: ApiEndpoint) => {
  onSelect?.(endpoint);
};

// ProjectsView.tsx
const handleSelectEndpoint = async (endpoint: ApiEndpoint) => {
  try {
    // 1. Store 업데이트
    setSelectedEndpoint(endpoint);
    
    // 2. Lock 획득
    const lockAcquired = await acquireLock('endpoint', endpoint.id);
    if (!lockAcquired) {
      toast.error('This endpoint is locked by another user');
      return;
    }
    
    // 3. 버전 로드
    const versions = await apiClient.getVersions(endpoint.id);
    setVersions(versions);
    
    // 4. 테스트 케이스 로드
    const testCases = await apiClient.getTestCases(endpoint.id);
    setTestCases(testCases);
    
    // 5. 현재 버전 설정
    if (versions.length > 0) {
      setCurrentVersion(versions[0]);
    }
    
    toast.success('Endpoint loaded successfully');
  } catch (error) {
    toast.error('Failed to load endpoint');
    console.error(error);
  }
};
```

---

## Builder 폼 렌더링 플로우

### 전체 흐름

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. BuilderTab Mounted                                            │
│    useEffect(() => { ... }, [selectedEndpoint])                  │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 2. Load YAML Definitions                                         │
│    useSchemaDefinitions(schemaType)                              │
│      ├─ schemaType = settings.schemaDefinition                   │
│      │   ('auto' | 'original' | 'enhanced')                      │
│      ├─ isNewEnhancedSchema = has x-* fields?                    │
│      └─ Load YAML files:                                         │
│          - schema_definitions/{schemaType}/ui-rules.yaml         │
│          - schema_definitions/{schemaType}/builder.yaml          │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 3. Compile Schema                                                │
│                                                                   │
│    If Enhanced Schema:                                           │
│      enhancedSchemaCompiler.compileEnhancedSchema(schema)        │
│        ├─ Parse x-ui, x-section-header, x-enum-by-type          │
│        ├─ Create section blocks                                  │
│        └─ Generate EnhancedField[]                               │
│                                                                   │
│    If Original Schema:                                           │
│      schemaCompiler.compileSchema(schema)                        │
│        ├─ Parse oneOf, allOf, if-then-else                       │
│        ├─ Normalize types                                        │
│        └─ Generate CanonicalField[]                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 4. Parse x-* Extensions (V3)                                     │
│    dynamicSchemaCompiler.parseSchemaExtensions(schema)           │
│      ├─ Load x-extensions-v3.yaml                                │
│      ├─ For each extension definition:                           │
│      │   ├─ Extract field value (e.g., x-enum-by-type)          │
│      │   └─ Store as internalName (enumByType)                   │
│      └─ Attach to field metadata                                 │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 5. Initialize Behavior Engine                                    │
│    const engine = getBehaviorEngine()                            │
│    await engine.loadDefinitions('/x-extensions-v3.yaml')         │
│      ├─ Parse extensions and behaviors                           │
│      ├─ Register action executors                                │
│      └─ Ready for trigger execution                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 6. Render Form (DynamicSchemaRenderer)                           │
│    <DynamicSchemaRenderer                                        │
│      schemaFields={fields}                                       │
│      formData={dynamicFormData}                                  │
│      yamlDefinitions={yamlDefs}                                  │
│      settings={settings}                                         │
│    />                                                             │
│                                                                   │
│    For each field:                                               │
│      ├─ Determine field type (string, integer, enum, etc.)       │
│      ├─ Apply YAML rendering rules                               │
│      │   ├─ Get component from ui-rules.yaml                     │
│      │   ├─ Apply styles from builder.yaml                       │
│      │   └─ Handle conditional rendering (visibleWhen)           │
│      └─ Render component:                                        │
│          ├─ <Input> for string/number                            │
│          ├─ <Select> for enum                                    │
│          ├─ <Checkbox> for boolean                               │
│          └─ <Textarea> for long text                             │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 7. User Interaction                                              │
│    User changes TYPE field to "BEAM"                             │
│      ↓ onChange event                                            │
│      ↓ handleFieldChange('TYPE', 'BEAM')                         │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 8. Behavior Engine Trigger (V3)                                  │
│    useBehaviorEngine().onFieldChange(                            │
│      'TYPE', field, formValues, schema,                          │
│      updateField, showMessage                                    │
│    )                                                              │
│      ↓ engine.executeTrigger('value-change', context)            │
│      ↓ Find applied extensions in dependent fields               │
│      ↓ Execute behaviors (update-enum, validate, etc.)           │
│      ↓ Update field state                                        │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 9. Dependency Change Detection                                   │
│    useEffect(() => {                                             │
│      onDependencyChange(                                         │
│        'STYPE',  // Affected field                               │
│        schema.STYPE, formValues, schema,                         │
│        updateField, showMessage,                                 │
│        'TYPE'    // Changed field                                │
│      )                                                            │
│    }, [formValues.TYPE])                                         │
│                                                                   │
│    Behavior Engine:                                              │
│      ├─ Trigger: 'dependency-change'                             │
│      ├─ Actions:                                                 │
│      │   ├─ update-enum: STYPE.enum = [1, 2]                     │
│      │   └─ clear-value: if invalid, clear STYPE.value           │
│      └─ Update STYPE field                                       │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 10. Component Re-render                                          │
│     React detects state change                                   │
│       ↓ schemaFields updated                                     │
│       ↓ formValues updated                                       │
│       ↓ DynamicSchemaRenderer re-renders                         │
│       ↓ Select component shows new options ✨                    │
└─────────────────────────────────────────────────────────────────┘
```

### 코드 예시

```typescript
// BuilderTab.tsx
const BuilderTab = () => {
  const [schemaFields, setSchemaFields] = useState({});
  const [formData, setFormData] = useState({});
  const { onFieldChange, onDependencyChange } = useBehaviorEngine();
  
  // 1. 스키마 컴파일
  useEffect(() => {
    if (currentVersion?.request_schema) {
      const compiled = isEnhanced 
        ? enhancedSchemaCompiler.compile(schema)
        : schemaCompiler.compile(schema);
      
      setSchemaFields(compiled);
    }
  }, [currentVersion]);
  
  // 2. 필드 변경 핸들러
  const handleFieldChange = async (fieldName, value) => {
    setFormData(prev => ({ ...prev, [fieldName]: value }));
    
    await onFieldChange(
      fieldName,
      schemaFields[fieldName],
      formData,
      schemaFields,
      updateField,
      toast.error
    );
  };
  
  // 3. 의존성 변경 감지
  useEffect(() => {
    if (formData.TYPE) {
      onDependencyChange(
        'STYPE',
        schemaFields.STYPE,
        formData,
        schemaFields,
        updateField,
        toast.error,
        'TYPE'
      );
    }
  }, [formData.TYPE]);
  
  return (
    <DynamicSchemaRenderer
      schemaFields={schemaFields}
      formData={formData}
      onChange={handleFieldChange}
    />
  );
};
```

---

## API 테스트 실행 플로우

### 전체 흐름

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. User Action                                                   │
│    RunnerTab에서 "Send" 버튼 클릭                               │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 2. Build Request Body (RunnerTab)                                │
│    handleSend()                                                  │
│      ├─ Get data from builderData                                │
│      ├─ If Assign wrapper enabled:                               │
│      │   requestBody = {                                         │
│      │     "Assign": {                                           │
│      │       "1": { /* instance 1 data */ },                     │
│      │       "2": { /* instance 2 data */ }                      │
│      │     }                                                      │
│      │   }                                                        │
│      └─ Else: requestBody = { /* direct data */ }                │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 3. Send Request (apiClient)                                      │
│    apiClient.proxy({                                             │
│      method: 'POST',                                             │
│      url: 'https://api.example.com/node',                        │
│      body: requestBody,                                          │
│      headers: { 'Content-Type': 'application/json' }             │
│    })                                                             │
│      ↓ POST /api/proxy                                           │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 4. Backend Proxy (server/routes/proxy.js)                        │
│    POST /api/proxy                                               │
│      ├─ Extract target URL, method, body, headers                │
│      ├─ Forward request to target API:                           │
│      │   fetch(targetUrl, {                                      │
│      │     method, body, headers                                 │
│      │   })                                                       │
│      └─ Return response with CORS headers                        │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 5. Target API Processing                                         │
│    External API (e.g., https://api.example.com)                  │
│      ├─ Receive request                                          │
│      ├─ Process business logic                                   │
│      ├─ Validate data                                            │
│      └─ Return response:                                         │
│          - 2xx: Success                                          │
│          - 4xx: Client error                                     │
│          - 5xx: Server error                                     │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 6. Response Handling (RunnerTab)                                 │
│    const response = await apiClient.proxy(...)                   │
│                                                                   │
│    if (response.status >= 200 && response.status < 300) {        │
│      // Success                                                  │
│      setRunnerData({                                             │
│        response: response.data,                                  │
│        status: 'success'                                         │
│      })                                                           │
│                                                                   │
│      // Auto-update endpoint status                              │
│      apiClient.updateEndpoint(endpointId, {                      │
│        status: 'success',                                        │
│        status_message: 'OK'                                      │
│      })                                                           │
│                                                                   │
│    } else {                                                       │
│      // Error                                                    │
│      setRunnerData({                                             │
│        response: response.data,                                  │
│        status: 'error'                                           │
│      })                                                           │
│                                                                   │
│      apiClient.updateEndpoint(endpointId, {                      │
│        status: 'error',                                          │
│        status_message: response.data.message || 'Error'          │
│      })                                                           │
│    }                                                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 7. Update Endpoint Status (Backend)                              │
│    PUT /api/endpoints/:id                                        │
│      UPDATE api_endpoints                                        │
│      SET status = :status, status_message = :message             │
│      WHERE id = :id                                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 8. UI Update                                                     │
│    - RunnerTab: Display response (JSON formatted)                │
│    - APIListPanel: Update status indicator                       │
│      🟢 Success / 🔴 Error                                       │
└─────────────────────────────────────────────────────────────────┘
```

### 코드 예시

```typescript
// RunnerTab.tsx
const handleSend = async () => {
  try {
    setLoading(true);
    
    // 1. Build request
    const requestBody = settings.useAssignWrapper
      ? wrapWithAssign(builderData)
      : builderData;
    
    // 2. Send request
    const response = await apiClient.proxy({
      method: selectedEndpoint.http_method,
      url: `${selectedEndpoint.base_url}${selectedEndpoint.path}`,
      body: requestBody
    });
    
    // 3. Handle response
    setRunnerData({
      request: requestBody,
      response: response.data,
      status: response.status
    });
    
    // 4. Auto-update endpoint status
    const isSuccess = response.status >= 200 && response.status < 300;
    await apiClient.updateEndpoint(selectedEndpoint.id, {
      status: isSuccess ? 'success' : 'error',
      status_message: isSuccess 
        ? 'OK' 
        : response.data?.message || `HTTP ${response.status}`
    });
    
    toast.success('Request completed');
    
  } catch (error) {
    toast.error('Request failed');
    
    // Network error
    await apiClient.updateEndpoint(selectedEndpoint.id, {
      status: 'error',
      status_message: error.message || 'Network error'
    });
  } finally {
    setLoading(false);
  }
};
```

---

## 버전 저장 플로우

### 전체 흐름

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. User Action                                                   │
│    메인 바에서 "Save Version" 버튼 클릭                         │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 2. Gather All Tab Data (ProjectsView)                            │
│    handleSaveVersion()                                           │
│      ├─ manualData.content         (Manual tab)                  │
│      ├─ specData.tableSchema       (Spec tab)                    │
│      ├─ builderData.schema         (Builder tab)                 │
│      ├─ runnerData.requestExample  (Runner tab)                  │
│      └─ runnerData.responseExample (Runner tab)                  │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 3. Save Version (useAppStore)                                    │
│    saveCurrentVersion()                                          │
│      ├─ If editing existing version:                             │
│      │   ↓ PUT /api/versions/:id                                 │
│      │   ↓ Update existing version                               │
│      └─ If creating new version:                                 │
│          ↓ POST /api/versions                                    │
│          ↓ Create new version                                    │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 4. Backend Processing (server/routes/versions.js)                │
│                                                                   │
│    POST /api/versions                                            │
│      ├─ Generate version number:                                 │
│      │   SELECT MAX(version_number) + 1                          │
│      │   FROM api_versions                                       │
│      │   WHERE endpoint_id = :id                                 │
│      │                                                            │
│      └─ Insert new version:                                      │
│          INSERT INTO api_versions (                              │
│            endpoint_id, version_number,                          │
│            request_schema, response_schema,                      │
│            examples, manual_content,                             │
│            created_at, created_by                                │
│          ) VALUES (...)                                          │
│                                                                   │
│    PUT /api/versions/:id                                         │
│      └─ Update existing version:                                 │
│          UPDATE api_versions                                     │
│          SET request_schema = :req,                              │
│              response_schema = :res,                             │
│              examples = :ex,                                     │
│              manual_content = :content                           │
│          WHERE id = :id                                          │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 5. Response Handling (ProjectsView)                              │
│    try {                                                         │
│      await saveCurrentVersion()                                  │
│      toast.success('Version saved successfully!')                │
│                                                                   │
│      // Reload versions                                          │
│      const versions = await apiClient.getVersions(endpointId)    │
│      setVersions(versions)                                       │
│                                                                   │
│    } catch (error) {                                             │
│      toast.error('Failed to save version')                       │
│      console.error(error)                                        │
│    }                                                              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 6. UI Update                                                     │
│    - VersionTab: Show new version in list                        │
│    - Toast notification: Success/Error message                   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 테스트 케이스 관리 플로우

### 저장 플로우

```
BuilderTab: "Save as Test Case" 버튼 클릭
    ↓
handleSaveTestCase(name)
    ↓
Build full JSON:
  - Gather all instances data
  - Apply Assign wrapper (if enabled)
  - Convert dot notation → nested object
    ↓
apiClient.addTestCase(endpointId, name, requestBody)
    ↓
POST /api/test-cases
  INSERT INTO test_cases (endpoint_id, name, request_body)
    ↓
Response: { id, name, request_body, created_at }
    ↓
Store update: testCases = [...testCases, newCase]
    ↓
UI update: Test case list shows new item
```

### 로드 플로우

```
BuilderTab: Test case 클릭
    ↓
handleLoadTestCase(testCase)
    ↓
Parse request body:
  - If has Assign wrapper → extract instances
  - Convert nested object → dot notation
    ↓
Populate form:
  - setAssignInstances(instances)
  - setDynamicFormData(flattenedData)
    ↓
BuilderTab re-renders with loaded data
```

### 업데이트 플로우

```
BuilderTab: "Update Test Case" 버튼 클릭
    ↓
handleUpdateTestCase()
    ↓
Build full JSON (same as save)
    ↓
apiClient.updateTestCase(id, name, requestBody)
    ↓
PUT /api/test-cases/:id
  UPDATE test_cases
  SET name = :name, request_body = :body
    ↓
Store update: testCases = testCases.map(...)
    ↓
UI update: Test case list reflects changes
```

---

## 락 관리 플로우

### 락 획득 플로우

```
Endpoint 선택
    ↓
acquireLock('endpoint', endpointId)
    ↓
POST /api/locks/endpoint/:id/lock
  Body: { userId, expiresAt: now + 5min }
    ↓
Backend: locks.js
  ├─ Check existing lock
  │   SELECT * FROM endpoint_locks
  │   WHERE endpoint_id = :id
  ├─ If locked by another user
  │   → 409 Conflict
  └─ If available
      → UPSERT lock
        INSERT ... ON CONFLICT UPDATE
    ↓
Success: { success: true, lock }
Failure: { success: false, locked_by }
    ↓
Store update: endpointLock = lock
    ↓
UI: Lock acquired, editing enabled
```

### 락 상태 확인 (Realtime)

```
┌─────────────────────────────────────────────────────────────────┐
│ Supabase Realtime Subscription                                   │
│                                                                   │
│ supabase                                                          │
│   .channel('endpoint_locks')                                     │
│   .on('postgres_changes',                                        │
│     { event: '*', schema: 'public', table: 'endpoint_locks' },   │
│     (payload) => {                                               │
│       if (payload.new.endpoint_id === currentEndpointId) {       │
│         updateLockStatus(payload.new)                            │
│       }                                                           │
│     }                                                             │
│   )                                                               │
│   .subscribe()                                                    │
└─────────────────────────────────────────────────────────────────┘
```

### 락 해제 플로우

```
Endpoint 변경 또는 언마운트
    ↓
releaseLock('endpoint', endpointId)
    ↓
POST /api/locks/endpoint/:id/unlock
  Body: { userId }
    ↓
Backend: locks.js
  DELETE FROM endpoint_locks
  WHERE endpoint_id = :id
  AND user_id = :userId
    ↓
Store update: endpointLock = null
    ↓
UI: Lock released, read-only mode
```

### 자동 만료 (5분 TTL)

```
Backend: Periodic cleanup job (every minute)
    ↓
DELETE FROM endpoint_locks
WHERE locked_at < NOW() - INTERVAL '5 minutes'
    ↓
Supabase Realtime broadcasts DELETE event
    ↓
Frontend receives event
    ↓
If current user's lock expired:
  → Show warning
  → Switch to read-only mode
```

---

## YAML 기반 렌더링 플로우

### 정의 로드

```
Component mounted
    ↓
useSchemaDefinitions(schemaType)
    ↓
Load YAML files:
  - schema_definitions/{schemaType}/ui-rules.yaml
  - schema_definitions/{schemaType}/builder.yaml
  - (or table.yaml, html-template.yaml)
    ↓
Parse YAML → JavaScript object
    ↓
Merge with base rules
    ↓
Return definitions
```

### 동적 렌더링

```
<DynamicSchemaRenderer
  schemaFields={fields}
  yamlDefinitions={defs}
/>
    ↓
For each field:
  ├─ Get field type (string, integer, enum, etc.)
  ├─ Look up component in yaml:
  │   defs.fieldTypes[type].component  // e.g., "Input"
  ├─ Get styles from yaml:
  │   defs.styles[type]
  ├─ Check visibility:
  │   field.visibleWhen → evaluate condition
  └─ Render component with props
```

### V3 Behavior 통합

```
Field rendered
    ↓
User interacts
    ↓
Event handler triggered
    ↓
Behavior Engine.executeTrigger()
  ├─ Find applied x-* extensions
  ├─ Load behaviors from x-extensions-v3.yaml
  ├─ Execute actions (YAML-defined)
  └─ Update field state
    ↓
Component re-renders with new state
```

---

**Last Updated**: 2026-01-14  
**Version**: 3.0

