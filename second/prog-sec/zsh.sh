#!/bin/bash

# Install zsh and zshrc
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

cat > /home/vagrant/.zshrc <<- EOM
ZSH_DISABLE_COMPFIX=true
export ZSH="/home/vagrant/.oh-my-zsh"
ZSH_THEME="robbyrussell"
plugins=(git)
source ~/.oh-my-zsh/oh-my-zsh.sh
EOM