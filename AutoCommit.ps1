# 切换到项目目录
Set-Location "D:\obsidian\知识库\QG寒假训练营"

# 添加所有更改到暂存区
git add .

# 提交更改，使用当前时间作为提交信息
$commitMessage = "自动提交: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
git commit -m $commitMessage

# 推送到远程仓库
git push

# 提示完成
Write-Host "提交和推送完成！" -ForegroundColor Green