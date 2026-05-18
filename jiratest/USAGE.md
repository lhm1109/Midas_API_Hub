# Jira / Confluence Test Usage

이 문서는 `C:\MIDAS\code\apiverification\jiratest` 기준으로 Jira 이슈 조회와 Confluence 페이지 조회/업데이트를 테스트하는 방법을 정리합니다.

## 1. 환경 설정

`jiratest/.env`에 아래 값을 설정합니다.

```env
JIRA_BASE_URL=https://midasitdev.atlassian.net
JIRA_EMAIL=lhm1109@midasit.com
JIRA_API_TOKEN=your-jira-api-token
JIRA_AUTH_MODE=basic

CONFLUENCE_BASE_URL=https://midastech.atlassian.net/wiki
CONFLUENCE_EMAIL=lhm1109@midasit.com
CONFLUENCE_API_TOKEN=your-confluence-api-token
CONFLUENCE_AUTH_MODE=basic
```

토큰은 저장소에 커밋하지 않습니다.

## 2. Jira 이슈 조회

작업 폴더로 이동합니다.

```powershell
cd C:\MIDAS\code\apiverification\jiratest
```

특정 이슈 1건 조회:

```powershell
node .\jira_issue_categorizer.mjs --jql "key = MAPI-1595" --by status
```

특정 이슈의 하위 업무 조회:

```powershell
node .\jira_issue_categorizer.mjs --jql "parent = MAPI-1595" --by status
```

프로젝트 전체 조회:

```powershell
node .\jira_issue_categorizer.mjs --jql "project = MAPI ORDER BY updated DESC" --by status --max-results 5000
```

접근 가능한 전체 이슈 조회:

```powershell
node .\jira_issue_categorizer.mjs --jql "ORDER BY updated DESC" --by status --max-results 5000
```

카테고리 기준 변경:

```powershell
node .\jira_issue_categorizer.mjs --jql "project = MAPI ORDER BY updated DESC" --by assignee
node .\jira_issue_categorizer.mjs --jql "project = MAPI ORDER BY updated DESC" --by labels
node .\jira_issue_categorizer.mjs --jql "project = MAPI ORDER BY updated DESC" --by issuetype
node .\jira_issue_categorizer.mjs --jql "project = MAPI ORDER BY updated DESC" --by priority
```

결과 파일:

- `output/issues_by_category.md`
- `output/issues_by_category.csv`
- `output/issues_by_category.json`
- `output/issues_raw.json`

## 3. Jira 결과 검색

Markdown에서 키워드 검색:

```powershell
Select-String -Path .\output\issues_by_category.md -Pattern "MAPI-1595|검색어"
```

CSV에서 요약 검색:

```powershell
Import-Csv .\output\issues_by_category.csv |
  Where-Object { $_.summary -match "검색어" } |
  Format-Table category,key,status,assignee,summary -AutoSize
```

CSV에서 담당자/상태 기준 정렬:

```powershell
Import-Csv .\output\issues_by_category.csv |
  Sort-Object assignee,status,key |
  Export-Csv .\output\issues_sorted.csv -NoTypeInformation -Encoding UTF8
```

## 4. Confluence 읽기 테스트

Confluence 인증 확인:

```powershell
$envMap = @{}
Get-Content .\.env | ForEach-Object {
  if ($_ -match '^\s*([^#][^=]+)=(.*)$') {
    $envMap[$matches[1].Trim()] = $matches[2].Trim()
  }
}

$pair = $envMap.CONFLUENCE_EMAIL + ":" + $envMap.CONFLUENCE_API_TOKEN
$auth = "Basic " + [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($pair))
$headers = @{ Authorization = $auth; Accept = "application/json" }

Invoke-RestMethod `
  -Uri ($envMap.CONFLUENCE_BASE_URL + "/rest/api/user/current") `
  -Headers $headers `
  -Method Get
```

페이지 조회:

```powershell
Invoke-RestMethod `
  -Uri ($envMap.CONFLUENCE_BASE_URL + "/rest/api/content/1343881241?expand=version,body.storage") `
  -Headers $headers `
  -Method Get
```

브라우저 URL과 API URL의 차이:

- 브라우저 URL: `https://midastech.atlassian.net/wiki/spaces/~7120200dfb9ccc779445fe93da7fd72d896752/pages/1343881241/SC+Design+Code`
- API URL: `https://midastech.atlassian.net/wiki/rest/api/content/1343881241?expand=version,body.storage`

## 5. Confluence 페이지 업데이트 테스트

아래 예시는 페이지 `1343881241`의 본문을 테스트 문구로 교체합니다. Confluence는 업데이트할 때 현재 version보다 1 큰 숫자를 보내야 합니다.

```powershell
$pageId = "1343881241"
$page = Invoke-RestMethod `
  -Uri ($envMap.CONFLUENCE_BASE_URL + "/rest/api/content/$pageId?expand=version,body.storage") `
  -Headers $headers `
  -Method Get

$nextVersion = [int]$page.version.number + 1
$bodyValue = "<p>Confluence API write test from jiratest.</p>"

$payload = @{
  id = $pageId
  type = "page"
  title = $page.title
  status = "current"
  version = @{
    number = $nextVersion
  }
  body = @{
    storage = @{
      value = $bodyValue
      representation = "storage"
    }
  }
} | ConvertTo-Json -Depth 20

Invoke-WebRequest `
  -Uri ($envMap.CONFLUENCE_BASE_URL + "/rest/api/content/$pageId") `
  -Headers (@{
    Authorization = $auth
    Accept = "application/json"
    "Content-Type" = "application/json"
  }) `
  -Method Put `
  -Body ([Text.Encoding]::UTF8.GetBytes($payload)) `
  -ContentType "application/json; charset=utf-8" `
  -UseBasicParsing
```

주의: 이 방식은 기존 본문을 새 본문으로 교체합니다. 기존 내용을 보존하려면 먼저 `body.storage.value`를 읽어서 원하는 내용을 합친 뒤 업데이트해야 합니다.

## 6. 상태 코드 해석

- `200 OK`: 인증/권한/요청 성공
- `400 Bad Request`: JSON payload 형식, version, title, body 형식 문제 가능
- `401 Unauthorized`: 이메일/토큰 인증 실패
- `403 Forbidden`: 계정은 확인됐지만 해당 Confluence 또는 페이지 권한 없음
- `404 Not Found`: 페이지 ID가 틀렸거나 접근 권한이 없어 숨겨짐
