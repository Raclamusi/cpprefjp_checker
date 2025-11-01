#!/bin/zsh -xe

cd site_generator

git pull upstream master
git submodule update

(cd cpprefjp/site && git pull upstream master)
(cd cpprefjp/image && git pull)
./crsearch.json/docker.sh run

(cd kunai && git pull upstream master)
kunai_timestamp=$(find kunai \( -path kunai/node_modules -o -path kunai/dist -o -path kunai/.git \) -prune -o -type f -exec stat -f '%m' {} + | sort -n | tail -1)
rss_timestamp=$(stat -f '%m' cpprefjp/cpprefjp.github.io/rss.xml)
if [ $kunai_timestamp -gt $rss_timestamp ]; then
  ./kunai/docker.sh run build
fi

if [ "$1" = 'nocache' ] && [ -f settings.cpprefjp_local.cache ]; then
  rm settings.cpprefjp_local.cache
fi

./docker.sh run settings.cpprefjp_local --concurrency=16 | tee ../log.txt
./docker.sh localhost cpprefjp
