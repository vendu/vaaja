To compile, do

        ./build.sh

Make sure compiler optimizations are left off. :)

I don't think it's a good idea to enable compiler optimization as this would
very likely break the tests; I don't want to actually do anything real with
calculation results not to distort the test results.

I use sleep(1) as an ad hoc way to mess with the system scheduler. I'm in the
hopes to have more accurate values by trying to give entire timeslices to small
operations. I know this is dirty, but it seems to give consistent enough
results here. Your Mileage May Vary (TM). :)

/* vendu */
