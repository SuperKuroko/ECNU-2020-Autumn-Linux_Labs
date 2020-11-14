#!/bin/bash
echo "">out.txt  #clear the out.txt

for file in /usr/bin/*  #deal with every file in /bin
do
	if [[ ${file:9:1} == "b" ]]   #if first char is b
	then
		filename=${file:9}    #file deletes the path
		information=$(ls -l $file) #through ls getting owner and permission
		permission=${information:0:10} #permission begins at 0,and length is 10
		
		# as for the owner,we don't know the index
		# but we know it's place is between the second blank and third
		# so we search the information, and use begin and end to write down the index
		count=0 #count is to get the number of blank
		for i in $(seq 1 ${#information})
		do
			if [[ ${information:i-1:1} == " " ]]
			then
				count=$[$count+1]
			fi

			if [ $count -eq 2 ]
			then
				begin=$i
			fi

			if [ $count -eq 3 ]
			then
				end=$i
			fi
		done
		len=$[$end-$begin]  # get the length of owner
		owner=${information:$begin:$len} 
		result=$filename" "$owner" "$permission #connect
		echo $result>>out.txt
	fi	
done
chmod o=r out.txt  #set only read for others
