#!/bin/zsh -xe

pull_from_upstream() {
  (
    set +x
    cd "$1"
    if [ "$(git branch --show-current)" = 'master' ]; then
      git pull upstream master
      git submodule update
    fi
  )
}

warn_branch() {
  (
    set +x
    for repo in "$@"; do
      local branch="$(git -C "$repo" branch --show-current)"
      if [ "$branch" != 'master' ]; then
        local repo_name="$(basename "$(readlink -f "$repo")")"
        echo -e "\x1b[1;33mWarning:\x1b[m \x1b[1;34m$repo_name\x1b[m is on branch \x1b[1;32m$branch\x1b[m." >&2
      fi
    done
  )
}


cd site_generator

pull_from_upstream .
pull_from_upstream cpprefjp/site
pull_from_upstream cpprefjp/image
./crsearch.json/docker.sh build
./crsearch.json/docker.sh run

pull_from_upstream kunai
kunai_timestamp=$(find kunai \( -path kunai/node_modules -o -path kunai/dist -o -path kunai/.git \) -prune -o -type f -exec stat -f '%m' {} + | sort -n | tail -1)
rss_timestamp=$(stat -f '%m' cpprefjp/cpprefjp.github.io/rss.xml)
if [ $kunai_timestamp -gt $rss_timestamp ]; then
  ./kunai/docker.sh build
  ./kunai/docker.sh install
  ./kunai/docker.sh dist
fi

if [ "$1" = 'nocache' ] && [ -f settings.cpprefjp_local.cache ]; then
  rm settings.cpprefjp_local.cache
fi

./docker.sh build
time ./docker.sh run settings.cpprefjp_local | tee ../log.txt
warn_branch . cpprefjp/site cpprefjp/image kunai
./docker.sh localhost cpprefjp
