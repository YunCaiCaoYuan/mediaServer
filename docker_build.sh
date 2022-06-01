docker run --rm -it \
   --mount type=bind,source=/home/ubuntu/media-server-go,target=/media-server-go \
   ubuntu:18.04 \
   bash
