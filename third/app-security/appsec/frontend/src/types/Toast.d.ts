export type ToastType = "primary" | "success" | "error";

export type Toast = {
  id: number;
  message: string;
  remove(): any;
  pause(): any;
  resume(): any;
  type: ToastType;
  progress: number;
};
