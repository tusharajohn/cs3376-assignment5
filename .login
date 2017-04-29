# $Id: .login,v 1.1 1998/05/02 21:25:11 amos Exp $
# Used by csh and tcsh

# To pick a default editor besides pico uncomment one of the following lines
#setenv	EDITOR	vi
#setenv EDITOR	emacs

# Don't generate a core file
limit	coredumpsize	0

#stty intr ^c kill ^u
#stty start ^q stop ^s susp ^z

# I like my default window manager to be fvwm
#setenv startwindows fvwm

cd

# If you want to add your own paths in and still use the global
# login file set the path here.  For example,
#   set path = ( $path ~/bin )
# will add ~/bin to the path.  The $path variable *must* be
# used here.

# This allows the global variables to be set up. Do not erase.
if ( -f /usr/local/etc/skel/global/login ) then 
  echo Sourcing /usr/local/etc/skel/global/login
  source /usr/local/etc/skel/global/login
else
  echo Could not find /usr/local/etc/skel/global/login\!\!  Continuing...
endif

# $Source: /warez/home/skel/src/RCS/.login,v $
