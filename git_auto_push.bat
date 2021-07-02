cd /d
cd "/d/My Documents/Programming/Projects_Arduino/"
git init
git add .
git commit -m "upload"
git remote add origin https://github.com/spspider/Arduino.git
git pull --rebase
git fetch origin
git branch --set-upstream-to=origin/master master
git pull --allow-unrelated-histories
git push origin --all

