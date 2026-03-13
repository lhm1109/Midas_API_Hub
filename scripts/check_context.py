import openpyxl

def is_new(row):
    fill = row[0].fill
    return fill and fill.patternType=='solid' and fill.fgColor and fill.fgColor.rgb.upper().endswith('FFE0E0')

wb = openpyxl.load_workbook(r'C:\MIDAS\code\apiverification\manager_tasks_2026-03-12.xlsx')
ws = wb['Tasks']

rows_data = []
for row in ws.iter_rows(min_row=3):
    ep = str(row[11].value).strip() if row[11].value else ''
    rows_data.append((row[0].row, ep, is_new(row)))

check_eps = ['/DB/LCOM-ALUM','/DB/LCOM-CFSTEEL','/DB/LCOM-FDN','/DB/LCOM-LINEAR',
             '/DB/MVHL','/DB/MVHLTR','/DB/SLANCH','/DB/SLANOP',
             '/DB/CONS','/DB/CUTL','/DB/DCTL']
for target_ep in check_eps:
    for i, (rn, ep, isnew) in enumerate(rows_data):
        if ep.upper() == target_ep.upper():
            print('--- %s ---' % target_ep)
            for j in range(max(0,i-2), min(len(rows_data),i+3)):
                r2, e2, n2 = rows_data[j]
                mark = ' [NEW]' if n2 else ''
                arrow = ' <<<' if j==i else ''
                print('  Row%4d  %s%s%s' % (r2, e2 if e2 else '(empty)', mark, arrow))
            break
