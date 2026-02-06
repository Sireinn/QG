@echo off
chcp 65001 > nul
echo Committing changes to Git...

:: 使用PowerShell获取标准格式的时间
for /f "delims=" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd HH:mm:ss'"') do set datetime=%%a

:: 切换到项目目录（修改为新的文件夹名）
cd /d "D:\obsidian\知识库\QG"
git add .

:: 提交信息改为英文，避免编码问题
git commit -m "Auto commit: %datetime%"
echo Pushing to GitHub...

:: 推送到远程仓库（明确指定分支）
git push origin main

echo.
echo Operation completed!
pause