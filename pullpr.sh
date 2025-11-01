#!/bin/zsh -x

id=$1

cd site_generator/cpprefjp/site
git fetch upstream pull/$id/head:pr-$id
git switch pr-$id
git pull upstream pull/$id/head
