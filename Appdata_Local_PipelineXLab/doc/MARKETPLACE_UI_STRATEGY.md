# Marketplace UI Strategy (마켓플레이스 UI 구현 전략)

## 1. Overview (개요)
To implement a Marketplace tab where users can explore, download, and purchase components, workflows, and automation tools.
Based on the existing `MARKETPLACE_GUIDE.md`, this document defines the user-centric UI/UX strategy.

## 2. Key Requirements (핵심 요구사항)
- **Language**: System responses in Korean, UI text in **English**.
- **Structure**:
    - **Top**: Event Banner (Carousel) - horizontal scrolling.
    - **Sections**: Free vs. Paid (Premium).
- **Paid Content Scope**: 3rd party components, Workflow automation requests, etc.
- **User Info**: Creator nickname, profile picture, ratings.

## 3. UI Structure (UI 구조)

### 3.1 Layout (레이아웃)
```
+-------------------------------------------------------+
|  [Header] Search Bar | Filter | My Points/Credits     |
+-------------------------------------------------------+
|  [Banner Carousel]                                    |
|  <  [Event Banner 1] [Event Banner 2] [Event...]   >  |
+-------------------------------------------------------+
|                                                       |
|  [Featured / Free Section]                            |
|  +--------+  +--------+  +--------+                   |
|  | Item 1 |  | Item 2 |  | Item 3 | ...               |
|  +--------+  +--------+  +--------+                   |
|                                                       |
|  [Premium / Paid Section]                             |
|  +--------+  +--------+  +--------+                   |
|  | Paid 1 |  | Paid 2 |  | Paid 3 | ...               |
|  +--------+  +--------+  +--------+                   |
|                                                       |
+-------------------------------------------------------+
```

### 3.2 Component Design (컴포넌트 디자인)

#### **Banner Component**
- **Function**: Promote key events, discounts, new features.
- **Interaction**: Auto-play, Left/Right navigation arrows.

#### **Marketplace Item Card**
- **Thumbnail**: Icon or preview image.
- **Title**: Component name.
- **Creator**: Profile image (Avatar) + Nickname.
- **Rating**: Star rating (e.g., ⭐ 4.5/5.0).
- **Price**: "Free" or Price tag ($/₩).
- **Category Badge**: Workflow, Component, Automation, etc.

## 4. Data Models (데이터 모델 - Frontend)

```typescript
interface MarketplaceItem {
  id: string;
  title: string;
  description: string;
  thumbnailUrl: string;
  type: 'component' | 'workflow' | 'automation_request';
  price: number; // 0 for free
  currency: string;
  isPaid: boolean;
  rating: number;
  reviewCount: number;
  publisher: SchemaPublisher;
}

interface SchemaPublisher {
  id: string;
  nickname: string;
  avatarUrl: string;
  isVerified: boolean;
}

interface BannerItem {
    id: string;
    imageUrl: string;
    linkUrl: string;
    title: string;
}
```

## 5. Implementation Steps (구현 단계)

1.  **Skeleton Setup**: Modify `MarketplaceTab.tsx` to establish basic scroll layout.
2.  **Mock Data**: Create `mockMarketplaceData.ts` for frontend development.
3.  **Banner Implementation**: Implement Carousel (Shadcn/Embla).
4.  **Item Card Implementation**: Reusable Card component with new fields (Rating, Creator).
5.  **Section Layout**: Horizontal lists for Free/Paid sections.
6.  **Responsive Check**: Ensure layouts work on different window sizes.

## 6. Libraries (라이브러리)
- UI: Shadcn UI (already in use).
- Icons: Lucide React.
- Carousel: `embla-carousel-react` (standard in Shadcn).
