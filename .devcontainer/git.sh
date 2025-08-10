#!/bin/bash
set -e

echo "[INFO] Running git.sh..."

echo "[SETUP] Configuring Git user..."
git config --global user.name "$GIT_NAME"
git config --global user.email "$GIT_EMAIL"

echo "[INFO] Git already configured:"
echo "       Name:  $(git config --global user.name)"
echo "       Email: $(git config --global user.email)"

if [ ! -f ~/.ssh/id_rsa ]; then
    echo "[SETUP] Generating new SSH key pair..."
    mkdir -p ~/.ssh
    chmod 700 ~/.ssh
    ssh-keygen -t rsa -b 4096 -C "$GIT_EMAIL" -f ~/.ssh/id_rsa -N ""
    chmod 644 ~/.ssh/id_rsa
fi

echo "[INFO] SSH key already exists at ~/.ssh/id_rsa"
echo "       Public key: $(cat ~/.ssh/id_rsa.pub)"
