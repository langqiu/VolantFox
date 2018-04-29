# terminal color settings
export PS1='\[\e[00;38;5;64m\]\u\[\e[00;38;5;64m\]@\[\e[00;38;5;64m\]\h\[\e[00;38;5;31m\]:\[\e[00;38;5;31m\]\w\[\e[00;38;5;244m\]\$ '

# coreutils
if brew list | grep coreutils > /dev/null ; then
  PATH="$(brew --prefix coreutils)/libexec/gnubin:$PATH"
  alias ls='ls -F --show-control-chars --color=auto'
  eval `gdircolors -b $HOME/.dir_colors`
fi

# 256colors
export TERM=xterm

# alias
#alias vi="vim"
alias ll="ls -l"
alias grep="grep --color"

# cd
cd ~/never_give_up
