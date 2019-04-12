14 a)
1)copy the program in a linux machine and save it in a file loop1.
2)change the file executable file with the command chmod u+x loop1
3)Run the program with command ./loop1 3 3 3 from the same directory where you have saved the file(this is only sample argument, program should work with any integer number)

The output should look something like this
root@arnab-VirtualBox1:/# ./loop1 3 3 -3


-3 is not a positive integer
root@arnab-VirtualBox1:/# 
root@arnab-VirtualBox1:/# ./loop1 3 3
Three arguments required
Usage:./loop1 3 3 3
root@arnab-VirtualBox1:/# 

14 b)
1)copy the program in a linux machine and save it in a file disk-benchmark-background.sh.
2)change the file to an executable file with the command chmod u+x disk-benchmark-background.sh
3)Run the program with command ./disk-benchmark-background.sh from the same directory where you have saved the file.
4) After the program finishes check disk-benchmark-background-log.txt to find out the console output and error.It should
   be in the same directory as the source file.

14 c) 
1)copy the program in a linux machine and save it in a file network-test.sh.
2)change the file to an executable file with the command chmod u+x network-test.sh
3)This program expects a file named network-test-machine-list.txt , this should be in the same location as the 
source file. content of this file should be something like below.

network-test-machine-list.txt
===========================
google.com
iit.edu
espn.org
flipkart.co.in
amazon.com
youtube.com
facebook.com
instagram.com
twitter.com
firefox.com

4)Run the program with command ./network-test.sh from the same directory where you have saved the file.
5)Output of the program should be saved in network-test-latency.txt file.

6)Output should look like. The second column is avg latency.
google.com 32.923
iit.edu 283.394
espn.org 280.546
flipkart.co.in 378.034
amazon.com 393.593
youtube.com 37.644
facebook.com 10.709
instagram.com 364.304
twitter.com 174.389
firefox.com 395.136

14 d)
1) Install ploticus on your ubuntu machine.
2) Run the command on the command line , the graph would show .Input data file should be in the same diretory from where you are running the command.
   or an absolute path has to be provided. 





