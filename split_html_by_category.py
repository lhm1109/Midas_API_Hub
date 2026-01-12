import re
import os
from pathlib import Path
from bs4 import BeautifulSoup

def split_html_by_category(html_file):
    """HTML 파일을 카테고리별로 분리하여 저장"""
    
    print(f"HTML 파일 읽는 중: {html_file}")
    with open(html_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # BeautifulSoup으로 파싱
    soup = BeautifulSoup(content, 'html.parser')
    
    # 모든 h2 태그 찾기 (카테고리 제목)
    h2_tags = soup.find_all('h2')
    
    print(f"\n발견된 카테고리: {len(h2_tags)}개")
    categories = []
    for h2 in h2_tags:
        cat_id = h2.get('id', '')
        cat_name = h2.get_text().strip()
        categories.append((cat_id, cat_name, h2))
        print(f"  - {cat_name} (ID: {cat_id})")
    
    # 각 카테고리별로 처리
    for i, (cat_id, cat_name, h2_tag) in enumerate(categories):
        # INTRODUCTION은 건너뛰기
        if cat_name == "INTRODUCTION":
            continue
        
        print(f"\n{'='*60}")
        print(f"처리 중: {cat_name}")
        print(f"{'='*60}")
        
        # 카테고리별 폴더 생성
        category_folder = Path(f"api_docs/{cat_name}")
        category_folder.mkdir(parents=True, exist_ok=True)
        
        # 현재 h2 다음 요소들을 수집 (다음 h2가 나올 때까지)
        current_elements = []
        next_element = h2_tag.find_next_sibling()
        
        while next_element and next_element.name != 'h2':
            current_elements.append(next_element)
            next_element = next_element.find_next_sibling()
        
        # 테이블에서 API 링크 추출
        api_list = []
        for element in current_elements:
            # 테이블 찾기
            if element.name in ['div', 'table'] or (element.find('table')):
                table = element.find('table') if element.name == 'div' else element
                if table:
                    rows = table.find_all('tr')
                    for row in rows[1:]:  # 첫 행(헤더)은 건너뛰기
                        cells = row.find_all('td')
                        if len(cells) >= 3:
                            # No., Endpoint, Details
                            no_cell = cells[0].get_text().strip()
                            endpoint = cells[1].get_text().strip()
                            detail_cell = cells[2]
                            
                            # 링크와 제목 추출
                            link_tag = detail_cell.find('a')
                            if link_tag:
                                title = link_tag.get_text().strip()
                                href = link_tag.get('href', '')
                                api_list.append({
                                    'no': no_cell,
                                    'endpoint': endpoint,
                                    'title': title,
                                    'href': href,
                                    'row': row
                                })
        
        print(f"발견된 API: {len(api_list)}개\n")
        
        # h3 태그로 세부 카테고리 구분 (DB 카테고리의 경우)
        h3_sections = {}
        for element in current_elements:
            if element.name == 'h3':
                h3_id = element.get('id', '')
                h3_title = element.get_text().strip()
                h3_sections[h3_id] = {
                    'title': h3_title,
                    'content': []
                }
                current_h3_id = h3_id
            elif element.name in ['div', 'table', 'p'] and 'current_h3_id' in locals():
                if current_h3_id in h3_sections:
                    h3_sections[current_h3_id]['content'].append(element)
        
        # 각 API를 개별 HTML 파일로 저장
        for idx, api in enumerate(api_list, 1):
            # 파일명 생성
            safe_title = re.sub(r'[<>:"/\\|?*↗&]', '', api['title']).strip()
            safe_title = safe_title.replace(' ', '_')
            safe_title = safe_title.replace('(', '').replace(')', '')
            
            html_file_path = category_folder / f"{idx:03d}_{safe_title}.html"
            
            # HTML 파일 생성
            html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{api['title']} - {cat_name}</title>
    <style>
        body {{ font-family: Arial, sans-serif; max-width: 1200px; margin: 20px auto; padding: 0 20px; }}
        h1 {{ color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 10px; }}
        .info {{ background: #f8f9fa; padding: 15px; border-radius: 5px; margin: 20px 0; }}
        .info-item {{ margin: 5px 0; }}
        .label {{ font-weight: bold; color: #555; }}
        table {{ border-collapse: collapse; width: 100%; margin: 20px 0; }}
        th, td {{ border: 1px solid #ddd; padding: 12px; text-align: left; }}
        th {{ background-color: #3498db; color: white; }}
        tr:nth-child(even) {{ background-color: #f2f2f2; }}
    </style>
</head>
<body>
    <h1>{api['title']}</h1>
    <div class="info">
        <div class="info-item"><span class="label">Category:</span> {cat_name}</div>
        <div class="info-item"><span class="label">Endpoint:</span> {api['endpoint']}</div>
        <div class="info-item"><span class="label">No:</span> {api['no']}</div>
        <div class="info-item"><span class="label">Original URL:</span> {api['href']}</div>
    </div>
    
    <div class="api-content">
        {api['row'].prettify()}
    </div>
</body>
</html>
"""
            
            with open(html_file_path, 'w', encoding='utf-8') as f:
                f.write(html_content)
            
            print(f"[{idx}/{len(api_list)}] {api['title']} -> {html_file_path.name}")
        
        # 카테고리 요약 파일 생성
        summary_file = category_folder / "_summary.txt"
        with open(summary_file, 'w', encoding='utf-8') as f:
            f.write(f"Category: {cat_name}\n")
            f.write(f"Total APIs: {len(api_list)}\n")
            f.write(f"\nAPI List:\n")
            for idx, api in enumerate(api_list, 1):
                f.write(f"{idx}. {api['title']}\n")
                f.write(f"   Endpoint: {api['endpoint']}\n")
                f.write(f"   URL: {api['href']}\n\n")
        
        print(f"\n요약 파일 생성: {summary_file}")
        
        # 전체 카테고리 HTML 파일 생성
        category_html_file = category_folder / f"_ALL_{cat_name}.html"
        category_html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{cat_name} - All APIs</title>
    <style>
        body {{ font-family: Arial, sans-serif; max-width: 1400px; margin: 20px auto; padding: 0 20px; }}
        h1 {{ color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }}
        h2 {{ color: #34495e; margin-top: 30px; }}
        table {{ border-collapse: collapse; width: 100%; margin: 20px 0; }}
        th, td {{ border: 1px solid #ddd; padding: 12px; text-align: left; }}
        th {{ background-color: #3498db; color: white; }}
        tr:nth-child(even) {{ background-color: #f2f2f2; }}
    </style>
</head>
<body>
    <h1>{cat_name}</h1>
    <p>Total APIs: {len(api_list)}</p>
"""
        
        # 모든 요소를 HTML에 추가
        for element in current_elements:
            category_html_content += str(element) + "\n"
        
        category_html_content += """
</body>
</html>
"""
        
        with open(category_html_file, 'w', encoding='utf-8') as f:
            f.write(category_html_content)
        
        print(f"전체 카테고리 HTML 생성: {category_html_file}")

if __name__ == "__main__":
    html_file = "doc/main.html"
    
    if not os.path.exists(html_file):
        print(f"Error: {html_file} 파일을 찾을 수 없습니다.")
    else:
        print("="*60)
        print("HTML 파일을 카테고리별로 분리하는 중...")
        print("="*60)
        split_html_by_category(html_file)
        print("\n" + "="*60)
        print("완료! api_docs 폴더를 확인하세요.")
        print("="*60)

