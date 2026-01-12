import re
import os
import time
import requests
from pathlib import Path
from requests.auth import HTTPBasicAuth

# Zendesk 계정 정보
ZENDESK_EMAIL = "mit_structure@midasit.com"
ZENDESK_PASSWORD = "mid@sit0902"
ZENDESK_BASE_URL = "https://midas-support.zendesk.com/api/v2/help_center"

def get_article_content(article_id, retry_count=3):
    """Zendesk API로 특정 article의 내용 가져오기 (재시도 포함)"""
    url = f"{ZENDESK_BASE_URL}/en-us/articles/{article_id}.json"
    
    for attempt in range(retry_count):
        try:
            response = requests.get(
                url,
                auth=HTTPBasicAuth(ZENDESK_EMAIL, ZENDESK_PASSWORD),
                timeout=30
            )
            response.raise_for_status()
            data = response.json()
            return data.get('article', {})
        except requests.exceptions.Timeout:
            if attempt < retry_count - 1:
                print(f"    ⚠ 타임아웃, 재시도 {attempt + 1}/{retry_count}...")
                time.sleep(2)
            else:
                print(f"    ✗ 타임아웃 (최대 재시도 초과)")
                return None
        except requests.exceptions.RequestException as e:
            if attempt < retry_count - 1:
                print(f"    ⚠ 오류 발생, 재시도 {attempt + 1}/{retry_count}...")
                time.sleep(2)
            else:
                print(f"    ✗ API 요청 실패: {e}")
                return None
        except Exception as e:
            print(f"    ✗ 예상치 못한 오류: {e}")
            return None
    
    return None

