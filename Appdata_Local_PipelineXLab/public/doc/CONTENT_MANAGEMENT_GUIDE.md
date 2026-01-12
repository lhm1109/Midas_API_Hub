# 콘텐츠 관리 시스템 (CMS) 가이드

## 개요

Launcher 앱의 Home 탭 콘텐츠(뉴스, 공지, 튜토리얼 등)를 **WordPress**에서 관리하고 앱에서 불러오는 방법을 설명합니다.

---

## 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                  │
│   [관리자]                                                       │
│      │                                                          │
│      ▼                                                          │
│   ┌──────────────┐         ┌──────────────────────────────────┐ │
│   │  WordPress   │  ─────▶ │      Launcher App (Home)         │ │
│   │  (글 작성)    │  REST   │  - 뉴스 카드                      │ │
│   │              │   API   │  - 공지사항                       │ │
│   └──────────────┘         │  - 튜토리얼                       │ │
│                            └──────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## 1. WordPress REST API 사용

WordPress는 기본적으로 REST API를 제공합니다.

### 1.1 기본 엔드포인트

| 엔드포인트 | 설명 |
|-----------|------|
| `/wp-json/wp/v2/posts` | 포스트 목록 |
| `/wp-json/wp/v2/posts/{id}` | 특정 포스트 |
| `/wp-json/wp/v2/categories` | 카테고리 목록 |
| `/wp-json/wp/v2/media` | 미디어 (이미지) |

### 1.2 예시 요청

```bash
# 최신 포스트 10개 가져오기
GET https://your-wordpress.com/wp-json/wp/v2/posts?per_page=10&_embed

# 특정 카테고리 포스트 (카테고리 ID: 5)
GET https://your-wordpress.com/wp-json/wp/v2/posts?categories=5&_embed

# 특정 태그 포스트
GET https://your-wordpress.com/wp-json/wp/v2/posts?tags=10&_embed
```

---

## 2. WordPress 카테고리 설정

앱에서 사용할 카테고리를 WordPress에 생성합니다:

| 카테고리 | Slug | 용도 |
|---------|------|------|
| 뉴스 | `news` | 업데이트 소식 |
| 공지사항 | `announcement` | 중요 공지 |
| 튜토리얼 | `tutorial` | 사용 가이드 |
| 컴포넌트 | `component` | 컴포넌트 소개 |

---

## 3. 앱에서 WordPress 데이터 Fetch

### 3.1 타입 정의

```typescript
// src/types/content.ts
export interface WordPressPost {
  id: number;
  title: { rendered: string };
  excerpt: { rendered: string };
  content: { rendered: string };
  date: string;
  link: string;
  categories: number[];
  tags: number[];
  _embedded?: {
    'wp:featuredmedia'?: Array<{
      source_url: string;
    }>;
  };
}

export interface ContentItem {
  id: string;
  title: string;
  summary: string;
  thumbnail: string | null;
  link: string;
  category: string;
  publishedAt: string;
}
```

### 3.2 Fetch 함수

```typescript
// src/services/contentService.ts
const WP_API_BASE = 'https://your-wordpress.com/wp-json/wp/v2';

// 카테고리 ID 매핑 (WordPress에서 확인)
const CATEGORY_IDS = {
  news: 5,
  announcement: 6,
  tutorial: 7,
  component: 8,
};

export async function fetchNews(): Promise<ContentItem[]> {
  const response = await fetch(
    `${WP_API_BASE}/posts?categories=${CATEGORY_IDS.news}&per_page=10&_embed`
  );
  const posts: WordPressPost[] = await response.json();
  
  return posts.map(post => ({
    id: String(post.id),
    title: decodeHtml(post.title.rendered),
    summary: decodeHtml(post.excerpt.rendered.replace(/<[^>]*>/g, '')),
    thumbnail: post._embedded?.['wp:featuredmedia']?.[0]?.source_url || null,
    link: post.link,
    category: 'news',
    publishedAt: post.date,
  }));
}

export async function fetchAnnouncements(): Promise<ContentItem[]> {
  const response = await fetch(
    `${WP_API_BASE}/posts?categories=${CATEGORY_IDS.announcement}&per_page=5&_embed`
  );
  const posts: WordPressPost[] = await response.json();
  
  return posts.map(post => ({
    id: String(post.id),
    title: decodeHtml(post.title.rendered),
    summary: decodeHtml(post.excerpt.rendered.replace(/<[^>]*>/g, '')),
    thumbnail: post._embedded?.['wp:featuredmedia']?.[0]?.source_url || null,
    link: post.link,
    category: 'announcement',
    publishedAt: post.date,
  }));
}

// HTML 엔티티 디코딩
function decodeHtml(html: string): string {
  const txt = document.createElement('textarea');
  txt.innerHTML = html;
  return txt.value;
}
```

