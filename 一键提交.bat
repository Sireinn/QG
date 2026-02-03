@echo off
chcp 65001 > nul
echo 正在提交更改到Git...
cd /d "D:\obsidian\知识库\QG寒假训练营"
git add .
git commit -m "自动提交: %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,8%"
echo 正在推送到GitHub...
git push
echo.
echo 操作完成！
pause