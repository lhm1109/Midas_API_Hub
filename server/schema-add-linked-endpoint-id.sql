-- Add linked_endpoint_id column to manager_tasks table
-- This column stores the ID of the linked project endpoint

ALTER TABLE manager_tasks 
ADD COLUMN IF NOT EXISTS linked_endpoint_id TEXT DEFAULT NULL;

-- Add comment for documentation
COMMENT ON COLUMN manager_tasks.linked_endpoint_id IS 'ID of the linked project endpoint for quick navigation';