### 3.3 React Hook

```typescript
// src/hooks/useContent.ts
import { useState, useEffect } from 'react';
import { fetchNews, fetchAnnouncements, ContentItem } from '../services/contentService';

export function useHomeContent() {
  const [news, setNews] = useState<ContentItem[]>([]);
  const [announcements, setAnnouncements] = useState<ContentItem[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    async function load() {
      try {
        const [newsData, announcementsData] = await Promise.all([
          fetchNews(),
          fetchAnnouncements(),
        ]);
        setNews(newsData);
        setAnnouncements(announcementsData);
      } catch (err) {
        setError('콘텐츠를 불러오는데 실패했습니다.');
      } finally {
        setLoading(false);
      }
    }
    load();
  }, []);

  return { news, announcements, loading, error };
}
```

### 3.4 HomeTab에서 사용

```tsx
// src/components/HomeTab.tsx
import { useHomeContent } from '../hooks/useContent';

export default function HomeTab() {
  const { news, announcements, loading, error } = useHomeContent();

  if (loading) return <div>Loading...</div>;
  if (error) return <div>{error}</div>;

  return (
    <div>
      {/* 공지사항 배너 */}
      {announcements[0] && (
        <div className="bg-blue-600 text-white p-4 rounded-lg mb-4">
          {announcements[0].title}
        </div>
      )}
      
      {/* 뉴스 카드 그리드 */}
      <div className="grid grid-cols-4 gap-4">
        {news.map(item => (
          <div key={item.id} className="bg-slate-800 rounded-lg overflow-hidden">
            {item.thumbnail && (
              <img src={item.thumbnail} alt="" className="w-full h-32 object-cover" />
            )}
            <div className="p-3">
              <h3 className="text-sm font-medium">{item.title}</h3>
              <p className="text-xs text-slate-400 mt-1">{item.summary}</p>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
```

---

## 4. WordPress 글 작성 가이드

### 4.1 새 포스트 작성 시

1. **제목**: 뉴스 제목 입력
2. **본문**: 상세 내용 (앱에서는 excerpt만 표시)
3. **발췌**: 카드에 표시될 요약문
4. **특성 이미지**: 썸네일 이미지 업로드
5. **카테고리**: `news`, `announcement`, `tutorial` 중 선택

### 4.2 특성 이미지 권장 사이즈

| 용도 | 사이즈 |
|------|--------|
| 뉴스 카드 썸네일 | 400 x 200 px |
| 배너 이미지 | 1200 x 300 px |

---

## 5. CORS 설정 (필요 시)

WordPress에서 앱이 API를 호출하려면 CORS 허용이 필요할 수 있습니다.

### functions.php에 추가

```php
// CORS 허용
add_action('rest_api_init', function() {
    remove_filter('rest_pre_serve_request', 'rest_send_cors_headers');
    add_filter('rest_pre_serve_request', function($value) {
        header('Access-Control-Allow-Origin: *');
        header('Access-Control-Allow-Methods: GET');
        header('Access-Control-Allow-Headers: Content-Type');
        return $value;
    });
});
```

또는 **.htaccess**에 추가:

```apache
<IfModule mod_headers.c>
    Header set Access-Control-Allow-Origin "*"
</IfModule>
```

---

## 6. 캐싱 전략

### 앱 내 캐싱

```typescript
const CACHE_KEY = 'pxlab_wp_cache';
const CACHE_TTL = 10 * 60 * 1000; // 10분

async function fetchWithCache(url: string) {
  const cacheKey = `${CACHE_KEY}_${url}`;
  const cached = localStorage.getItem(cacheKey);
  
  if (cached) {
    const { data, timestamp } = JSON.parse(cached);
    if (Date.now() - timestamp < CACHE_TTL) {
      return data;
    }
  }
  
  const response = await fetch(url);
  const data = await response.json();
  
  localStorage.setItem(cacheKey, JSON.stringify({
    data,
    timestamp: Date.now(),
  }));
  
  return data;
}
```

---

## 7. 요약

| 항목 | 값 |
|------|-----|
| CMS | WordPress (기존 보유) |
| API | WordPress REST API |
| 글 작성 | WordPress 관리자 페이지 |
| 앱 연동 | fetch + useEffect |
| 캐싱 | localStorage (10분 TTL) |

**장점:**
- ✅ 이미 보유한 WordPress 사용
- ✅ 익숙한 글 작성 인터페이스
- ✅ 이미지 관리 내장
- ✅ 추가 비용 없음

