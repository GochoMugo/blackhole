#!/usr/bin/env bash

export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

export SSH_AGENT_PID=$(pgrep -U "${USER}" ssh-agent)
export SSH_AUTH_SOCK="$(lsof -a -p "${SSH_AGENT_PID}" -U -F n | sort | uniq | sed -n 's/^n//p')"

echo "SSH_AGENT_PID=${SSH_AGENT_PID}"
echo "SSH_AUTH_SOCK=${SSH_AUTH_SOCK}"

${*}
