#!/bin/bash

git status

echo 'enter commitMsg'
read commitMsg

# Use current date time in the commit message when its not passed
if [[ -z "${message// }" ]]
    then
        message=$(date '+%Y-%m-%d %H:%M:%S')
fi

# stage all changes
git add .
echo "Staged all files"

# add commit
git commit -m "$message"
echo "Added the commit with message: '$message'"

git push -u origin main 
echo "Push changes to main branch"


