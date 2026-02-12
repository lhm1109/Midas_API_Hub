# CSS 중앙 관리 구조

모든 스타일을 CSS 파일에서 중앙 관리합니다.

## 📁 구조

```
src/styles/
├── index.css           # Tailwind 기본 + CSS 변수
├── components.css      # 공통 컴포넌트 클래스
```

## 🎨 CSS 변수 (index.css)

### 색상
```css
--background: 210 20% 98%;
--foreground: 222 47% 11%;
--surface-1: 210 33% 99%;
--surface-2: 210 25% 93%;
--surface-3: 210 20% 88%;
--primary: 221 83% 53%;
--muted-foreground: 215 16% 47%;
```

### 간격
```css
--spacing-xs: 4px;
--spacing-sm: 8px;
--spacing-md: 16px;
--spacing-lg: 24px;
```

### 폰트
```css
--font-size-xs: 12px;
--font-size-base: 14px;
--font-weight-medium: 500;
--font-weight-semibold: 600;
```

## 🧱 공통 컴포넌트 클래스 (components.css)

### Tab Content
```css
.tab-content                    /* 탭 메인 컨테이너 */
.tab-content__container         /* 최대 너비 컨테이너 */
.tab-content__header            /* 헤더 영역 */
.tab-content__title             /* 제목 */
.tab-content__description       /* 설명 */
.tab-content__grid              /* 그리드 레이아웃 */
```

### Card
```css
.card                           /* 카드 컨테이너 */
.card__header                   /* 카드 헤더 */
.card__title                    /* 카드 제목 */
.card__body                     /* 카드 본문 */
.card__icon                     /* 아이콘 */
```

### Button
```css
.btn                            /* 기본 버튼 */
.btn--primary                   /* 주요 버튼 */
.btn--secondary                 /* 보조 버튼 */
.btn--method                    /* HTTP 메서드 버튼 */
```

### Upload Area
```css
.upload-area                    /* 업로드 영역 */
.upload-area__icon              /* 업로드 아이콘 */
.upload-area__text              /* 메인 텍스트 */
.upload-area__subtext           /* 서브 텍스트 */
```

### Info Box
```css
.info-box                       /* 정보 박스 */
.info-box__icon                 /* 아이콘 */
.info-box__icon--warning        /* 경고 아이콘 */
.info-box__icon--success        /* 성공 아이콘 */
.info-box__title                /* 제목 */
.info-box__text                 /* 내용 */
```

### Stats
```css
.stats-grid                     /* 통계 그리드 */
.stats-card                     /* 통계 카드 */
.stats-card__value              /* 값 */
.stats-card__value--success     /* 성공 값 (초록색) */
.stats-card__value--error       /* 오류 값 (빨간색) */
.stats-card__label              /* 라벨 */
```

### Code
```css
.code-block                     /* 코드 블록 */
.text-mono                      /* 고정폭 폰트 */
```

## 💡 사용 예시

### Before (Tailwind 직접 사용)
```tsx
<div className="h-full bg-background p-6">
  <div className="max-w-7xl mx-auto">
    <h1 className="text-base font-semibold text-foreground flex items-center gap-2">
      제목
    </h1>
  </div>
</div>
```

### After (CSS 클래스 사용)
```tsx
<div className="tab-content">
  <div className="tab-content__container">
    <h1 className="tab-content__title">
      제목
    </h1>
  </div>
</div>
```

## 🎯 장점

1. **중앙 관리**: 모든 스타일이 `components.css`에 정의됨
2. **재사용성**: 동일한 클래스를 여러 컴포넌트에서 사용
3. **일관성**: 디자인 시스템 통일
4. **유지보수**: 스타일 변경 시 한 곳만 수정
5. **가독성**: 의미 있는 클래스 이름

## 🔧 스타일 수정 방법

### 1. 전역 색상 변경
`src/styles/index.css` 수정
```css
:root {
  --primary: 221 83% 53%;  /* 주요 색상 변경 */
}
```

### 2. 컴포넌트 스타일 변경
`src/styles/components.css` 수정
```css
.card {
  padding: var(--spacing-lg);  /* 패딩 조정 */
}
```

### 3. 새 클래스 추가
`src/styles/components.css`에 추가
```css
.my-custom-class {
  /* 스타일 정의 */
}
```

## 📋 네이밍 컨벤션

**BEM (Block Element Modifier)** 사용

```
.block                  /* 블록 */
.block__element         /* 요소 */
.block--modifier        /* 수정자 */
.block__element--modifier
```

예시:
```css
.card                   /* 블록 */
.card__header           /* 요소 */
.card--highlighted      /* 수정자 */
```



