import { cn } from "@/components/ui/utils";

interface CategoryBadgeProps {
  category: string;
  className?: string;
}

const categoryColors: Record<string, string> = {
  doc: "bg-blue-600 text-white",
  db: "bg-purple-600 text-white",
  ope: "bg-green-600 text-white",
  view: "bg-yellow-600 text-white",
  post: "bg-orange-600 text-white",
  design: "bg-pink-600 text-white",
};

export function CategoryBadge({ category, className }: CategoryBadgeProps) {
  if (!category) return null;

  const colorClass = categoryColors[category.toLowerCase()] || "bg-zinc-700 text-zinc-300";

  return (
    <div
      className={cn(
        "inline-flex items-center px-2 py-0.5 rounded-md text-xs font-medium",
        colorClass,
        className
      )}
    >
      {category}
    </div>
  );
}
