// 🎯 무한 확장성을 위한 설정 기반 아키텍처 예시
// 이 파일은 참고용 예시입니다. 실제로 구현하려면 추가 작업이 필요합니다.

import { lazy, ComponentType } from 'react';
import { LucideIcon } from 'lucide-react';

/**
 * Feature 설정 인터페이스
 * 새로운 최상위 탭(feature)의 메타데이터를 정의합니다.
 */
export interface FeatureConfig {
  /** 고유 식별자 */
  id: string;
  
  /** 표시될 이름 */
  name: string;
  
  /** 아이콘 컴포넌트 */
  icon: LucideIcon;
  
  /** 지연 로딩될 컴포넌트 */
  component: React.LazyExoticComponent<ComponentType<any>>;
  
  /** 표시 순서 */
  order: number;
  
  /** 활성화 여부 */
  enabled: boolean;
  
  /** 권한 (선택사항) */
  permission?: string;
}

/**
 * Tab 설정 인터페이스
 * Feature 내부의 하위 탭을 정의합니다.
 */
export interface TabConfig {
  /** 고유 식별자 */
  id: string;
  
  /** 표시될 이름 */
  name: string;
  
  /** 아이콘 컴포넌트 */
  icon: LucideIcon;
  
  /** 지연 로딩될 컴포넌트 */
  component: React.LazyExoticComponent<ComponentType<any>>;
  
  /** 표시 순서 */
  order: number;
  
  /** 버전 선택이 필요한지 여부 */
  requiresVersion?: boolean;
  
  /** 활성화 여부 */
  enabled?: boolean;
}

/**
 * 예시: Feature 설정
 * 
 * 사용 방법:
 * 1. 새 feature 추가 시 이 배열에 설정만 추가
 * 2. GlobalSidebar가 자동으로 버튼 생성
 * 3. App.tsx가 자동으로 라우팅 처리
 */
/*
import { FolderOpen, History, BarChart } from 'lucide-react';

export const FEATURES: FeatureConfig[] = [
  {
    id: 'projects',
    name: 'Projects',
    icon: FolderOpen,
    component: lazy(() => import('@/features/projects/components')),
    order: 1,
    enabled: true,
  },
  {
    id: 'history',
    name: 'History',
    icon: History,
    component: lazy(() => import('@/features/history/components')),
    order: 2,
    enabled: true,
  },
  // 새 탭 추가 예시:
  {
    id: 'analytics',
    name: 'Analytics',
    icon: BarChart,
    component: lazy(() => import('@/features/analytics/components')),
    order: 3,
    enabled: true,
    permission: 'analytics.view', // 권한 체크 (선택사항)
  },
];
*/

/**
 * 예시: Projects Feature의 하위 탭 설정
 * 
 * 사용 방법:
 * 1. 새 하위 탭 추가 시 이 배열에 설정만 추가
 * 2. ProjectsView가 자동으로 탭 생성
 */
/*
import { GitBranch, BookOpen, FileText, Wrench, Rocket } from 'lucide-react';

export const PROJECT_TABS: TabConfig[] = [
  {
    id: 'version',
    name: 'Version',
    icon: GitBranch,
    component: lazy(() => import('./tabs/VersionTab')),
    order: 1,
    requiresVersion: false,
  },
  {
    id: 'manual',
    name: 'Manual',
    icon: BookOpen,
    component: lazy(() => import('./tabs/ManualTab')),
    order: 2,
    requiresVersion: true,
  },
  {
    id: 'spec',
    name: 'Spec',
    icon: FileText,
    component: lazy(() => import('./tabs/SpecTab')),
    order: 3,
    requiresVersion: true,
  },
  {
    id: 'builder',
    name: 'Builder',
    icon: Wrench,
    component: lazy(() => import('./tabs/BuilderTab')),
    order: 4,
    requiresVersion: true,
  },
  {
    id: 'runner',
    name: 'Runner',
    icon: Rocket,
    component: lazy(() => import('./tabs/RunnerTab')),
    order: 5,
    requiresVersion: true,
  },
];
*/

/**
 * 이 방식의 장점:
 * 1. ✅ 새 탭 추가 시 설정만 변경
 * 2. ✅ 타입 안정성 유지
 * 3. ✅ 지연 로딩으로 성능 최적화
 * 4. ✅ 순서, 활성화 여부 쉽게 변경
 * 5. ✅ 권한 체크 통합 가능
 * 
 * 구현하려면:
 * - GlobalSidebar를 설정 기반으로 리팩토링
 * - App.tsx를 동적 라우팅으로 변경
 * - ProjectsView를 설정 기반으로 변경
 */

export {};







