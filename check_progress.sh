#!/bin/bash
# Zendesk 다운로드 진행 상황 모니터링

echo "=== Zendesk API 다운로드 진행 상황 ==="
echo ""

# 각 카테고리별 파일 개수 확인
echo "📁 카테고리별 다운로드 상황:"
for dir in api_docs_zendesk/*/; do
    if [ -d "$dir" ]; then
        cat_name=$(basename "$dir")
        html_count=$(find "$dir" -name "*.html" | wc -l)
        echo "  - $cat_name: $html_count 개"
    fi
done

echo ""
echo "📊 전체 통계:"
total_html=$(find api_docs_zendesk -name "*.html" | wc -l)
total_error=$(find api_docs_zendesk -name "*_ERROR.txt" | wc -l)
echo "  - 총 HTML 파일: $total_html 개"
echo "  - 에러 파일: $total_error 개"

echo ""
echo "🔄 Python 프로세스 상태:"
ps aux | grep "download_from_zendesk.py" | grep -v grep || echo "  실행 중인 프로세스 없음"

echo ""
echo "📄 최근 로그 (마지막 20줄):"
if [ -f "zendesk_download.log" ]; then
    tail -20 zendesk_download.log
else
    echo "  로그 파일 없음"
fi










