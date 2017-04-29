# $Id: .bash_profile,v 1.1 1998/05/02 21:14:25 amos Exp $
# Personal initialization file, executed for login shells

# If you want to add your own paths in and still use the global
# login file set the path here.  For example,
#   PATH=$PATH:~/bin; export PATH
# will add ~/bin to the path.  The $PATH variable *must* be
# used here. 


# Source global .profile
if [ -f /usr/local/etc/skel/global/profile ] ; then
  echo Sourcing /usr/local/etc/skel/global/profile
  . /usr/local/etc/skel/global/profile
else
  echo "Could not find /usr/local/etc/skel/global/profile!!  Continuing..."
fi

if [ -f $HOME/.bashrc ] ; then 
  source ~/.bashrc
fi

cd

# $Source: /warez/home/skel/src/RCS/.bash_profile,v $
