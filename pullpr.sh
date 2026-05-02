#!/bin/zsh -x

id=$1

cd site_generator/cpprefjp/site
git switch master
git branch -D pr-$id || true 
git fetch upstream pull/$id/head:pr-$id
git switch pr-$id
