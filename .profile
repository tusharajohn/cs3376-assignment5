# $Id: .profile,v 1.1 1998/05/02 21:20:02 amos Exp $

HOST=`uname -n`; export HOST

stty erase 

if [ $SHELL = "/bin/ksh" ] ; then
# Aliases
# Some familiar DOS commands
  alias   cls=clear
  alias   md=mkdir
  alias   rd=rmdir
  alias   del='rm -i'
  alias   erase='rm -i'

# Some so-I-won't-bash-my-head-into-the-wall-because-I-deleted-a-file commands
  alias   cp='cp -i'
  alias   mv='mv -i'
  alias   rm='rm -i'
  
# Some commands to make life easier
  alias   open='chmod go+r'
  alias   shut='chmod go-r'
  alias   fixlines='stty rows 24 cols 80;stty -tabs'

# Some   aliased ls commands
  alias   la='ls -a'
  alias   lc='ls -CF'
  alias   ll='ls -la'
  alias   ls='ls -F'
 
# Some help commands
  alias   help="man man"
  alias   key='man -k'
 
  alias   bye=logout
  alias   lo=logout

  alias   ldir="ls -l \$* | grep '^d'"


# Allows you to use the arrow keys to page through the command history.
# Must be in emacs mode to do this.
  alias __A=''		# Up arrow key
  alias __B=''		# Down arrow key
  alias __C=''		# Right arrow key
  alias __D=''		# Left arrow key
  set -o emacs
  PS1="`uname -n`[`logname`:\!] "

# functions
  function dir
  {
    if [ $# != 0 ] ; then
      ls -alF $* | more
    else 
      ls -alF ./ | more
    fi
  }

else
  PS1="`uname -n`[`logname`] "
fi

PS2=">"

# If you want to add your own paths in and still use the global
# login file set the path here.  For example,
#  PATH=$PATH:~/bin; export PATH
# will add ~/bin to the path.  The $PATH variable *must* be
# used here. 


if [ -f /usr/local/etc/skel/global/profile ] ; then
  echo Sourcing /usr/local/etc/skel/global/profile
  . /usr/local/etc/skel/global/profile
else
  echo "Could not find /usr/local/etc/skel/global/profile!!  Continuing..."
fi

# $Source: /warez/home/skel/src/RCS/.profile,v $
