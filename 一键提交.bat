@echo off
chcp 65001 > nul
echo Committing changes to Git...

:: 使用PowerShell获取标准格式的时间
for /f "delims=" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd HH:mm:ss'"') do set "datetime=%%a"

cd /d "D:\obsidian\知识库\QG寒假训练营"
git add .
:: 提交信息改为英文，避免编码问题
git commit -m "Auto commit: %datetime%"
echo Pushing to GitHub...
git push
echo.
echo Operation completed!
pause