def download_apis_from_zendesk(html_file):
    """HTML 파일에서 article ID를 추출하고 Zendesk API로 실제 내용 다운로드"""
    
    print(f"HTML 파일 읽는 중: {html_file}")
    with open(html_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 카테고리 패턴 찾기 (h2 태그)
    category_pattern = r'<h2 id="([^"]+)">([^<]+)</h2>'
    categories = re.findall(category_pattern, content)
    
    print(f"\n발견된 카테고리: {len(categories)}개")
    for cat_id, cat_name in categories:
        print(f"  - {cat_name} (ID: {cat_id})")
    
    # 각 카테고리의 시작 위치 찾기
    category_positions = []
    for cat_id, cat_name in categories:
        match = re.search(f'<h2 id="{cat_id}">{cat_name}</h2>', content)
        if match:
            category_positions.append((cat_name, match.start(), match.end()))
    
    # 마지막에 끝 위치 추가
    category_positions.append(("END", len(content), len(content)))
    
    # 각 카테고리 섹션 처리
    for i in range(len(category_positions) - 1):
        cat_name, start_pos, _ = category_positions[i]
        next_start_pos = category_positions[i + 1][1]
        
        # INTRODUCTION은 건너뛰기
        if cat_name == "INTRODUCTION":
            continue
        
        print(f"\n{'='*60}")
        print(f"처리 중: {cat_name}")
        print(f"{'='*60}")
        
        # 카테고리 섹션 추출
        section = content[start_pos:next_start_pos]
        
        # 링크 패턴 찾기: href와 텍스트 모두 추출
        link_pattern = r'<a[^>]*href="([^"]+)"[^>]*>([^<]+)</a>'
        links = re.findall(link_pattern, section)
        
        print(f"발견된 링크: {len(links)}개\n")
        
        # 카테고리별 폴더 생성
        category_folder = Path(f"api_docs_zendesk/{cat_name}")
        category_folder.mkdir(parents=True, exist_ok=True)
        
        # 각 링크에 대해 처리
        api_list = []
        for idx, (url, title) in enumerate(links, 1):
            # Article ID 추출
            article_id_match = re.search(r'/articles/(\d+)', url)
            if not article_id_match:
                print(f"[{idx}/{len(links)}] {title} - Article ID를 찾을 수 없음, 건너뜀")
                continue
            
            article_id = article_id_match.group(1)
            
            # 파일명에서 특수문자 제거
            safe_title = re.sub(r'[<>:"/\\|?*↗&]', '', title).strip()
            safe_title = safe_title.replace(' ', '_')
            safe_title = safe_title.replace('(', '').replace(')', '')
            
            # HTML 파일로 저장
            html_file_path = category_folder / f"{idx:03d}_{safe_title}.html"
            
            # 이미 다운로드된 파일은 건너뛰기
            if html_file_path.exists():
                print(f"[{idx}/{len(links)}] {title} - 이미 존재함, 건너뜀")
                # API 목록에 추가 (요약 파일을 위해)
                api_list.append({
                    'title': title,
                    'article_id': article_id,
                    'url': f"https://support.midasuser.com/hc/en-us/articles/{article_id}",
                    'section_id': 'N/A',
                    'created_at': 'N/A',
                    'updated_at': 'N/A'
                })
                continue
            
            print(f"[{idx}/{len(links)}] {title}")
            print(f"    Article ID: {article_id}")
            
            # Zendesk API로 문서 내용 가져오기
            article_data = get_article_content(article_id)
            
            if article_data:
                # HTML 파일 생성
                html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{article_data.get('title', title)} - {cat_name}</title>
    <style>
        body {{ font-family: Arial, sans-serif; max-width: 1200px; margin: 20px auto; padding: 0 20px; line-height: 1.6; }}
        h1 {{ color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }}
        .info {{ background: #f8f9fa; padding: 15px; border-radius: 5px; margin: 20px 0; border-left: 4px solid #3498db; }}
        .info-item {{ margin: 8px 0; }}
        .label {{ font-weight: bold; color: #555; display: inline-block; width: 150px; }}
        .content {{ margin-top: 30px; }}
        table {{ border-collapse: collapse; width: 100%; margin: 20px 0; }}
        th, td {{ border: 1px solid #ddd; padding: 12px; text-align: left; }}
        th {{ background-color: #3498db; color: white; }}
        tr:nth-child(even) {{ background-color: #f2f2f2; }}
        img {{ max-width: 100%; height: auto; }}
        code {{ background: #f4f4f4; padding: 2px 6px; border-radius: 3px; }}
        pre {{ background: #f4f4f4; padding: 15px; border-radius: 5px; overflow-x: auto; }}
    </style>
</head>
<body>
    <h1>{article_data.get('title', title)}</h1>
    
    <div class="info">
        <div class="info-item"><span class="label">Category:</span> {cat_name}</div>
        <div class="info-item"><span class="label">Article ID:</span> {article_id}</div>
        <div class="info-item"><span class="label">Created:</span> {article_data.get('created_at', 'N/A')}</div>
        <div class="info-item"><span class="label">Updated:</span> {article_data.get('updated_at', 'N/A')}</div>
        <div class="info-item"><span class="label">Author ID:</span> {article_data.get('author_id', 'N/A')}</div>
        <div class="info-item"><span class="label">Section ID:</span> {article_data.get('section_id', 'N/A')}</div>
        <div class="info-item"><span class="label">URL:</span> <a href="{article_data.get('html_url', '#')}" target="_blank">{article_data.get('html_url', 'N/A')}</a></div>
    </div>
    
    <div class="content">
        {article_data.get('body', '<p>내용을 불러올 수 없습니다.</p>')}
    </div>
</body>
</html>
"""
                
                with open(html_file_path, 'w', encoding='utf-8') as f:
                    f.write(html_content)
                
                print(f"    ✓ 저장 완료: {html_file_path.name}")
                
                # API 목록에 추가
                api_list.append({
                    'title': article_data.get('title', title),
                    'article_id': article_id,
                    'url': article_data.get('html_url', ''),
                    'section_id': article_data.get('section_id', ''),
                    'created_at': article_data.get('created_at', ''),
                    'updated_at': article_data.get('updated_at', '')
                })
                
                # API 속도 제한 방지 (더 긴 대기 시간)
                time.sleep(1)
            else:
                # 에러 정보를 파일에 저장
                error_file = category_folder / f"{idx:03d}_{safe_title}_ERROR.txt"
                with open(error_file, 'w', encoding='utf-8') as f:
                    f.write(f"Title: {title}\n")
                    f.write(f"Article ID: {article_id}\n")
                    f.write(f"Error: API 요청 실패\n")
                print(f"    ✗ 에러 파일 생성: {error_file.name}")
        
        # 카테고리 요약 파일 생성
        if api_list:
            summary_file = category_folder / "_summary.txt"
            with open(summary_file, 'w', encoding='utf-8') as f:
                f.write(f"Category: {cat_name}\n")
                f.write(f"Total APIs: {len(api_list)}\n")
                f.write(f"\nAPI List:\n")
                for idx, api in enumerate(api_list, 1):
                    f.write(f"{idx}. {api['title']}\n")
                    f.write(f"   Article ID: {api['article_id']}\n")
                    f.write(f"   Section ID: {api['section_id']}\n")
                    f.write(f"   URL: {api['url']}\n")
                    f.write(f"   Updated: {api['updated_at']}\n\n")
            
            print(f"\n요약 파일 생성: {summary_file}")

if __name__ == "__main__":
    html_file = "doc/main.html"
    
    if not os.path.exists(html_file):
        print(f"Error: {html_file} 파일을 찾을 수 없습니다.")
    else:
        print("="*60)
        print("Zendesk API를 사용한 문서 다운로드 시작")
        print("="*60)
        download_apis_from_zendesk(html_file)
        print("\n" + "="*60)
        print("완료! api_docs_zendesk 폴더를 확인하세요.")
        print("="*60)

