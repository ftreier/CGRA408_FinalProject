#!/bin/sh
#
# Force Bourne Shell if not Sun Grid Engine default shell (you never know!)
#
#$ -S /bin/sh
#
# I know I have a directory here so I'll use it as my initial working directory
#
#$ -wd /vol/grid-solar/sgeusers/treierflor 
#
# End of the setup directives
#
# Now let's do something useful, but first change into the job-specific
# directory that should have been created for us
#
# Check we have somewhere to work now and if we don't, exit nicely.
#
if [ -d /local/tmp/treierflor/$JOB_ID.$SGE_TASK_ID ]; then
        cd /local/tmp/treierflor/$JOB_ID.$SGE_TASK_ID
else
        echo "Uh oh ! There's no job directory to change into "
        echo "Something is broken. I should inform the programmers"
        echo "Save some information that may be of use to them"
        echo "There's no job directory to change into "
        echo "Here's LOCAL TMP "
        ls -la /local/tmp
        echo "AND LOCAL TMP treierflor "
        ls -la /local/tmp/treierflor
        echo "Exiting"
        exit 1
fi
#
# Now we are in the job-specific directory so now can do something useful
#
# Stdout from programs and shell echos will go into the file
#    scriptname.o$JOB_ID.$SGE_TASK_ID
#  so we'll put a few things in there to help us see what went on
#
echo ==UNAME==
uname -n
echo ==WHO AM I and GROUPS==
id
groups
echo ==SGE_O_WORKDIR==
echo $SGE_O_WORKDIR
echo ==/LOCAL/TMP==
ls -ltr /local/tmp/
echo ==/VOL/GRID-SOLAR==
ls -l /vol/grid-solar/sgeusers/
#
# OK, where are we starting from and what's the environment we're in
#
echo ==RUN HOME==
pwd
ls
echo ==ENV==
env
echo ==SET==
set
#
echo == WHATS IN LOCAL/TMP ON THE MACHINE WE ARE RUNNING ON ==
ls -ltra /local/tmp | tail
#
echo == WHATS IN LOCAL TMP treierflor JOB_ID AT THE START==
ls -la 
#
# Copy the input file to the local directory
#
#cp /vol/grid-solar/sgeusers/treierflor/krb_tkt_flow.JPG .
# copy pbrt directory
#cp -R /vol/grid-solar/sgeusers/treierflor/cgra408/FinalProject/CGRA408_FinalProject/cmake-build-release/ build/
#cp -R /vol/grid-solar/sgeusers/treierflor/cgra408/FinalProject/CGRA408_FinalProject/scenes/ scenes/
echo ==WHATS THERE HAVING COPIED STUFF OVER AS INPUT==
#ls -la 
# 
# Note that we need the full path to this utility, as it is not on the PATH
#
cd /am/rialto/home1/treierflor/cgra408/FinalProject/CGRA408_FinalProject/
./release/pbrt ./scenes/test.pbrt --fn $SGE_TASK_ID
#/usr/pkg/bin/convert krb_tkt_flow.JPG krb_tkt_flow.png
#
echo ==AND NOW, HAVING DONE SOMTHING USEFUL AND CREATED SOME OUTPUT==
#ls -la
#
# Now we move the output to a place to pick it up from later
#  noting that we need to distinguish between the TASKS
#  (really should check that directory exists too, but this is just a test)
#
#mkdir -p /vol/grid-solar/sgeusers/treierflor/$JOB_ID
#cp krb_tkt_flow.png  /vol/grid-solar/sgeusers/treierflor/$JOB_ID/krb_tkt_flow.$SGE_TASK_ID.png
#
echo "Ran through OK"
