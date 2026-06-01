# POST Result Table Supabase Import Report

Date: 2026-05-29

## Scope

- Source: `api_data_set/POST`
- Product: `verification`
- Group: `verification_result_table` (`Result Table`)
- Version name: `LATEST API MANUAL`
- Imported endpoints: 130
- Excluded source document: `001_Designing_with_Intent_The_Vision_Behind_POSTTABLE` (POST/TABLE overview, not an endpoint)

## Backup

- Backup completed before DB writes.
- Backup path: `backups/supabase-data-2026-05-29T00-49-10`

## Result

- Success: 130 endpoints
- Failed: 0 endpoints
- Schemas with `TABLE_TYPE`: 128
- Schemas with `COMPONENTS`: 128

## Component Notes

`COMPONENTS` enums were generated from request `COMPONENTS` values when present, otherwise from response `HEAD` values in the old Zendesk HTML. `Index` was excluded from selectable components.

Two endpoints do not include `COMPONENTS` because their source manuals do not provide result table component headers:

- `122_P-M_Interaction_Diagram`: empty `Argument`, no response `HEAD`
- `123_Steel_Code_Check`: empty `Argument`, response arrays without result table `HEAD`

## Verification

Sample endpoint checked after import:

- Endpoint: `post-table-element-weight-table`
- Path: `/post/TABLE`
- Version: `LATEST API MANUAL`
- Spec keys: `TABLE_NAME`, `TABLE_TYPE`, `EXPORT_PATH`, `UNIT`, `STYLES`, `NODE_ELEMS`, `COMPONENTS`
- Components: `No`, `Type`, `Name`, `Value`, `UnitWeight`, `TotalWeight`
- Manual HTML retained: yes
- Manual old `TABLE` wrapper retained: yes
