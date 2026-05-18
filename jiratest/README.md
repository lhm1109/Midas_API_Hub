# Jira Issue Categorizer

Jira 프로젝트 이슈를 JQL로 조회한 뒤 지정한 필드 기준으로 Markdown, CSV, JSON 리포트를 생성합니다.

## 설정

1. `jiratest/.env.example`을 참고해서 `jiratest/.env`를 만듭니다.
2. `JIRA_API_TOKEN`에는 토큰을 넣습니다. 토큰은 커밋하지 마세요.
3. Jira Cloud API 토큰이면 `JIRA_EMAIL`도 필요합니다.

```env
JIRA_BASE_URL=https://your-site.atlassian.net
JIRA_EMAIL=your-email@example.com
JIRA_API_TOKEN=...
JIRA_PROJECT_KEY=PROJECTKEY
```

## 실행

```powershell
cd C:\MIDAS\code\apiverification\jiratest
node .\jira_issue_categorizer.mjs
```

특정 JQL을 직접 지정할 수도 있습니다.

```powershell
node .\jira_issue_categorizer.mjs --jql "project = PROJECTKEY AND statusCategory != Done ORDER BY updated DESC"
```

## 카테고리 기준 변경

기본값은 `components`입니다. 아래처럼 기준을 바꿀 수 있습니다.

```powershell
node .\jira_issue_categorizer.mjs --by labels
node .\jira_issue_categorizer.mjs --by status
node .\jira_issue_categorizer.mjs --by issuetype
node .\jira_issue_categorizer.mjs --by priority
node .\jira_issue_categorizer.mjs --by customfield_12345
```

## 결과

결과는 기본적으로 `jiratest/output`에 생성됩니다.

- `issues_by_category.md`: 사람이 읽기 좋은 카테고리별 목록
- `issues_by_category.csv`: Excel에서 열기 좋은 목록
- `issues_by_category.json`: 후처리용 그룹 데이터
- `issues_raw.json`: Jira 원본 응답 이슈 데이터
