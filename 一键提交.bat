@echo off
chcp 65001 > nul
echo 正在提交更改到Git...

:: 使用PowerShell获取标准格式的时间，避免乱码
for /f "delims=" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd HH:mm:ss'"') do set "datetime=%%a"

cd /d "D:\obsidian\知识库\QG寒假训练营"
git add .
git commit -m "自动提交: %datetime%"
echo 正在推送到GitHub...
git push
echo.
echo 操作完成！
pause