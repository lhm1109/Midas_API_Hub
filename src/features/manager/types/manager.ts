// 상태 타입 (소문자 통일)
export type StatusType =
  | "empty"     // 비어있는원 - 시작 안함
  | "progress"  // 작업 중 (Work In Progress)
  | "done"      // 완료
  | "warning";  // 경고/문제

export interface ApiTask {
  id: string;
  product: string;
  tab: string;
  group: string;
  sub1: string;
  sub2: string;
  sub3: string;
  seg1: string;
  seg2: string;
  endPoint: string;
  mode: string;
  plan: StatusType;  // 새로 추가
  dev: StatusType;
  vv: StatusType;
  doc: StatusType;
  deploy: StatusType;
  issue: StatusType;
  status: string;  // cancel, done, progress, working, none 등
  charge: string;
  remark: string;
  linkedEndpointId?: string; // 프로젝트 탭 엔드포인트 연결
  order_index?: number; // 정렬 순서
  created_at?: string; // 생성 시간
  updated_at?: string; // 수정 시간
  // 커스텀 필드는 여기에 추가 가능
  [key: string]: string | StatusType | number | undefined;
}

export interface Column {
  id: string;
  label: string;
  visible: boolean;
}

export const defaultColumns: Column[] = [
  { id: "product", label: "Product", visible: true },
  { id: "tab", label: "Tab", visible: true },
  { id: "group", label: "Group", visible: true },
  { id: "sub1", label: "sub1", visible: true },
  { id: "sub2", label: "sub2", visible: true },
  { id: "sub3", label: "sub3", visible: true },
  { id: "seg1", label: "seg1", visible: true },
  { id: "seg2", label: "seg2", visible: true },
  { id: "endPoint", label: "End Point", visible: true },
  { id: "mode", label: "mode", visible: true },
  { id: "plan", label: "Plan", visible: true },  // dev 앞에 추가
  { id: "dev", label: "Dev", visible: true },
  { id: "vv", label: "V&V", visible: true },
  { id: "doc", label: "doc.", visible: true },
  { id: "deploy", label: "Deploy", visible: true },
  { id: "issue", label: "Issue", visible: true },
  { id: "status", label: "status", visible: true },
  { id: "charge", label: "charge", visible: true },
  { id: "remark", label: "remark", visible: true },
];
