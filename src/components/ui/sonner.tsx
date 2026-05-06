import { useEffect } from "react";
import { Toaster as Sonner, ToasterProps } from "sonner";

const Toaster = ({ toastOptions, closeButton, ...props }: ToasterProps) => {
  useEffect(() => {
    const handleToastClick = (event: MouseEvent) => {
      const target = event.target as HTMLElement | null;
      if (!target) return;

      // 액션/취소/입력 요소 클릭은 기존 동작 유지
      if (target.closest('[data-button], [data-close-button], a, button, input, textarea, select, label')) {
        return;
      }

      const toastElement = target.closest('[data-sonner-toast]') as HTMLElement | null;
      if (!toastElement) return;

      const closeBtn = toastElement.querySelector('[data-close-button]') as HTMLButtonElement | null;
      closeBtn?.click();
    };

    document.addEventListener('click', handleToastClick);
    return () => document.removeEventListener('click', handleToastClick);
  }, []);

  const mergedToastOptions: ToasterProps["toastOptions"] = {
    ...toastOptions,
    closeButton: toastOptions?.closeButton ?? true,
    classNames: {
      ...toastOptions?.classNames,
      toast: [toastOptions?.classNames?.toast, "cursor-pointer"].filter(Boolean).join(" "),
    },
  };

  return (
    <Sonner
      theme="dark"
      className="toaster group"
      closeButton={closeButton ?? true}
      toastOptions={mergedToastOptions}
      style={
        {
          "--normal-bg": "var(--popover)",
          "--normal-text": "var(--popover-foreground)",
          "--normal-border": "var(--border)",
        } as React.CSSProperties
      }
      {...props}
    />
  );
};

export { Toaster };
