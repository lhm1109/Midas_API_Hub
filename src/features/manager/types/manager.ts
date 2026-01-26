export type StatusType =
  | "green" // 초록불 - 완료
  | "yellow" // 노랑불 - 진행 중
  | "red" // 빨강불 - 문제
  | "empty" // 비어있는원 - 시작 안함
  | "working" // Working 스핀 - 작업 중
  | "done" // Done 체크 - 완료
  | "warning" // 주의판 - 경고
  | "na"; // N/A - 해당없음

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
  dev: StatusType;
  vv: StatusType;
  doc: StatusType;
  issue: StatusType;
  status: "Working" | "Done" | string;
  charge: string;
  remark: string;
  // 커스텀 필드는 여기에 추가 가능
  [key: string]: string | StatusType;
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
  { id: "dev", label: "Dev", visible: true },
  { id: "vv", label: "V&V", visible: true },
  { id: "doc", label: "doc.", visible: true },
  { id: "issue", label: "Issue", visible: true },
  { id: "status", label: "status", visible: true },
  { id: "charge", label: "charge", visible: true },
  { id: "remark", label: "remark", visible: true },
];
