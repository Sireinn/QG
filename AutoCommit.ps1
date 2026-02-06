# 切换到项目目录（修改为新的文件夹名）
Set-Location "D:\obsidian\知识库\QG"

# 添加所有更改到暂存区
git add .

# 提交更改，使用当前时间作为提交信息
$commitMessage = "自动提交: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
git commit -m $commitMessage

# 推送到远程仓库（将本地master推送到远程main）
git push origin master:main

# 提示完成
Write-Host "提交和推送完成！" -ForegroundColor Green