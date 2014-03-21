air_mod
=======

git stuff for your convenience (remove if you don't need):
  git config branch.master.remote origin
  git config branch.master.merge refs/heads/master

how to run:
  cd <OUR_PROJECT_FOLDER>
  source setup.sh
  cd <catkin_ws_root>
  catkin_make install
  air_mod_run.sh 2500

setup.sh 
1. sources catkin_ws/devel/setup.sh, so you don't have to add it to your .bashrc file
2. adds the scripts subdirectory to the path, and catkin_make install generates a custom run script for your computer
