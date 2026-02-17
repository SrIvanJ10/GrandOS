#!/bin/bash

lb config \
  --architecture amd64 \
  --distribution bookworm \
  --binary-image iso-hybrid \
  --archive-areas "main" \
  --bootappend-live "boot=live components locales=es_ES.UTF-8 keyboard-layouts=es" \
  --image-name "GrandOS" \
  --debian-installer none \
  --memtest none \
  --apt-recommends false \
  --apt-indices false
