#!/bin/zsh -x

cd site_generator/cpprefjp/site
git switch master
git pull upstream master
git branch -D $(git branch | grep 'pr-')
