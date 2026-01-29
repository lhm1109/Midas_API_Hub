// 상태 타입 (단순화됨)
export type StatusType =
  | "empty"   // 비어있는원 - 시작 안함
  | "wip"     // 작업 중 (Work In Progress)
  | "done"    // 완료
  | "warning" // 경고/문제
  | "na";     // N/A - 해당없음

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
  issue: StatusType;
  status: "In Progress" | "Done" | string;  // Working 제거
  charge: string;
  remark: string;
  linkedEndpointId?: string; // 프로젝트 탭 엔드포인트 연결
  // 커스텀 필드는 여기에 추가 가능
  [key: string]: string | StatusType | undefined;
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
  { id: "issue", label: "Issue", visible: true },
  { id: "status", label: "status", visible: true },
  { id: "charge", label: "charge", visible: true },
  { id: "remark", label: "remark", visible: true },
];
